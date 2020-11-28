syms H(w)
H(w)=-(sign(pi/2-n)*0.5+sign(pi/2+n)*0.5);
syms h(t)
%h(t)=int(H(n)*e^(1i*w*t),w);
fplot(H(w))