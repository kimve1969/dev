#include<iostream>
#include<vector>
#include<list>
#include<type_traits>
/*
 *	p 31 of book
 * */
template<typename T, typename U>
void add(T t, U u)
{
	if constexpr( std::is_same< T, std::vector<U> >::value )
	{
		std::cout<<" T == std::vector<U> \n";
	}
	else if constexpr( std::is_same< T, std::list<U> >::value  )
	{
		std::cout<<" T == std::list<U> \n";
	}
	else
	{
		std::cout<<" T is not vector && is not list!\n";
	}

}

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";
	
	std::vector vi{1,2,3};
	std::list   li{1,2,3};

	add( vi, 1);
	add( li, 2);
	add( 2.0, 'c');

	std::cout<<"End\n";
	return EXIT_SUCCESS;
}

