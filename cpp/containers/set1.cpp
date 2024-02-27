#include<iostream>
#include<set>
#include<unordered_set>
#include<initializer_list>
#include<cassert>

template< typename C, typename IL = std::initializer_list< typename C::value_type > >
bool operator==(C c, IL il)
{
	if( c.size() != il.size()) return false;

	for(auto itc = c.begin(), itl = il.begin() ; itc != c.end() ; itc++, itl++ )
	{
		if( *itc != * itl) return false;
	}
		return true;
}

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";

	std::initializer_list il{1,3,2,3,1,1,5,4,2,8,9,0,8,7};
	std::cout<<"initializer_list:\n";
	for(auto &e: il) std::cout<<e<<", ";
	std::cout<<std::endl;

	std::set s(il);
	if( not (s == std::initializer_list{0,1,2,3,4,5,7,8,9}) ) std::cout<<"set check failed...\n"; // set has unique elements and is sorted
	std::cout<<"set:\n";
	for(auto &e: s) std::cout<<e<<", "; 	
	std::cout<<std::endl;

	std::multiset ms(il);
	if( not( ms == std::initializer_list{0,1,1,1,2,2,3,3,4,5,7,8,8,9} ) ) std::cout<<"multiset check failed...\n"; // ms has not unique elements and is sorted
	std::cout<<"multiset:\n";													      
	for(auto &e: ms) std::cout<<e<<", ";
	std::cout<<std::endl;

	std::unordered_set us(il);
	std::cout<<"unordered_set:\n";
	for( auto &e: us) std::cout<<e<<", ";
	std::cout<<std::endl;

	std::cout<<"End\n";
	return EXIT_SUCCESS;
}
