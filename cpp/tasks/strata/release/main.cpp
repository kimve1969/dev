#include<iostream>
#include"utility.h"
#include"heat_eq_2d.h"

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
			return 0.005; //std::abs( std::sin(xi) + std::cos(yj) );
		}
	};

	struct init_xy_t : public base_xy_t
	{
		auto operator()(size_t xi, size_t yj)
		{
			return 0.0 /* const, maybe exchange */;
		}
	};

	struct bound_x0_t  : public base_xyt_t
	{
		auto operator()(size_t yj, size_t tn)
		{
			return 20.0 /* const, maybe exchange */; 
		}
	};

	struct bound_xL_t : public base_xyt_t 
	{
		auto operator()(size_t yj, size_t tn)
		{
			return 20.0; //5.0 /* const, maybe exchange */;
		}
	};

	struct bound_y0_t : public base_xyt_t
	{
		auto operator()(size_t xi, size_t tn)
		{
			return 20.0; //20.0 - (15.0 / num_x_nodes ) * xi /* const, maybe exchange */;
		}
	};

	struct bound_yM_t : public base_xyt_t
	{
		auto operator()(size_t xi, size_t tn)
		{
			return 20.0;//20.0 - (15.0 / num_x_nodes ) * xi /* const, maybe exchange */;
		}
	};

	struct source_t : base_xyt_t
	{
		auto operator()(size_t xi, size_t yj, size_t tn)
		{
			size_t cx = num_x_nodes / 2;
			size_t cy = num_y_nodes / 2;
			if( (cx-1 <= xi) && (xi <= cx+1 ) && (cy-1 <= yj) && (yj <= cy+1) )
			{
				return tn < num_t_nodes/2 ? -2.0 /* turn on */ : 0.0 /* turn off */;
			}
			else
			{
				return 0.0;
			}
		}
	};
};

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";
	
	constexpr double Lx{ 1.0 }, Ly{ 1.0 }, T{ 50.0 };
	constexpr size_t Nx{ 10 }, Ny{ 10 }, Nt{ 100 };
	constexpr double dt{ T/Nt }, dx{ Lx/Nx }, dy{ Ly/Ny };

	std::cout << "------------------ DIRECT TASK ---------------------\n";

	// Fill grid by coefficient of thermal conductivity - alpha[xi][yj]
	vec2D_t<double> alpha( Nx+1, Ny+1);		
	init_alpha_t init_alpha{Nx+1, Ny+1};
	for( size_t xi = 0; xi < Nx+1 ; ++xi )
		for( size_t yj = 0; yj < Ny+1 ; ++yj )
			alpha[ xi ][ yj ] =  init_alpha( xi, yj );

	prn2D("2D-coefficient of thermal conductivity:\n", alpha);

	init_xy_t  init_xy{Nx+1, Ny+1};	
	bound_x0_t bound_x0{Nx+1, Ny+1, Nt+1};
	bound_xL_t bound_xL{Nx+1, Ny+1, Nt+1};
	bound_y0_t bound_y0{Nx+1, Ny+1, Nt+1};
	bound_yM_t bound_yM{Nx+1, Ny+1, Nt+1};
	source_t   source{Nx+1, Ny+1, Nt+1};

	auto u = calc_heat_eq_2d(alpha, init_xy, bound_x0, bound_xL, bound_y0, bound_yM, source, Lx, Ly, T, Nt);

	prn2D("2D-temperature:\n", u);

	std::cout << "---------------------- BACK TASK ----------------------\n";
	// a priori known coefficient of termal conductivity  
	constexpr double cnst_alpha_min{0.00001}, cnst_alpha_max{0.1};
	std::cout<< "alpha min = "<<cnst_alpha_min<<", alpha max = "<<cnst_alpha_max<<"\n";
	constexpr double sigma{ 0.00001 };
	constexpr size_t cnst_max_steps{30};

	// initial alpha tuning
	vec2D_t<double> alpha_tuning( Nx+1, Ny+1, (cnst_alpha_max + cnst_alpha_min)*0.5 );
	prn2D("alpha tuning initial\n", alpha_tuning);

	//std::cout<< "sigma * Nx * Ny = "<< sigma * Nx * Ny <<"\n";

	// tuning at inner area alpha [1;N-1]
	vec2D_t alpha_min(Nx+1, Ny+1, cnst_alpha_min);
	vec2D_t alpha_max(Nx+1, Ny+1, cnst_alpha_max);
	vec2D_t alpha_optim(Nx+1, Ny+1, (cnst_alpha_max + cnst_alpha_min)*0.5);
	vec2D_t lsm(Nx+1, Ny+1, -1.0);

	for (size_t n_step = 0; n_step < cnst_max_steps; ++n_step)
	{
		for( size_t xi = 1; xi < Nx; ++xi )
			for( size_t yj = 1; yj < Ny; ++yj )
			{
				// check out
				if( lsm[xi][yj] > 0.0 && ( lsm[xi][yj] < sigma * Nx * Ny) ) continue;

				// calculate at left
				alpha_tuning[xi][yj] -= sigma; 
				auto w1 = calc_heat_eq_2d(alpha_tuning, init_xy, bound_x0, bound_xL, bound_y0, bound_yM, source, Lx, Ly, T, Nt);
				auto lsm1 = calc_lsm( u, w1 );

				// calculate at right
				alpha_tuning[xi][yj] += sigma + sigma; 
				auto w2 = calc_heat_eq_2d(alpha_tuning, init_xy, bound_x0, bound_xL, bound_y0, bound_yM, source, Lx, Ly, T, Nt);
				auto lsm2 = calc_lsm( u, w2 );

				// change boundaries
				alpha_min[xi][yj]   = (lsm1 < lsm2 ? alpha_min[xi][yj] : alpha_optim[xi][yj] - sigma);
				alpha_max[xi][yj]   = (lsm1 < lsm2 ? alpha_optim[xi][yj] + sigma : alpha_max[xi][yj]);
				alpha_optim[xi][yj] = (alpha_min[xi][yj] + alpha_max[xi][yj])*0.5;

				alpha_tuning[xi][yj] = alpha_optim[xi][yj];

				lsm[xi][yj] = (lsm1 < lsm2 ? lsm1 : lsm2); 
				//std::cout << lsm[xi][yj] <<", ";
			}
	}

	prn2D("\nalpha tuning\n", alpha_tuning);

	std::cout<<"\nEnd\n";
	return EXIT_SUCCESS;
}
