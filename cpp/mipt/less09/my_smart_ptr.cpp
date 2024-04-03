#include<iostream>
#include<cstdlib>
#include<ctime>

template<typename T>
struct my_smart_ptr
{
	T* _ptr;
	my_smart_ptr(T* p):_ptr(p){ std::cout<<"cnt, *_ptr == "<<*_ptr<<std::endl; }
	~my_smart_ptr()
	{
		std::cout<<"dtr, *_ptr == "<<*_ptr<<std::endl;
		delete _ptr; 
	}

	T& operator*(){ return *_ptr; }
	T* operator->(){ return _ptr; }
};

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";

	std::srand( std::time(nullptr) );
	int rnd = std::rand()%2;

	my_smart_ptr<int> pi1( new int{rnd} );	// create and capture resource

	if(rnd == 0)
	{
		return EXIT_SUCCESS; // EXIT 1
	} // call dst pi1
	// rnd == 1
	
	{
		my_smart_ptr<int> pi2 = pi1; // default copy cnt, one resourse and two pointers!
	} // dst pi2

	std::cout<<"End\n";
	return EXIT_SUCCESS; // EXIT 2
} // call dst pi1 is double free resource. Program is aborted.
 
