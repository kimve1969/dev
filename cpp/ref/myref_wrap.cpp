#include<iostream>
#include<cassert>

namespace my
{
	template<class T>
	struct ref_wrap
	{
		T* ptr_;
		ref_wrap(T* p):ptr_(p){}
		ref_wrap(const T& rhs){ ptr_ = rhs.ptr_; } 

		// assign
		operator T&(){ std::cout<<"operator T&()\n";  return *ptr_; }
		T& get(){ return *ptr_; }
	};
}

void foo(int x)
{
	std::cout<<"foo()\n";
	x += x;
}

template<class R>
void bar(R x)
{
	std::cout<<"bar()\n";
	x += x;
}

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";

	int x = 1;
	foo(x);
	assert( x == 1);
	std::cout<<x<<"\n";

	my::ref_wrap rx(&x); // deduce my::ref_wrap<int>
	foo(rx);  // call rx.operator int&(), call foo(int)
	std::cout<<x<<"\n";

	bar(rx); // call bar(my::ref_wrap<int> x)
		 // x += x, call twice rx.operator int&() !
	std::cout<<x<<"\n";

	std::cout<<"End\n";
	return 0;
}
