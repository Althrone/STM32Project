% %生成模拟数据
OX = 100;
OY = 123;
OZ = 345;
Rx = 100;
Ry = 200;
Rz = 150;
fprintf('真实值  : XYZ中心[%0.2f, %0.2f, %0.2f], 半轴长[%1.2f, %1.2f, %1.2f]\n', OX, OY, OZ, Rx, Ry, Rz);
x = [];
y = [];
z = [];
for theta = 0: 0.1: 2*pi%经度
    for phi = 0: 0.1: pi%纬度
        x = [x, Rx * cos(theta) * sin(phi) + OX];
        y = [y, Ry * sin(theta) * sin(phi) + OY];
        z = [z, Rz * cos(phi) + OZ];
    end
end

% %unifrnd(a,b,m,n)%产生m行n列[a,b]之间的随机数
error = 10;
x = x + unifrnd(-error, error, 1, length(x));
y = y + unifrnd(-error, error, 1, length(y));
z = z + unifrnd(-error, error, 1, length(z));
x = x';
y = y';
z = z';
%至此，模拟的三维样本点已生成完毕。实际应用时，这些三维样本点应由传感器采样获得。

clf;
set(figure(1),'NumberTitle','off','Name','椭球拟合') ;
%显示模拟数据
clf;
hold on;
grid on;
axis equal;
xlabel('X轴');
ylabel('Y轴');
zlabel('Z轴');
plot3(x,y,z, 'or');
plot3(x,y,z, '*r');
[VOX, VOY, VOZ, VRX, VRY, VRZ] = ellipsoidFit_gyh(x,y,z);%求解椭球参数

%拟合结果的可视化显示
ellipsoid(VOX, VOY, VOZ, VRX, VRY, VRZ, 50);% ellipsoid(OX, OY, OZ, ra, rb, rc, 50);
alpha(0.01)
plot3([VOX - VRX, VOX + VRX], [VOY, VOY], [VOZ, VOZ], 'LineWidth',5);
plot3([VOX, VOX], [VOY - VRY, VOY + VRY], [VOZ, VOZ],  'LineWidth',5);
plot3([VOX, VOX], [VOY, VOY], [VOZ - VRZ, VOZ + VRZ],  'LineWidth', 5);

fprintf('拟合结果: XYZ中心[%0.2f, %0.2f, %0.2f], 半轴长[%1.2f, %1.2f, %1.2f]\n', VOX, VOY, VOZ, VRX, VRY, VRZ);

%对拟合的结果进行质量评估
Rsphere = sqrt(((x - VOX)/VRX).^2 + ((y - VOY)/VRY).^2 +((z - VOZ)/VRZ).^2);%每一个样本点的归一化球径
figure(3);
%Rsphere(find(Rsphere>1.1)) = [];%误差过大的样本点删掉
hist(Rsphere,50)
Rerror = abs(Rsphere - 1);

fprintf('最大误差: %1.2f%%, 平均误差: %1.2f%% ,误差标准差: %1.7f\n', max(Rerror) * 100, mean(Rerror) * 100, std(Rerror));





