#include<iostream>
#include<map>


int main(int argc, char* srgv[])
{
	std::cout<<"Start\n";

	std::map< std::string, std::string > m{ {"Dad", "+789224035320"}, {"Mam", "+79222520266"}, {"George", "Unknown"}};

	const std::string key = "Mam"; 
	std::cout<<"key = "<<key<<", value = "<<m.find(key)->second<<std::endl;	

	std::cout<<"End\n";
	return EXIT_SUCCESS;
}


