#include<iostream>

struct A
{
	A(){ std::cout<<"call A()\n"; }
	A(const A&){ std::cout<<"call A(const A&)\n"; }
	A& operator=(const A&){ std::cout<<"call operator(const A&)\n"; return *this; }
	A(A&&){ std::cout<<"call A(A&&)\n";  }
	A& operator=(A&&){ std::cout<<"call operator=(A&&)\n"; return *this; }

	A& get(){ return *this; }
};

A nrvo()
{
	A a;
	return a; // Named RVO
}

A rvo()
{
	return A{}; // RVO
}

A no_rvo()
{
	return A{}.get();
}

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";

	A obj_nrvo = nrvo();
	std::cout<<"----------------------\n";
	A obj_rvo  = rvo();
	std::cout<<"-----------------------\n";
	A obj_no_rvo = no_rvo();
	
	return 0;
}
