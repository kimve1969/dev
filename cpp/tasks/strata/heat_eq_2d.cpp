#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include"utility.h"

/*
 * 	Homework: Strata Solution, 21.05.2024
 *
 *	Need solve heat eqution: du/dt = alpha * (du2/d2x + du2/d2y) + f(x,y,t)
 *		
 *		u = u(x,y,t) 
 *		du/dt - first order differential in time
 *		du2/d2x, du2/d2y - second order differential in coords x & y
 *		alpha = init_alpha(x,y) - coordinate-depend coefficient of thermal conductivity
 *		f(x,y,t) = source(t) - source, x - const, y - const, t > 0 
 *	
 *	Initial condition: 
 *		u(x,y,0) = init_xy(x,y), {0.0 < x < L}, {0.0 < y < M}, t = 0
 * 	Boundary conditions: 
 * 		u(0,y,t) = bound_x0(y,t), x = 0.0, 	t > 0
 * 		u(L,y,t) = bound_xL(y,t), x = L, 	t > 0
 * 		u(x,0,t) = bound_y0(x,t), y = 0.0, 	t > 0
 * 		u(x,M,t) = bound_yM(x,t), y = M, 	t > 0  
 * 	Indexs:
 * 		xi - index x-coord
 * 		yj - index y-coord
 * 		tn - index of time
 * 	Steps:
 *		dx - step x-coord
 *		dy - step y-coord
 *		dt - step time
 * */

namespace
{
	struct init_alpha
	{
		auto operator()(int xi, int yj)
		{
			return std::abs( std::sin(xi) + std::cos(yj) );
		}
	};

	struct init_xy
	{
		auto operator()(int xi, int yj)
		{
			return 0.0 /* const, maybe exchange */;
		}
	};

	struct bound_x0
	{
		auto operator()(int yj, int tn)
		{
			return 2.0 /* const, maybe exchange */; 
		}
	};

	struct bound_xL
	{
		auto operator()(int yj, int tn)
		{
			return 3.0 /* const, maybe exchange */;
		}
	};

	struct bound_y0
	{
		auto operator()(int xi, int tn)
		{
			return 4.0 /* const, maybe exchange */;
		}
	};

	struct bound_yM
	{
		auto operator()(int xi, int tn)
		{
			return 4.0 /* const, maybe exchange */;
		}
	};

	struct source
	{
		auto operator()(int tn)
		{
			return tn < 1000 ? 10.0 /* turn */ : 0.0 /* off */;
		}
	};


};

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";
	
	constexpr double Lx{ 1.0 }, Ly{ 1.0 }, T{ 100.0 };
	constexpr int Nx{ 10 }, Ny{ 10 }, Nt{ 100 };
	constexpr double dx{ Lx/Nx }, dy{ Ly/Ny }, dt{ T/Nt };

	// Step 1. Create 3-d grid of surface Nx * Ny * Ct, where 
	// Ct is 10 steps of time: 0, 1, 2, 3, 10, 0.1*Nt, 0.25*Nt, 0.5*Nt, 0.75*Nt, 1.0*Nt
	std::vector< std::vector< std::vector< double > > > u( 10 /* Ct */, std::vector( Nx + 1, std::vector( Ny + 1, 0.0 ) ) );
	
	// 1.1 Fill grid by coefficient of thermal conductivity - alpha[xi][yj]
	std::vector< std::vector< double > > alpha( Nx + 1, std::vector( Ny + 1, 0.0 ) );		
	for( int xi = 0; xi < alpha.size() ; ++xi )
		for( int yj = 0; yj < alpha[ xi ].size() ; ++yj )
			alpha[ xi ][ yj ] =  init_alpha{}( xi, yj );

	prn2D("alpha:\n", alpha);

	// 1.2 Set initial condition
	
	// Step 2. 
	
	std::cout<<"End\n";
	return EXIT_SUCCESS;
}
