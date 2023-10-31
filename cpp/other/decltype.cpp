#include<iostream>
#include<vector>

template<typename Container, typename Index>
int& f(Container& C, Index i)
{
	return C[i];
}

template<typename Container, typename Index>
// c++11: auto g(Container C, Index i)->decltype(C[i])
// c++14: decltype(auto) g(Container C, Index i)
//	  auto g(Container C, Index i)
auto g(Container& C, Index i)
{
	return C[i];
}

int main(int argc, char* argv[])
{
	std::vector v{1,2,3,4,5};
	std::size_t i = 2;
	std::cout<<"v["<<i<<"] = "<<v[i]<<std::endl;
	std::cout<<"f(v, "<<i<<") = "<<f(v,i)<<std::endl;

	std::vector vs{"one", "two", "three"};
	std::cout<<"vs["<<i<<"] = "<<vs[i]<<std::endl;
	// compiling ERROR std::cout<<"f(vs, "<<i<<") = "<<f(vs,i)<<std::endl;
	std::cout<<"g(vs, "<<i<<") = "<<g(vs,i)<<std::endl;

	return 0;
}
