clear;

% step=0.01;%积分步长
% t=0:step:pi;

% x1=1:length(t);
% x1(1)=sin(t(1));
% for i=2:length(t)
%    y=(sin(t(i))+x1(i-1))*step;
%    x1(i)=y;
% end
% plot(t,x1);

syms t;
order =1;
f1=int(sin(t/order),t,0,t);
ezplot(f1,[0,1*pi]);
hold;

f2=int(1-cos(t/2),t,0,t);
ezplot(f2,[0,4*pi]);