#include<iostream>
#include<vector>
#include<algorithm>
/*
 * 	remove element from vector by index, complexity O(1)!
 * 	if use remove-erase idiom the compexity will be O(n)
 *	p 50 of book three
 *
 * */
template<typename Container>
void remove_quick_at(Container &c, std::size_t indx)
{
	if( indx > c.size()-1 ) return;

	c[indx] = std::move( c.back() );
	c.pop_back();
}

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";

	std::vector v{1,2,3,4,5,6,7,8,9};

	const auto prn = [&v]()
	{ 
		for(auto &e:v) std::cout<<e<<" "; 
		std::cout<<"\n";
	};

	prn();
	remove_quick_at(v, 3);	// remove 3-th element of v, complexity O(1)
	prn();
	remove_quick_at(v, 3);	// remove 3-th element of v, complexity O(1)
	prn();

	std::cout<<"End\n";
	return EXIT_SUCCESS;
}
