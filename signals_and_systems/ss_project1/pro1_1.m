clear all; close all; clc;

fs = 100;
t = -5:1/fs:5;
x = (sinc(t)).^2;

N = length(x);
n = 0:N-1;
f = fs*n/N;
X = fft(x);
MX = abs(X)/fs;

subplot(211);
plot(t,x);
grid on;
ylim([-0.2,1.2]);
title('x(t)');
xlabel('t');

subplot(212);
plot(f, MX);
grid on;
title('|X(f)|');
xlabel('f');