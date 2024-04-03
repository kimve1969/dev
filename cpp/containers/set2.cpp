#include<iostream>
#include<unordered_set>
#include<initializer_list>
#include<string>
#include<functional>

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";
	
	std::initializer_list<std::string> il{"Anna", "Boris", "Cook", "Dad", "Egle", "Gang", "Hand", "Kate", "Lara", "Yang"};

	std::unordered_set<std::string, std::hash<std::string> > us( il );

	std::cout<<"unordered set size = "<<us.size()<<std::endl;
	std::cout<<"bucket count = "<<us.bucket_count()<<std::endl;
	for(auto &e: il)
	{
		auto h = std::hash<std::string>{}(e);
		std::cout<<e<<" :\tbacket = "<<us.bucket(e)<<" :\thash = "<<h<<"\t:\thash%"<<us.bucket_count()<<" = "<<h%us.bucket_count()<<"\n";
	}

	std::cout<<"End\n";
	return EXIT_SUCCESS;
}
