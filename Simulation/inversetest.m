close all

% Array Parameters
m = 8; % number of microphones
d = 0.104;  % distance between microphones (in meters)
Fs = 48000;  % sampling rate of the microphones

% Signal Parameters
f = 2000;  % frequency of signal of interest
aoa = -5;  % intended angle of arrival (in degrees)
snr = 1000;  % signal power to noise power ratio in dBW

% Trial Parameters
n = 1024;  % samples in data block
snum = 12; % number of sectors to split half-circle into

bnum = snum+1;  % number of beams to form
tdir = 12;  % direction to apply reverse delay in

arrsig = generate_array_signals(m, d, aoa, f, n, Fs, snr);

% Plot signals to verify delay
figure(1);
title("Input Signals")
xlabel("Sample")
ylabel("Value")
hold on;
for n = 1:m
    plot(arrsig(n, 1:50))
end
hold off;

% Generate FFT for each microphone
L = 1024;
spatial_spectrum = zeros(m, L);

% figure(3);
hold on
for n=1:m
    spatial_spectrum(n, :) = fft(arrsig(n, :), L);
%     plot(Fs/L*(-L/2:L/2-1), abs(fftshift(spatial_spectrum(1, :))))
%     plot(Fs/L*(-L/2:L/2-1), angle(spatial_spectrum(n, :)))
end
hold off

% Generate delay matrix
dmat = zeros(m, bnum);  % Rows are sensors, columns are directions
angs = (0:2*pi/bnum:2*pi-2*pi/bnum);
wlength = 343/f;

for n = 1:m  % Iterate over sensors
    shift_constant = (2*pi*d*(n-1))/wlength;
    shvec = shift_constant*sin(angs);
    eshifts = exp(-1i*shvec);
    dmat(n, :) = eshifts;
end

[mx, tbin] = max(spatial_spectrum(1, 1:L/2));

% Apply delays
revsigs = zeros(m, bnum, 1024);
for k = 1:bnum
    for n = 1:m
        signal = spatial_spectrum(n, tbin) * dmat(n, k);
        revsigs(n, k, tbin) = signal;
    end
end

% Apply inverse to realigned signals
invsigs = zeros(m, 1024);
for n = 1:m
    invsigs(n, :) = ifft(revsigs(n, tdir, :), 1024);
end

% Plot signals to verify corrected delay
figure(2);
title("Reverse Delayed Signals")
xlabel("Sample")
ylabel("Value")
hold on;
for n = 1:m
    plot((1:50), real(invsigs(n, 1:50)))
end
hold off;