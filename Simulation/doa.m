% Implements DOA algorithm on simulated linear array

% Array Parameters
m = 8; % number of microphones
d = 0.10;  % distance between microphones (in meters)
Fs = 48000;  % sampling rate of the microphones

% Signal Parameters
f = 9000;  % frequency of signal of interest
aoa = 20;  % intended angle of arrival (in degrees)
snr = 10;  % signal power to noise power ratio in dBW

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
%     subplot(4, 2, i)
%     plot(Fs/L*(-L/2:L/2-1), angle(spatial_spectrum(i, :)))
%     plot(Fs/L*(-L/2:L/2-1), abs(fftshift(spatial_spectrum(i, :))))
end

% Extract bin with FFT peak
[mx, tbin] = max(spatial_spectrum(1, 1:L/2));

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
