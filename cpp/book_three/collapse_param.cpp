#include<iostream>
#include<algorithm>
/*
 *	p 37-39 of book three
 * */
template<typename ... Ts>
auto add(Ts ... ts)
{
	return (ts + ...);
}

template<typename ...Ts>
auto sum(Ts ... ts)
{
	return (ts + ... + 0);
}

template<typename Container, typename ...Ts>
auto count_all(Container c, Ts ... ts)
{
	return (std::count(c.begin(), c.end(), ts) + ... );
}

template<typename T, typename ... Ts>
auto within(T min, T max, Ts ... ts)
{
	return ((min <= ts && ts <= max) && ...);
}

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";

	std::cout<<add(1,2,3)<<"\n";

	std::string s1{"Hello"};
	std::string s2{" world!"};
	std::cout<<add(s1, s2)<<"\n";;

	std::cout<<sum()<<"\n";

	std::vector v{1,2,3,4,5,6,7,8,9};

	std::cout<<count_all(v, 2, 5, 15)<<", "<<count_all(v, 15, 16)<<"\n";

	std::string s3{"abcdef"};
	std::cout<<count_all(s3, 'a','c','h')<<"\n";

	std::cout<<std::boolalpha<<within(3,12, 4,8,9,5)<<", "<<within(3,12, 7,8,1)<<"\n";

	std::cout<<"End\n";
	return EXIT_SUCCESS;
}
