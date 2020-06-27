clear all; close all; clc;

[x,Fs] = audioread('sound.wav');

N = length(x);
t = (0:N-1)/Fs;
noise = (0.25*sin(2*pi*1000*t))';
y = x + noise;

Y = fft(y);

n = 0:N-1;
F = n/N;
f = (-N/2:N/2-1)/N;

% Noise Remover Filter
NRF = zeros(N,1);
for i = 1:N 
    if (i<N*0.0226 || i>N*0.0227 ) && (i<N-N*0.0227 || i>N-N*0.0226)
        NRF(i) = 1;
    end
end

Y = Y .* NRF;

figure(1);
plot(f,abs(fftshift(Y)));
title('sound.wav filtered(freq)');
xlabel('cycles/sample');

figure(2);
plot(f,fftshift(NRF));
title('Noise Remover Filter(freq)');
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