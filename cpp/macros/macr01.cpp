#include<iostream>

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";

	std::cout<<"file : "<<__FILE__<<", line : "<<__LINE__<<", function name : "<<__func__<<"\n";

	std::cout<<"End\n";
	return EXIT_SUCCESS;
}
