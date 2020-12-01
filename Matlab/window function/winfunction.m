%本函数用用户指定的窗函数截断理想低通滤波器,输出幅频和相频响应.
%依次输入窗函数,时域点数,滤波器截止频率
function winfunction(varargin)
close all;
%设置可变长参数和缺省值
w_default="rect";p_default=64;wc_default=pi/2;
if nargin==0
    window=w_default;points=p_default;wc=wc_default;
elseif nargin==1
    window=varargin{1};points=p_default;wc=wc_default;
elseif nargin==2
    window=varargin{1};points=varargin{2};wc=wc_default;
elseif nargin==3
    window=varargin{1};points=varargin{2};wc=varargin{3};
end

%生成窗函数wr
n=0:points-1;
wrlist=[rectwin(points)';triang(points)';hann(points)';hamming(points)';blackman(points)';kaiser(points)'];
dict=struct("rect",1,"triang",2,"hann",3,"hamming",4,"blackman",5,"kaiser",6);
wr=wrlist(dict.(window),:);

%生成线性相位低通滤波器的单位冲激响应hd
hd=ideallp(wc,points);     

%用窗函数截断,得到h=hd*wr作为实际设计的低通滤波器单位冲激响应
h=hd.*wr;                       

%得到h的频域特性[Hw,w]
[Hw,w]=freqz(h,1,'whole',1024);          
db=20*log10(abs(Hw));

%作图
%窗函数
subplot(2,2,1);plot(n,wr);grid;title('窗');ylabel('w(n)');
%实际滤波器单位冲激响应
subplot(2,2,2);stem(n,h);title('h(n)波形');
%幅频特性
subplot(2,2,3);plot(w/pi,db);title('幅频特性（dB）');xlabel('\omega/\pi');ylabel('20log|H(e^j^\omega)|(dB)');
set(gca,'XLim',[0 1]);
%相频特性
subplot(2,2,4);theta=angle(Hw);plot(w/pi,theta);title('相频特性');xlabel('\omega/\pi');
set(gca,'XLim',[0 1]);
end
