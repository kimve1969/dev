#include<iostream>
#include<type_traits>

int main(int argc, char* argv[])
{
	using two_t	= std::integral_constant<int, 2>;
       	using four_t	= std::integral_constant<int, 4>;

	static_assert(not std::is_same<two_t, four_t>::value, "two_t and four_t are not equal!");
	static_assert( two_t::value * 2 == four_t::value /* 2*2 == 4 */);

	if(not false) std::cout<<"not!"; 

	return 0;
}
