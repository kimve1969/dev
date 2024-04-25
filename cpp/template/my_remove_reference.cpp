#include<iostream>
#include<cassert>

template<class T> struct my_remove_reference { typedef T value_type; };
template<class T> struct my_remove_reference<T&> { typedef T value_type; };
template<class T> struct my_remove_reference<T&&> { typedef T value_type; };

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";

	static_assert( std::is_same_v< my_remove_reference<int&&>::value_type, int> );

	std::cout<<"End\n";
	return EXIT_SUCCESS;
}
