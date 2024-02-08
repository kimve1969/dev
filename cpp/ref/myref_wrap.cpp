#include<iostream>
namespace my
{
	template<class T>
	struct ref_wrap
	{
		T* _ptr;
		ref_wrap(T* p):_ptr(p){}
		ref_wrap(const T& rhs){ _ptr = rhs._ptr; } 

		// assign
		operator T&(){ return *_ptr; }
		T& get(){ return *_ptr; }
	};
}

void foo(int x)
{
	x += x;
}

template<class R>
void bar(R x)
{
	x += x;
}

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";

	int x = 1;
	foo(x);
	std::cout<<x<<"\n";

	my::ref_wrap rx(&x); // deduce my::ref_wrap<int>

	foo(rx);  // call foo(int)
	std::cout<<x<<"\n";

	bar(rx); // call bar(my::ref_wrap<int> x)
		 // x += x
	std::cout<<x<<"\n";

	std::cout<<"End\n";
	return 0;
}
