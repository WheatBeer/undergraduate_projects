clear all; close all; clc;
 
fc = 10^6;
fs = 10^7;
t = -3:1/fs:3;
ka = 1;

c = cos(2*pi*fc*t);
m = (sinc(t)).^2;
am = (1+ka*m).*c;

subplot(211);
plot(t,m);
grid on;
title('m(t)');
xlabel('t');

subplot(212);
plot(t,am);
grid on;
title('am(t)');
xlabel('t');

