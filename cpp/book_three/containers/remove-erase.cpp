#include<iostream>
#include<vector>
#include<algorithm>

/*
 * 	remove-erase idioms
 * 	p 46-49 of book three
 * */
int main(int argc, char* argv[])
{
	std::cout<<"Start\n";

	std::vector v{1,2,3,4,7,8,4,2,6,2,9};

	auto prn = [&v]()
	{
		std::cout<<v.size()<<", "<<v.capacity()<<"\n";
		for(auto &&e: v) std::cout<<e<<" ";
		std::cout<<"\n";
		std::cout<<"-----------------------------\n";
	};

	prn();

	auto new_end = std::remove(v.begin(), v.end(), 2);
	prn();
	std::cout<<*new_end<<"\n";

	v.erase( new_end, v.end() );
	prn();

	auto new_end_if = std::remove_if(v.begin(), v.end(), [](auto x){ return x%2 == 0; } );
	prn();
	std::cout<<*new_end_if<<"\n";

	v.erase( new_end_if, v.end() );
	prn();

	v.shrink_to_fit();
	prn();

	std::cout<<"End\n";
	return EXIT_SUCCESS;
}
