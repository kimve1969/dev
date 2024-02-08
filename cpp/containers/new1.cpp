#include<iostream>
#include<cassert>
#include<cstring>

struct A
{
	int a = -1;
	int b = -2;
	int c = -3;
};

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";

	// raw memory
	A* ptr = (A*) malloc(3 * sizeof(A));

	std::cout<<"ptr = "<<ptr<<", size of = "<<3*sizeof(A)<<std::endl;

	auto prn = [](const char* s, A* ptr)
	{
		for(int i=0; i<3 ; ++i)
		{	
			std::cout<<&ptr[i]<<", "<<s<<"["<<i<<"] = "<<ptr[i].a<<", "<<ptr[i].b<<", "<<ptr[i].c<<std::endl;
		}
	};

	prn("ptr", ptr);

	// allocate new array to raw memory
	A* new_ptr = ::new ( &ptr[0] ) A( {1,2,3} );
	::new ( &ptr[1] ) A( {4,5,6} );
	::new ( &ptr[2] ) A( {7,8,9} );
	
	prn("ptr", ptr);
	std::cout<<"\n";

	assert( ptr == new_ptr );
        assert( memcmp(ptr, new_ptr, 3 * sizeof(A) ) == 0 );

	return 0;
}
