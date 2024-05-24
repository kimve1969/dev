#include<iostream>
#include<cmath>
#include"utility.h"
#include"tomas.h"

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
	class base_xy_t
	{
		public:
			base_xy_t(double Nx, double Ny) : m_Nx(Nx), m_Ny(Ny){}
		private:
			double m_Nx, m_Ny;
	};

	struct init_alpha_t : public base_xy_t
	{
		init_alpha_t(double Nx, double Ny) : base_xy_t(Nx, Ny){}
		auto operator()(int xi, int yj)
		{
			return std::abs( std::sin(xi) + std::cos(yj) );
		}
	};

	struct init_xy_t : public base_xy_t
	{
		init_xy_t(double Nx, double Ny) : base_xy_t(Nx, Ny){}
		auto operator()(double xi, double yj)
		{
			return 1.0 /* const, maybe exchange */;
		}
	};

	struct bound_x0_t  : public base_xy_t
	{
		bound_x0_t(double Nx, double Ny) : base_xy_t(Nx, Ny){}
		auto operator()(double yj, double tn)
		{
			return 2.0 /* const, maybe exchange */; 
		}
	};

	struct bound_xL_t : public base_xy_t 
	{
		bound_xL_t(double Nx, double Ny) : base_xy_t(Nx, Ny){}
		auto operator()(int yj, int tn)
		{
			return 3.0 /* const, maybe exchange */;
		}
	};

	struct bound_y0_t : public base_xy_t
	{
		bound_y0_t(double Nx, double Ny) : base_xy_t(Nx, Ny){}
		auto operator()(int xi, int tn)
		{
			return 4.0 /* const, maybe exchange */;
		}
	};

	struct bound_yM_t : public base_xy_t
	{
		bound_yM_t(double Nx, double Ny) : base_xy_t(Nx, Ny){}
		auto operator()(int xi, int tn)
		{
			return 4.0 /* const, maybe exchange */;
		}
	};

	struct source_t : base_xy_t
	{
		source_t(double Nx, double Ny) : base_xy_t(Nx, Ny){}
		auto operator()(int tn)
		{
			return tn < 1000 ? 10.0 /* turn on */ : 0.0 /* turn off */;
		}
	};

	/*
	 *	Yanenko numeric method, 1967 year
	 *	
	 *	du/dt = alpha * (du2/d2x + du2/d2y) + f(x,y,t)
	 *	
	 *	step 1, from (tn) to (tn+1/2)
	 *	r = dt * alpha[xi][yj] / 2 * dx * dx
	 *
	 *	r * v[tn+1/2][xi-1][yj] - (1 + r) * v[tn+1/2][xi][yj] + r * v[tn+1/2][xi+1][yj] = - u[tn][xi][yj] + 0.5 * f[tn][xi][yj],	
	 *	v[tn+1/2][][] - unknown, u[tn][][] - famous
	 *
	 *	step 2, from (tn+1/2) to (tn)
	 *	r = dt * alpha[xi][yj] / 2 * dy * dy
	 *
	 *	r * w[tn+1][xi][yj-1] - (1 + r) * w[tn+1][xi][yj] r * w[n+1][xi][yj+1] = -v[tn+1/2][xi][yj] + 0.5 * f[tn][xi][yj],
	 *	w[tn][][] - unknown, v[tn+1/2][xi][yj] - famous:q
	 *
	 * */
	
	vec2D_t<double> calc_Yanenko( const vec2D_t<double> &u, 
				      const vec2D_t<double> &alpha, 
				      int dt,
				      int dx,
				      int dy,
				      bound_x0_t &&bound_x0,
				      bound_xL_t &&bound_xL,
				      bound_y0_t &&bound_y0,
				      bound_yM_t &&bound_yM,
				      source_t   &&source )
	{
		vec2D_t v( u.size() /*x*/, u[0].size() /*y*/, 0.0d ); // v is u[tn+1/2]
		vec2D_t w( u.size() /*x*/, u[0].size() /*y*/, 0.0d ); // w is u[tn+1]
				
		// Step 1
		

		// Step 2

		return w;
	}	
};

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";
	
	constexpr double Lx{ 1.0 }, Ly{ 1.0 }, T{ 100.0 };
	constexpr int Nx{ 10 }, Ny{ 10 }, Nt{ 100 };
	constexpr double dt{ T/Nt }, dx{ Lx/Nx }, dy{ Ly/Ny };
	
	// Step 1. Create 3-d grid of surface Nx * Ny * Ct, where 
	// Ct is 10 steps of time: 0, 1, 2, 3, 10, 0.1*Nt, 0.25*Nt, 0.5*Nt, 0.75*Nt, 1.0*Nt
	vec2D_t<double> u( Nx + 1, Ny + 1, 0.0 );
	init_xy_t init_xy(Nx, Ny);
	for( int xi = 0; xi < u.size(); ++xi)
		for( int yj = 0; yj < u[xi].size(); ++yj )
			u[ xi ][ yj ] = init_xy( xi, yj );

	prn2D("u[x][y]:\n", u);

	// 1.1 Fill grid by coefficient of thermal conductivity - alpha[xi][yj]
	vec2D_t<double> alpha( Nx + 1, Ny + 1);		
	init_alpha_t init_alpha(Nx, Ny);
	for( int xi = 0; xi < alpha.size() ; ++xi )
		for( int yj = 0; yj < alpha[ xi ].size() ; ++yj )
			alpha[ xi ][ yj ] =  init_alpha( xi, yj );

	prn2D("alpha:\n", alpha);

	// Step 2. 
	vec2D_t<double> un_1 = calc_Yanenko( u, alpha, dt, dx, dy,
					     bound_x0_t {Nx,Ny}, 
					     bound_xL_t {Nx,Ny}, 
					     bound_y0_t {Nx,Ny}, 
					     bound_yM_t {Nx,Ny}, 
					     source_t {Nx,Ny} );

	prn2D("un_1:\n", un_1);	

	std::cout<<"End\n";
	return EXIT_SUCCESS;
}
