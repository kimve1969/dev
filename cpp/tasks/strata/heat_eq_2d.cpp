#include<iostream>
#include<algorithm>
#include<initializer_list>
#include<cmath>
#include"utility.h"
#include"tomas.h"
#include"yanenko.h"

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
	struct base_xy_t
	{
		/*public:
			base_xy_t(double Nx, double Ny) : m_Nx(Nx), m_Ny(Ny){}
		private:*/
		size_t num_x_nodes, num_y_nodes;
	};

	struct base_xyt_t
	{
		size_t num_x_nodes, num_y_nodes, num_t_nodes;
	};

	struct init_alpha_t : public base_xy_t
	{
		//init_alpha_t(double Nx, double Ny) : base_xy_t(Nx, Ny){}
		auto operator()(size_t xi, size_t yj)
		{
			return 0.005;//std::abs( std::sin(xi) + std::cos(yj) );
		}
	};

	struct init_xy_t : public base_xy_t
	{
		auto operator()(size_t xi, size_t yj)
		{
			return 0.0d /* const, maybe exchange */;
		}
	};

	struct bound_x0_t  : public base_xyt_t
	{
		auto operator()(size_t yj, size_t tn)
		{
			return 20.0d /* const, maybe exchange */; 
		}
	};

	struct bound_xL_t : public base_xyt_t 
	{
		auto operator()(size_t yj, size_t tn)
		{
			return 20.0d; //5.0d /* const, maybe exchange */;
		}
	};

	struct bound_y0_t : public base_xyt_t
	{
		auto operator()(size_t xi, size_t tn)
		{
			return 20.0d; //20.0d - (15.0d / num_x_nodes ) * xi /* const, maybe exchange */;
		}
	};

	struct bound_yM_t : public base_xyt_t
	{
		auto operator()(size_t xi, size_t tn)
		{
			return 20.0;//20.0d - (15.0d / num_x_nodes ) * xi /* const, maybe exchange */;
		}
	};

	struct source_t : base_xyt_t
	{
		auto operator()(size_t xi, size_t yj, double old_value, size_t tn)
		{
			if( (num_x_nodes / 2) == xi && (num_y_nodes / 2) == yj )
			{
				return tn < 100 ? 10.0 /* turn on */ : old_value /* turn off */;
			}
			else
			{
				return old_value;
			}
		}
	};
};

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";
	
	constexpr double Lx{ 1.0 }, Ly{ 1.0 }, T{ 100.0 };
	constexpr int Nx{ 10 }, Ny{ 10 }, Nt{ 100 };
	constexpr double dt{ T/Nt }, dx{ Lx/Nx }, dy{ Ly/Ny };
	
	// 1.1 Fill grid by coefficient of thermal conductivity - alpha[xi][yj]
	vec2D_t<double> alpha( Nx + 1, Ny + 1);		
	init_alpha_t init_alpha{Nx, Ny};
	for( size_t xi = 0; xi < alpha.size() ; ++xi )
		for( size_t yj = 0; yj < alpha[ xi ].size() ; ++yj )
			alpha[ xi ][ yj ] =  init_alpha( xi, yj );

	prn2D("alpha:\n", alpha);
	
	// Step 1. Create 3-d grid of surface Nx * Ny * Ct, where 
	// Ct is 10 steps of time: 0, 1, 2, 3, 10, 0.1*Nt, 0.25*Nt, 0.5*Nt, 0.75*Nt, 1.0*Nt
	vec2D_t<double> u( Nx + 1, Ny + 1, 0.0 );
	init_xy_t init_xy{Nx, Ny};
	for( size_t xi = 0; xi < u.size(); ++xi)
		for( size_t yj = 0; yj < u[xi].size(); ++yj )
			u[ xi ][ yj ] = init_xy( xi, yj );
	
	// Step 2.
	bound_x0_t bound_x0{Nx+1, Ny+1, Nt+1};
	bound_xL_t bound_xL{Nx+1, Ny+1, Nt+1};
	bound_y0_t bound_y0{Nx+1, Ny+1, Nt+1};
	bound_yM_t bound_yM{Nx+1, Ny+1, Nt+1};
	source_t source{Nx+1, Ny+1, Nt+1};

	for( size_t tn = 0; tn < Nt; ++tn )
	{
		// u(0,y) & u(M,y)
		for( size_t yj = 1; yj < Ny; ++yj )
		{
			u[0][yj] 	= bound_x0(yj, tn);
			u[Nx][yj] 	= bound_xL(yj, tn);
		}
		
		// u(x,0) & u(x,L) 
		for( size_t xi = 1; xi < Nx; ++xi )
		{
			u[xi][0]	= bound_y0(xi, tn);
			u[xi][Nx]	= bound_yM(xi, tn);
		}

		/*for( size_t xi = 0; xi < Nx+1; ++xi )
		  for( size_t yj = 0; yj < Ny+1; ++yj )
		  u[xi][yj] = source(xi, yj, u[xi][yj], tn);*/

		auto find_in_list = []( std::initializer_list<size_t> l, size_t val)
		{
			return std::find( l.begin(), l.end(), val ) != l.end();
		};

		// calc Yanenko
		static const std::initializer_list<size_t> trace_points{}; // {0,1}
		bool btrace = find_in_list( trace_points, tn );
		
		vec2D_t<double> u_tn = calc_Yanenko( u, alpha, dx, dy, dt, btrace );
	
		static const std::initializer_list<size_t> check_points{0, 1, 2, Nt/10, Nt/4, Nt/2, Nt*3/4, Nt-1 };
		bool bprn = find_in_list( check_points, tn );
		
		if(bprn) std::cout<< "\ntn: ----------------------- "<<tn<<"--------------------------\n";
		prn2D("u:\n", u, bprn);
		prn2D("u_tn:\n", u_tn, bprn);
	
		std::copy( u_tn.begin(), u_tn.end(), u.begin() );
	}

	std::cout<<"End\n";
	return EXIT_SUCCESS;
}
