clear all; close all; clc;

[x,Fs] = audioread('sound.wav');

y = awgn(x,10) - x;

N = length(y);
t = (0:N-1)/Fs;

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
ylim([0,5000]);

% play
pl=audioplayer(y, Fs);
play(pl);