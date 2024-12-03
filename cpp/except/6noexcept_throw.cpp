#include<iostream>

struct A
{
	A(int x):m(x){ std::cout<<"cnt A( "<<m<<" )\n"; }
	~A(){ std::cout<<"dst ~A( "<<m<<" )\n"; }
	int m;
};

void f() noexcept
{
	throw(1); 
}

A a0(0);

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";

	try
	{
		A a1(1);
		f(); // f() declare as noexcept, but exception handling fails and call terminate()

	}
	catch(int)
	{
		std::cout<<"catch exception int"<<std::endl;
	}

	std::cout<<"End\n";
	return 0;
}
