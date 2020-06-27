clear all; close all; clc;

[y,Fs] = audioread('sound.wav');

Y = fft(y);

N = length(Y);
n = 0:N-1;
F = n/N;
f = (-N/2:N/2-1)/N;
t = (0:N-1)/Fs;

% Ideal HPF
HPF = zeros(N,1);
for i = 1:N 
    if i>N/8 && i<N-N/8
        HPF(i) = 1;
    end
end

Y = Y .* HPF;


figure(1);
plot(f,abs(fftshift(Y)));
title('sound.wav filtered(freq)');
xlabel('cycles/sample');
ylim([0,40000]);

figure(2);
plot(f,fftshift(HPF));
title('HPF(freq)');
xlabel('cycles/sample');

% Inverse FT
y_filtered = ifft(Y,'symmetric');

figure(3);
plot(t,y_filtered);
ylim([-2,2]);
title('sound.wav filtered(time)');
xlabel('t');

% play
pl=audioplayer(y_filtered, Fs);
play(pl);