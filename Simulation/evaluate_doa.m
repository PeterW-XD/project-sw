function [doa_res, pmax, tbin] = evaluate_doa(input_signal, m, d, f, snum)
    % Array Parameters
    % m: number of microphones
    % d: distance between microphones (in meters)

    % Signal Parameters
    % f: frequency of signal of interest
    % snum: number of sectors to split half-circle into

    % Trial Parameters
    bnum = snum+1;  % number of beams to form

    % Generate FFT for each microphone
    L = 1024;
    spatial_spectrum = zeros(m, L);

    for i=1:m
        spatial_spectrum(i, :) = fft(input_signal(i, :), L);
    end

    % Extract bin with FFT peak
    [~, tbin] = max(spatial_spectrum(1, 30:L/2));
    tbin = tbin + 30;

    % Compute delay matrix
    dmat = zeros(m, bnum);  % Rows are sensors, columns are directions
    angs = (-pi/2:pi/snum:pi/2);
    wlength = 343/f;

    for n = 1:m  % Iterate over sensors
        shift_constant = (2*pi*d*(n-1))/wlength;
        shvec = shift_constant*sin(angs);
        eshifts = exp(-1i*shvec);
        dmat(n, :) = eshifts./abs(eshifts);
    end

    % Apply delays and calculate power
    sigvec = spatial_spectrum(:, tbin);
    outvec = dmat.'*sigvec;
    pwrvec = abs(outvec).^2;

    [pmax, ind] = max(pwrvec);
    doa_res = angs(ind)*180/pi;
end
