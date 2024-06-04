#pragma once
#include<algorithm>
#include<initializer_list>
#include"utility.h"
#include"yanenko.h"

/*
 * */
namespace 
{
	template<typename T, typename Init_xy_t, typename Bound_x0_t, typename Bound_xL_t, typename Bound_y0_t, typename Bound_yM_t, typename Source_t>
		vec2D_t<T> calc_heat_eq_2d(const vec2D_t<T> &alpha, 
				Init_xy_t  init_xy,
				Bound_x0_t bound_x0,
				Bound_xL_t bound_xL,
				Bound_y0_t bound_y0,
				Bound_yM_t bound_yM,
				Source_t   source,
				double Lx,
				double Ly,
				double Time,
				size_t Nt = 100)
		{
			const size_t Nx{ alpha.size() }, Ny{ alpha[0].size() };
			const double dt{ Time/Nt }, dx{ Lx/Nx }, dy{ Ly/Ny };

			// Step 1.
			vec2D_t<double> u( Nx, Ny, 0.0 );
			for( size_t xi = 0; xi < Nx; ++xi)
				for( size_t yj = 0; yj < Ny; ++yj )
					u[ xi ][ yj ] = init_xy( xi, yj );
			
			// Step 2.
			for( size_t tn = 0; tn < Nt; ++tn )
			{
				// x-boundary u(0,y) & u(M,y)
				for( size_t yj = 1; yj < Ny-1; ++yj )
				{
					u[0][yj] 	= bound_x0(yj, tn);
					u[Nx-1][yj] 	= bound_xL(yj, tn);
				}

				// y-boundary u(x,0) & u(x,L) 
				for( size_t xi = 1; xi < Nx-1; ++xi )
				{
					u[xi][0]	= bound_y0(xi, tn);
					u[xi][Ny-1]	= bound_yM(xi, tn);
				}

				// sources
				for( size_t xi = 1; xi < Nx-1; ++xi )
					for( size_t yj = 1; yj < Ny-1; ++yj )
						u[xi][yj] += source(xi, yj, tn);

				auto find_in_list = []( std::initializer_list<size_t> l, size_t val)
				{
					return std::find( l.begin(), l.end(), val ) != l.end();
				};

				// you can enumarate tn for trace Yanenko method, example trace_points{0,1}
				static const std::initializer_list<size_t> trace_points{};
				bool btrace = find_in_list( trace_points, tn );

				// calc Yanennko method
				vec2D_t<double> u_tn = calc_Yanenko( u, alpha, dx, dy, dt, btrace );

				// you can enumarate tn for print u(tn), example check_points{0, 1, 2, Nt/10, Nt/4, Nt/2, Nt*3/4, Nt-1 };
				static const std::initializer_list<size_t> check_points{};
				bool bprn = find_in_list( check_points, tn );

				if(bprn) std::cout<< "\ntn: ----------------------- "<<tn<<"--------------------------\n";
				prn2D("u:\n", u, bprn);
				prn2D("u_tn:\n", u_tn, bprn);

				std::copy( u_tn.begin(), u_tn.end(), u.begin() );
			}

			return u;
		}
}
