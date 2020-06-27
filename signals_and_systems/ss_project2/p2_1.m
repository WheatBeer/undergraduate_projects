clear all; close all; clc;

[x,Fs] = audioread('sound.wav');

N = length(x);
t = (0:N-1)/Fs;

y = x + 0.2*randn(N,1);

figure(1);
plot(t,y);
ylim([-2,2]);
title('sound.wav(time)');
xlabel('t');


sound_fft = fftshift(fft(y));
N = length(sound_fft);
f = (-N/2:N/2-1)/N;


figure(2);
plot(f,abs(sound_fft))
title('sound.wav(freq)');
xlabel('cycles/sample');

% play
pl=audioplayer(y, Fs);
play(pl);