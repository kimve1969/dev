#include<iostream>
#include<cstdlib>
#include<ctime>

template<typename T>
struct my_unique_ptr
{
	T* _ptr;
	my_unique_ptr(T* p):_ptr(p){ std::cout<<"cnt, *_ptr == "<<*_ptr<<std::endl; }
	~my_unique_ptr()
	{
		if(_ptr) std::cout<<"dtr, *_ptr == "<<*_ptr<<std::endl;
		else	 std::cout<<"dtr, _ptr == nullptr"<<std::endl;

		delete _ptr; 
	}

	T& operator*(){ return *_ptr; }
	T* operator->(){ return _ptr; }

	// RULE OF FIVE
	my_unique_ptr() = delete;
	my_unique_ptr(const my_unique_ptr&) = delete;
	my_unique_ptr& operator=(const my_unique_ptr&) = delete;
	my_unique_ptr(my_unique_ptr&& rhs):_ptr(rhs._ptr)
	{
		std::cout<<"move cnt"<<std::endl;	
		_ptr = nullptr;  
	}
	my_unique_ptr& operator=(my_unique_ptr&& rhs)
	{
	        delete _ptr;
		_ptr = nullptr;
		std::swap( _ptr, rhs._ptr );	
	}
};

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";

	std::srand( std::time(nullptr) );
	int rnd = std::rand()%2;

	my_unique_ptr<int> ui1( new int{rnd} );	// create and capture resource

	if(rnd == 0)
	{
		return EXIT_SUCCESS; // EXIT 1
	} // call dst ui1
	
	// rnd == 1
	{
		my_unique_ptr<int> ui2 = std::move(ui1); // ui1 set empty
	} // dst ui2

	std::cout<<"End\n";
	return EXIT_SUCCESS; // EXIT 2
} // call dst ui1
 
