%本实验模块用于产生矩形窗
clc;clear;
N=[15,33,51,64];i=4;               %选择时域点数                      
wc=pi/2;                        %理想低通滤波器的截止频率
S=4;                            %选择使用的窗函数
n=0:N(i)-1;
wrlist=[rectwin(N(i))';triang(N(i))';hann(N(i))';hamming(N(i))'];              
wr=wrlist(S,:);                 %得到一个窗向量wr
hd=ideallp(wc,N(i));            %理想线性相位低通滤波器的单位冲激响应
h=hd.*wr;                       %实际低通滤波器的单位冲激响应
[Hw,w]=freqz(h,1,'whole',1024); %得到滤波器的频域特性         
db=20*log10(abs(Hw));           %幅频响应(分贝表示)
%作图
%矩形窗
subplot(2,2,1);plot(n,wr);grid;title('窗');ylabel('w(n)');
%实际滤波器单位冲激响应
subplot(2,2,2);stem(n,h);title('h(n)波形');
%幅频特性
subplot(2,2,3);plot(w/pi,db);title('幅频特性（dB）');xlabel('\omega/\pi');ylabel('20log|H(e^j^\omega)|(dB)');
set(gca,'XLim',[0 1]);%将横坐标限制在0到1之间画出幅频特性
%相频特性
subplot(2,2,4);theta=angle(Hw);plot(w/pi,theta);title('相频特性');xlabel('\omega/\pi');
set(gca,'XLim',[0 1]);%将横坐标限制在0到1之间画出相频特性
