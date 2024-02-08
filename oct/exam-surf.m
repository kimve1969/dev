[x y] = meshgrid(-2:0.1:2, 0:0.1:4);
z=sqrt(sin(x).^2 + cos(y).^2);
z2=2 + sqrt(cos(x).^2 + sin(y).^2);
surf(x,y,z);
hold on
surf(x,y,z2);
