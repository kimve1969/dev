#include<iostream>

struct A
{
	A(int x):m(x){ std::cout<<"cnt A( "<<m<<" )\n"; }
	~A(){ std::cout<<"dst ~A( "<<m<<" )\n"; }
	int m;
};

void f()
{
	A a3(3);
}

void g()
{
	A a2(2);
	f();
}

A a0(0);

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";

	A a1(1);
	g();

	std::cout<<"End\n";
	return 0;
}
