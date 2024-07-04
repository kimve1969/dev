# F(x) = 60*x1 + 40*x2 + 80*x3 + 100*x4 -> max
# x1 + x2 + x3 + x4 <= 20
# 2*x1 + 6*x2 + 4*x3 + 2*x4 <=60
# 4*x1 + 6*x2 + 10*x3 + 4*x4 <=40
# xj >= 0, j=1,4
# 
#	Answer: X=(0,0,0,10), F(x)=1000
#

c = [60, 40, 80, 100]
A = [1,1,1,1; 2,6,4,2; 4,6,10,4]
b = [20, 60, 40]
lb = [0,0,0,0]
ub = []
ctype = "UUU"
vartype = "CCCC"
s = -1

param.msglev = 1;
param.itlim = 100;

[xmax, fmax, status, extra] = glpk(c, A, b, lb, ub, ctype, vartype, s, param)

