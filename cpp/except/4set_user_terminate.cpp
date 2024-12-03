#include<iostream>
#include<execinfo.h>
#include<signal.h>
#include<exception>

void user_terminate()
{
	void* callstack[128];
	int frames = backtrace(callstack, 128);
	char** strs = backtrace_symbols(callstack, frames);
	std::cerr<<"Stack trace has "<<frames<<" frames:\n";
	
	/*for(int i=0; i < frames; ++i)
	{
		std::cout << strs[i] << std::end;
	}*/
	free(strs);
	exit(1);
}

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

	std::set_terminate( user_terminate );

	A a1(1);
	g();

	std::cout<<"End\n";
	return 0;
}
