clear all; close all; clc;

f0 = 10^6;
fs = f0*1000;
t = 0:1/fs:3/f0;
h = (cos(2*pi*f0*t)).^2;

N = length(h);
n = 0:N-1;
f = fs*n/N;
H = fft(h);
MH = abs(H)/N;

subplot(211);
plot(t,h);
grid on;
title('h(t)');
xlabel('t');

subplot(212);
plot(f,MH);
grid on;
title('|H(f)|');
xlabel('f');