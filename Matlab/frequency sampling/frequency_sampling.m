syms H(omega), omega;
H_target = 0.5 - 0.5 * sign(omega - pi / 2);

N = 64;
H_sample = double(subs(H_target, omega, (0:N-1) / N * 2 * pi));
%H_sample(17) = 0.3781;
%H_sample(17:18) = [0.591836, 0.108795];
%H_sample(16:18) = [0.667603, 0.23647, 0.0386697];
[H_sample,delta] = linprog_opt(H_sample, 15:17, 1024, 18/32*pi, 10)
%H_sample = linearPhase(H_sample);
h = ifft(H_sample);

N = 8192;
[H,w] = freqz(h,1,N);
omega_axis = (0 : N-1) / N * pi; 

Hw = winfunction('kaiser');

%[H,w] = interpolate(H_sample,N);

% subplot(2,2,1);
% stem(abs(H_sample));
% title('幅频响应取样');
% 
% subplot(2,2,2);
% stem(real(h));
% title('时域');

% subplot(2,2,3);
plot(omega_axis, 20 * log10(abs(H)));
hold on;
plot(omega_axis, 20 * log10(abs(Hw)));
title('幅频响应');
axis([0, pi, -150, 10]);
xticks([0,pi/4,pi/2,3*pi/4,pi]);
xticklabels({'0','\pi/4','\pi/2','3\pi/4','\pi'});

% subplot(2,2,4);
% plot(omega_axis, angle(H));
% title('相频响应');
% axis([0, pi, -1.2 * pi, 1.2 * pi]);
% xticks([0,pi/4,pi/2,3*pi/4,pi]);
% xticklabels({'0','\pi/4','\pi/2','3\pi/4','\pi'});
