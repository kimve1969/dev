#include<iostream>
#include<cassert>
#include<vector>
#include"utility.h"
#include"tomas.h"

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";
	/*
	 *	Ax = d
	 *
	 *	| 2 1 0 |	| 1 |
	 *	| 1 2 1 | x =	| 1 |
	 *	| 0 1 2 |	| 1 |
	 *	
	 *	x = {0.5, 0.0, 0.5}
	 * */

	vec1D_t<double> x = calc_Tomas( vec1D_t{1.0, 1.0}, vec1D_t{2.0, 2.0, 2.0}, vec1D_t{1.0, 1.0}, vec1D_t{1.0, 1.0, 1.0} );
	assert( (x == vec1D_t{0.5, 0.0, 0.5}) );
	prn1D("x: ", x);

	std::cout<<"End\n";
	return EXIT_SUCCESS;
}
