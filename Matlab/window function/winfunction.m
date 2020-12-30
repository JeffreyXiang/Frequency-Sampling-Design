%���������û�ָ���Ĵ������ض������ͨ�˲���,�����Ƶ����Ƶ��Ӧ.
%�������봰����,ʱ�����,�˲�����ֹƵ��
function [Hw] = winfunction(varargin)
close all;
%���ÿɱ䳤������ȱʡֵ
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

%���ɴ�����wr
n=0:points-1;
wrlist=[rectwin(points)';triang(points)';hann(points)';hamming(points)';blackman(points)';kaiser(points, 6)'];
dict=struct("rect",1,"triang",2,"hann",3,"hamming",4,"blackman",5,"kaiser",6);
wr=wrlist(dict.(window),:);

%����������λ��ͨ�˲����ĵ�λ�弤��Ӧhd
hd=ideallp(wc,points);     

%�ô������ض�,�õ�h=hd*wr��Ϊʵ����Ƶĵ�ͨ�˲�����λ�弤��Ӧ
h=hd.*wr;                       

%�õ�h��Ƶ������[Hw,w]
[Hw,w]=freqz(h,1,8192);          
db=20*log10(abs(Hw));

% %��ͼ
% %������
% subplot(2,2,1);plot(n,wr);grid;title('��');
% %ʵ���˲�����λ�弤��Ӧ
% subplot(2,2,2);stem(n,h);title('ʱ��');
% %��Ƶ����
% subplot(2,2,3);plot(w,db);
% title('��Ƶ��Ӧ');
% axis([0, pi, -100, 10]);
% xticks([0,pi/4,pi/2,3*pi/4,pi]);
% xticklabels({'0','\pi/4','\pi/2','3\pi/4','\pi'});
% 
% %��Ƶ����
% subplot(2,2,4);theta=angle(Hw);plot(w,theta);
% title('��Ƶ��Ӧ');
% axis([0, pi, -1.2 * pi, 1.2 * pi]);
% xticks([0,pi/4,pi/2,3*pi/4,pi]);
% xticklabels({'0','\pi/4','\pi/2','3\pi/4','\pi'})

end
