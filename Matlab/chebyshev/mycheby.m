function [b,a]=mycheby(fs,Wp,Ws,Rp,Rs,N)
%Wp通带截至频率
%Ws阻带起始频率
%Rp通带波纹
%Rs阻带最小衰减
%fs数字信号采样频率
[n,wp]=cheb1ord(Wp/(fs/2),Ws/(fs/2),Rp,Rs);
%返回Chebyshev类型I滤波器的最低阶n，该滤波器在通带中损失不超过Rp dB，并且在阻带中具有至少Rs dB的衰减。还返回相应截止频率wp的标量（或矢量）
[b,a]=cheby1(n,Rp/(fs/2),wp);%b和a分别对应离散系统的系统函数H(z)的分子、分母多项式的系数向量
[H,w]=freqz(b,a,N);%H包含了离散系统对应区间内N（N为正整数）个频率等分点的频率响应，w为N个频率等分点的值
Hf=abs(H);
Hx=angle(H);
clf
subplot(1,2,1)
plot(w,20 * log10(Hf));%幅度响应，纵单位为分贝，横坐标为频率
axis([0, pi, -100, 10]);
xticks([0,pi/4,pi/2,3*pi/4,pi]);
xticklabels({'0','\pi/4','\pi/2','3\pi/4','\pi'});
title('幅频特性曲线')
subplot(1,2,2)
plot(w,Hx)
title('相频特性曲线')
end