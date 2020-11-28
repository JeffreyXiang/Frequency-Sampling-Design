%用于生成一个理想低通滤波器
function hd=ideallp(wc,N)
epsilon=1e-4;
tau=(N-1)/2;
n=0:(N-1);
m=n-tau+epsilon;
hd=sin(wc*m)./(pi*m);
%plot(m,hd);
