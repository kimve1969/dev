#include<iostream>
#include<cassert>
#include"utility.h"

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
	 *	a = 1, b = 2, c = 1
	 *	x = {0.5, 0.0, 0.5}
	 * */

	double a{1.0}, b{2.0}, c{1.0};
	std::vector<double> x = calc_Tomas(a, b, c, std::vector<double>{ 1.0, 1.0, 1.0 });
	assert( (x == std::vector{0.5, 0.0, 0.5}) );
	prn1D("x: ", x);

	std::cout<<"End\n";
	return EXIT_SUCCESS;
}
