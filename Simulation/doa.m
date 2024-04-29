% Implements DOA algorithm on simulated linear array
close all

% Array Parameters
m = 4; % number of microphones
d = 0.104;  % distance between microphones (in meters)
Fs = 48000;  % sampling rate of the microphones

% Signal Parameters
f = 2000;  % frequency of signal of interest
aoa = 60;  % intended angle of arrival (in degrees)
snr = -15;  % signal power to noise power ratio in dBW

% Trial Parameters
n = 1024;  % samples in data block
snum = 12; % number of sectors to split half-circle into

bnum = snum+1;  % number of beams to form
arrsig = generate_array_signals(m, d, aoa, f, n, Fs, snr);

% Plot signals to verify delay
figure(1);
title("Input Signals")
xlabel("Sample")
ylabel("Value")
hold on;
for i = 1:m
    plot(arrsig(i, 1:50))
end
hold off;

% Generate FFT for each microphone
L = 1024;
spatial_spectrum = zeros(m, L);

% figure(2);
for i=1:m
    spatial_spectrum(i, :) = fft(arrsig(i, :), L);
    subplot(4, 2, i)
%     plot(Fs/L*(-L/2:L/2-1), angle(spatial_spectrum(i, :)))
    plot(Fs/L*(-L/2:L/2-1), abs(fftshift(spatial_spectrum(i, :))))
end

% Extract bin with FFT peak
[~, tbin] = max(spatial_spectrum(1, 1:L/2));

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

% Scale delay matrix to fit in 14-bit integer.
% dmat = dmat .* 10^3;

% Apply delays and calculate power
sigvec = spatial_spectrum(:, tbin);
outvec = dmat.'*sigvec;
pwrvec = abs(outvec).^2;

[pmax, ind] = max(pwrvec);
doa_res = angs(ind)*180/pi;

% Plot PSD
figure(3);
polarplot(angs, pwrvec);
title("PSD");
thetalim([-90, 90]);
ax = gca;
ax.ThetaZeroLocation = 'top';
ax.ThetaDir = 'clockwise';