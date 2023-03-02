# numeric solve of linea equtations
#
#  x1 = 0.32x1 - 0.23x2 + 0.41x3 - 0.06x4 + 0.67
#  x2 = 0.18x1 + 0.12x2 - 0.33x3 - 0.88
#  x3 = 0.12x1 - 0.32x2 - 0.05x3 + 0.67x4 - 0.18
#  x4 = 0.05x1 - 0.11x2 + 0.09x3 - 0.12x4 + 1.44
#  
#  x = Cx + b

C = [0.32 -0.23  0.41 -0.06;
     0.18  0.12 -0.33  0.00;
     0.12 -0.32 -0.05  0.67;
     0.05 -0.11  0.09 -0.12]
     
b = [0.67; -0.88; -0.18; 1.44]

printf('****************Numeric solve of linea equtation********************\n\n');
epsilon = 0.0001

printf('Normas of C matrix:\n');
printf('norma-1 C == %d\n',norm(C,1));
printf('norma-2 C == %d\n',norm(C,2));
printf('norma-inf C == %d\n\n',norm(C,inf));

# x= Cx+b
function retval = fx(k, C, x, b)
  tmp = 0.0;
  for i=[1:1:4]
    tmp += C(k,i) * x(i);
  endfor
  retval = tmp + b(k);
endfunction

function retval = fdelta(xk, x0)
  #tmp = zeros(length(xk));
  for k=[1:1:length(xk)]
    tmp(k)=abs(xk(k)-x0(k));
  endfor
  retval = tmp';

endfunction

function retval = is_less_epsilon(xk, x0, ep)
  tmp = fdelta(xk, x0);
  for i=[1:1:length(xk)]
    if (tmp(i) > ep)
      retval = false;
      return;
    endif
  endfor
  retval = true;

endfunction

# initial vector x as vector b
y0 = b  # 0 or k-1 iteration
yk = b; # k iteration

MAXSTEP = 30;
xstep = 1;
do
  printf('----------step N %i-------------\n', xstep);
  for k = [1:1:4]
    yk(k) = fx(k, C, y0, b);
  endfor
  yk
  delta = fdelta(yk, y0)
  b_less = is_less_epsilon(yk, y0, epsilon)
  y0 = yk;
  
  xstep++;
until(b_less || xstep > MAXSTEP)

printf('\n*****************************Check solve Ax=b, as x=A\b*********************\n');
B=C;
o=-ones(4,1);
B(1,1)=(C+o)(1,1);
B(2,2)=(C+o)(2,2);
B(3,3)=(C+o)(3,3);
B(4,4)=(C+o)(4,4);
B\(-b)

