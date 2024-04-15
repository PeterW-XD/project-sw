close all

% Array Parameters
m = 8; % number of microphones
d = 0.104;  % distance between microphones (in meters)
Fs = 48000;  % sampling rate of the microphones

% Signal Parameters
f = 2000;  % frequency of signal of interest
snr = 20;  % signal power to noise power ratio in dBW

% Trial Parameters
n = 1024;  % samples in data block
snum = 12; % number of sectors to split half-circle into
angstep = 1;

bnum = snum+1;  % number of beams to form
angs = (-90:angstep:90);
res = zeros(2, length(angs));

for a = 1:length(angs)
    arrsig = generate_array_signals(m, d, angs(a), f, n, Fs, snr);

    [resang, respwr, ~] = evaluate_doa(arrsig, m, d, f, snum);
    res(:, a) = [resang; respwr];
end

figure(1);
stem(angs, res(1, :))
title("Actual vs. Calculated Angle at SNR = 20 dB");
xlabel("Actual AOA");
ylabel("Calculated AOA");

figure(2);
stem(angs, res(2, :))
title("Actual AOA vs. Max Power");
xlabel("Actual AOA");
ylabel("Max Power");