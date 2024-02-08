#include<iostream>
#include<vector>
#include<string>
#include<type_traits>

template<class T>
std::ostream& operator<<(std::ostream& o, std::vector<T>& c)
{
	for(auto &&e: c){ o<<e<<", "; }
	return o;
}

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";

	std::vector<std::string> v1{"one", "two", "three"};
	std::vector<std::string> v2(v1.begin(), v1.end());
	std::vector<std::string> v3(std::move_iterator(v1.begin()), std::move_iterator(v1.end()));

	std::cout<<"v1 = "<<v1<<"size = "<<v1.size()<<"\n";
	std::cout<<"v2 = "<<v2<<"\n";
	std::cout<<"v3 = "<<v3<<"\n";

	std::iterator<std::output_iterator_tag, std::ostream> o;

	std::vector<std::string> v4(v3.size());

	std::copy(v3.begin(), v3.end(), v4.begin());
	std::cout<<"v4 = "<<v4<<"\n";

	std::cout<<"End\n";

	return 0;
}
