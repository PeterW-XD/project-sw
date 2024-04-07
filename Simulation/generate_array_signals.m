function outmat = generate_array_signals(m, d, aoa, f, n, Fs, snr)
%GENERATE_SIGNALS Synthesizes n samples of data for an m-element linear array
%   m: number of sensors
%   d: distance between sensors
%   aoa: angle of arrival
%   f: signal frequency
%   n: number of samples
%   Fs: sampling rate
%   snr: signal to noise power ratio

% Create base signal: n samples of f Hz sine wave sampled at Fs
svec = (1:n);
tvec = svec/Fs;

% Create time delay without (i-1) term
wlength = 343/f;
tdelay = 2*pi*d*sin(pi*aoa/180)/wlength;

% Create (m x n) output vector where each row is the original signal 
% delayed the appropriate amount
outmat = zeros(m, n);

for i = 1:m
    outmat(i, :) = awgn(sin(2*pi*f*tvec + (i-1)*tdelay), snr, 'measured');
%     outmat(i, :) = cos(2*pi*f*tvec + (i-1)*tdelay);
end
end

