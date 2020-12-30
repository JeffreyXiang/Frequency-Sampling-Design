function [b,a]=mycheby(fs,Wp,Ws,Rp,Rs,N)
%Wpͨ������Ƶ��
%Ws�����ʼƵ��
%Rpͨ������
%Rs�����С˥��
%fs�����źŲ���Ƶ��
[n,wp]=cheb1ord(Wp/(fs/2),Ws/(fs/2),Rp,Rs);
%����Chebyshev����I�˲�������ͽ�n�����˲�����ͨ������ʧ������Rp dB������������о�������Rs dB��˥������������Ӧ��ֹƵ��wp�ı�������ʸ����
[b,a]=cheby1(n,Rp/(fs/2),wp);%b��a�ֱ��Ӧ��ɢϵͳ��ϵͳ����H(z)�ķ��ӡ���ĸ����ʽ��ϵ������
[H,w]=freqz(b,a,N);%H��������ɢϵͳ��Ӧ������N��NΪ����������Ƶ�ʵȷֵ��Ƶ����Ӧ��wΪN��Ƶ�ʵȷֵ��ֵ
Hf=abs(H);
Hx=angle(H);
clf
subplot(1,2,1)
plot(w,20 * log10(Hf));%������Ӧ���ݵ�λΪ�ֱ���������ΪƵ��
axis([0, pi, -100, 10]);
xticks([0,pi/4,pi/2,3*pi/4,pi]);
xticklabels({'0','\pi/4','\pi/2','3\pi/4','\pi'});
title('��Ƶ��������')
subplot(1,2,2)
plot(w,Hx)
title('��Ƶ��������')
end