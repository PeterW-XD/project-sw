% Applies DOA algorithm to real recorded data
close all;

% Array Parameters
m = 4; % number of microphones
d = 0.104;  % distance between microphones (in meters)
Fs = 48000;  % sampling rate of the microphones

% Algorithm Parameters
snum = 12; % number of sectors to split half-circle into

% Signal Properties
datapath = "RecordedData\Apr14_60\";
f = 2000;  % frequency of signal of interest
samplestart = 1024;
samplecount = 2048;
dstrings = ["L", "R"];

clear arrsig;

% Read in full signal
for i = 1:m/2
    for j = 1:2
        fpath = datapath + "data" + i + "_" + dstrings(j) + ".txt";
        fileID = fopen(fpath);
        
        arrsig(i+j-mod(i, 2), :) = fscanf(fileID, "%i").';
    end
end

% Get subset
insig = arrsig(:, samplestart:samplestart+samplecount-1);
[resang, respwr, tbin] = evaluate_doa(insig, m, d, f, snum);

% Plot signals
figure(1);
title("Input Signals")
xlabel("Sample")
ylabel("Value")
hold on;
for n = 1:m
    plot(arrsig(n, samplestart:samplestart+samplecount-1))
end
hold off;

% Generate FFT for each microphone
L = 1024;
spatial_spectrum = zeros(m, L);
fmax = tbin*Fs/L;

figure(2);
for n=1:m
    spatial_spectrum(n, :) = fft(arrsig(n, :), L);
    subplot(m, 1, n);
    plot(Fs/L*(-L/2:L/2-1), abs(fftshift(spatial_spectrum(n, :))))
%     plot(abs(spatial_spectrum(n, :)))
%     plot(Fs/L*(-L/2:L/2-1), angle(spatial_spectrum(n, :)))
end
