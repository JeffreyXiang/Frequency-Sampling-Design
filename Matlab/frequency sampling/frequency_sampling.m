syms H(omega), omega;
H_target = 0.5 - 0.5 * sign(omega - pi / 2);

N = 64;
H_sample = double(subs(H_target, omega, (0:N-1) / N * 2 * pi));
H_sample(17) = 0.377759;
%H_sample(17:18) = [0.5886, 0.1065];
%H_sample(16:18) = [0.667603, 0.23647, 0.0386697];
H_sample = linearPhase(H_sample);
h = ifft(H_sample);

N = 8192;
[H,w] = freqz(h,1,N);
omega_axis = (0 : N-1) / N * pi; 

subplot(2,2,1);
fplot(H_target,[0,pi]);
xticks([0,pi/4,pi/2,3*pi/4,pi]);
xticklabels({'0','\pi/4','\pi/2','3\pi/4','\pi'});
title('目标幅频响应');

subplot(2,2,2);
stem(real(h));
title('时域');

subplot(2,2,3);
plot(omega_axis, 20 * log10(abs(H)));
title('幅频响应');
axis([0, pi, -100, 10]);
xticks([0,pi/4,pi/2,3*pi/4,pi]);
xticklabels({'0','\pi/4','\pi/2','3\pi/4','\pi'});

subplot(2,2,4);
plot(omega_axis, angle(H));
title('相频响应');
axis([0, pi, -1.2 * pi, 1.2 * pi]);
xticks([0,pi/4,pi/2,3*pi/4,pi]);
xticklabels({'0','\pi/4','\pi/2','3\pi/4','\pi'});
