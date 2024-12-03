#include<iostream>
#include<exception>

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
	throw(1);
	f();
}

A a0(0);

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";

	try
	{
		A a1(1);
		g();
	}
	catch(int)
	{
		std::cout<<"see above - call dst befor throw exception\n";
		std::cout<<"catch exception int\n";
		std::cout<<"see below - call dst after throw exception\n";
	}

	std::cout<<"End\n";
	return 0;
}
