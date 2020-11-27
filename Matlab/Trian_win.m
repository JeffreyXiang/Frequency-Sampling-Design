%��ʵ��ģ�����ڲ������δ�
clc;clear;
N=[15,33,51];i=3;               %ѡ��ʱ�����                      
wc=pi/4;                        %�����ͨ�˲����Ľ�ֹƵ��
n=0:N(i)-1;
wr=triang(N(i))';              %�õ�һ�����δ�����wr
hd=ideallp(wc,N(i));            %����������λ��ͨ�˲����ĵ�λ�弤��Ӧ
h=hd.*wr;                       %ʵ�ʵ�ͨ�˲����ĵ�λ�弤��Ӧ
[Hw,w]=freqz(h,1,'whole',1024); %�õ��˲�����Ƶ������         
db=20*log10(abs(Hw));            %��Ƶ��Ӧ(�ֱ���ʾ)
%��ͼ
%���δ�
subplot(2,2,1);plot(n,wr);grid;title('���δ�');ylabel('w(n)');
%ʵ���˲�����λ�弤��Ӧ
subplot(2,2,2);stem(n,h);title('h(n)����');
%��Ƶ����
subplot(2,2,3);plot(w/pi,db);title('��Ƶ���ԣ�dB��');xlabel('\omega/\pi');ylabel('20log|H(e^j^\omega)|(dB)');
set(gca,'XLim',[0 1]);%��������������0��1֮�仭����Ƶ����
%��Ƶ����
subplot(2,2,4);theta=angle(Hw);plot(w/pi,theta);title('��Ƶ����');xlabel('\omega/\pi');
set(gca,'XLim',[0 1]);%��������������0��1֮�仭����Ƶ����
