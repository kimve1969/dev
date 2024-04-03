#include<iostream>
#include<memory>

struct A
{
	A(){ std::cout<<"cnt A"<<std::endl; }
	~A(){ std::cout<<"dst A"<<std::endl; }
};

template<typename T>
struct my_deleter
{
	my_deleter(){ std::cout<<"cnt my_deleter"<<std::endl; }
	~my_deleter(){ std::cout<<"dst my_deleter"<<std::endl; }
	void operator()(T* ptr)
	{
		std::cout<<"my_deleter delete ptr"<<std::endl;
		delete ptr;	
	}
};

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";
	
	std::unique_ptr<int> ui( new int{1}  ); //ok
						//
	std::unique_ptr<int> ui1( new int[1000] ); // leak memory
	std::unique_ptr<int[]> ui3(new int[1000] ); // ok

	auto mui = std::make_unique<int[]>( 100 ); // ok
	
	std::cout<<"---------- Enter block -----------\n";
	{
		std::unique_ptr<A, my_deleter<A> > ua(new A);
	}
	std::cout<<"---------- Exit block ------------\n";

	std::cout<<"End\n";
	return EXIT_SUCCESS;
}
