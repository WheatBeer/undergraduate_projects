clear all; close all; clc;
 
fc = 10^6;
fs = 10^7;
t = -3:1/fs:3;

c = (cos(2*pi*fc*t)).^2;
m = (sinc(t)).^2;
g = m.*c;

N = length(g);
n = 0:N-1;
f = fs*n/N;
G = fft(g);
MG = abs(G)/fs;

subplot(211);
plot(t,g);
grid on;
title('g(t)');
xlabel('t');

subplot(212);
plot(f,MG);
grid on;
title('|G(f)|');
xlabel('f');

