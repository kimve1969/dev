#include<iostream>
#include<vector>
#include<algorithm>

/*
 *	placement-new constructor
 *
 * */

struct A
{
	bool operator ==(const A& a)
	{
		return (*this).m_ == a.m_;
	}
	A(int a){ m_ = a;  std::cout<<"A("<<a<<")\n"; }
	A(const A& a){ m_ = a.m_; std::cout<<"cstr copy A("<<a.m_<<")\n"; }
	~A(){ std::cout<<"~A("<<m_<<")\n"; }
	int m_;
};

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";

	std::vector<A> vA;
	vA.reserve(3);
	std::cout<<"capacity = "<<vA.capacity()<<"\n";
	vA.emplace_back(1);
	vA.emplace_back(2);
	vA.emplace( vA.begin()+2, 3 );
	auto new_end_vA = std::remove( vA.begin(), vA.end(), A{2});
	vA.erase( new_end_vA, vA.end() );

	std::cout<<"End\n";
	return EXIT_SUCCESS;
}
