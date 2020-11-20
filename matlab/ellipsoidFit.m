function [VOX, VOY, VOZ, VRX, VRY, VRZ] = ellipsoidFit(x,y,z)
%椭球拟合
%形参：xyz为要拟合的三维样本数据，必须输入列向量, sample为Nx3
%返回值：Ox, Oy, Oz为拟合出的椭球中心，Rx, Ry, Rz为三个半轴的长度

% x = sample(:, 1);
% y = sample(:, 2);
% z = sample(:, 3);

K = [y.^2, z.^2, x, y, z, ones(length(x), 1)];
Y = -x.^2;
V=(K'*K)^(-1)*K'*Y;

VOX = -V(3)/2;% -C/2
VOY = -V(4)/V(1)/2;% -D/A/2
VOZ= -V(5)/V(2)/2;% -E/B/2
VRX = sqrt(VOX^2 + V(1)*VOY^2 + V(2)*VOZ^2 - V(6));% sqrt(VOX^2 + A*VOY^2 + B*VOZ^2 - F)
VRY = sqrt(VRX^2/V(1));% sqrt(VRX^2/A);
VRZ = sqrt(VRX^2/V(2));% sqrt(VRX^2/B);
end
