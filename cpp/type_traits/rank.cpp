#include<iostream>
#include<type_traits>

int main(int argc, char* argv[])
{
	int a;
	int a1[1];
	int a2[1][2];
	int a3[1][2][3];
	int ax[][3] = {{0,1,2}};
	
	static_assert( std::rank< decltype(a) >::value == 0  );
	static_assert( std::rank< decltype(a1) >::value == 1  );
	static_assert( std::rank< decltype(a2) >::value == 2  );
	static_assert( std::rank< decltype(a3) >::value == 3  );
	static_assert( std::rank< decltype(ax) >::value == 2  );

	return 0;
}
