#pragma once
#include<algorithm>
#include"tomas.h"
#include"utility"

namespace
{
	/*
	 *	Yanenko numeric method, 1967 year
	 *	
	 *	du/dt = alpha * (du2/d2x + du2/d2y) + f(x,y,t)
	 *	
	 *	step 1, from (tn) to (tn+1/2)
	 *	r = dt * alpha[xi][yj] / ( 2 * dx * dx )
	 *
	 *	r * v[tn+1/2][xi-1][yj] - (1 + r) * v[tn+1/2][xi][yj] + r * v[tn+1/2][xi+1][yj] = - u[tn][xi][yj] + 0.5 * f[tn][xi][yj],	
	 *	v[tn+1/2][][] - unknown, u[tn][][] - famous
	 *
	 *	step 2, from (tn+1/2) to (tn)
	 *	r = dt * alpha[xi][yj] / ( 2 * dy * dy )
	 *
	 *	r * w[tn+1][xi][yj-1] - (1 + r) * w[tn+1][xi][yj] r * w[n+1][xi][yj+1] = -v[tn+1/2][xi][yj] + 0.5 * f[tn][xi][yj],
	 *	w[tn][][] - unknown, v[tn+1/2][xi][yj] - famous
	 *
	 * */
	template<typename T> //, typename Bound_x0, typename Bound_xL, typename Bound_y0, typename Bound_yM, typename Source>
	vec2D_t< T > calc_Yanenko( const vec2D_t< T > &u, const vec2D_t< T > &alpha, T dx, T dy, T dt, bool prn = false )
	{
		if( prn ) std::cout << "-------------------------- Start Yanenko method --------------------- \n";
		const size_t Nx{ u.size() } /*x*/, Ny{ u[0].size() /*y*/ };
		vec2D_t v( Nx, Ny, 0.0d ); // v is 	u[tn+1/2]
		vec2D_t w( Nx, Ny, 0.0d ); // w is 	u[tn+1]
		
		prn2D("u:\n", u, prn);
		
		// Step 1
		for( size_t yj = 1; yj < Ny-1 /*y*/ ; ++yj )
		{
			vec1D_t a(Nx-2, 0.0d), b(Nx-2, 0.0d), c(Nx-2, 0.0d), d(Nx-2, 0.0d); 
			for( size_t xi = 1; xi < Nx-1 /*x*/; ++xi )
			{
				T r = dt * alpha[xi][yj] / (dx*dx);
				a[xi-1] = c[xi-1] = r / 2.0d;
				b[xi-1] = -(1.0d + r);
				d[xi-1] = -u[xi][yj] /* u[tn] */;// + 0.5 * source(xi, yj, -u[xi][yj], tn);
			}

			d[0] 	+= -a[0] * u[0][yj] /* x0 bound */; 
			d[Nx-3] += -a[Nx-3] * u[Nx-1][yj] /*xL bound*/;

			vec1D_t<double> res = calc_Tomas( std::move(a), std::move(b), std::move(c), std::move(d) );
		
			prn1D("d: ", d, prn );
			prn1D("x: ", res, prn );
			
			for( size_t xi = 1; xi < Nx-1; ++xi )
			{
				v[xi][yj] = res[xi-1];
			}
		}		

		prn2D("v:\n", v, prn);

		// Step 2
		for( size_t xi = 1; xi < Nx-1 /*x*/ ; ++xi )
		{
			vec1D_t a(Ny-2, 0.0d), b(Ny-2, 0.0d), c(Ny-2, 0.0d), d(Ny-2, 0.0d); 
			for( size_t yj = 1; yj < Ny-1 /*y*/; ++yj )
			{
				T r = dt * alpha[xi][yj] / (dy*dy);
				a[yj-1] = c[yj-1] = r / 2.0d;
				b[yj-1] = -(1.0d + r);
				d[yj-1] = -v[xi][yj]/* u[tn+1/2] */;// + 0.5 * source(xi, yj, -u[xi][yj], tn);
			}

			d[0] 	+= -a[0] * u[xi][0] /* y0 bound */; 
			d[Ny-3] += -a[Ny-3] * u[xi][Ny-1] /* yM bound */;

			vec1D_t<double> res = calc_Tomas( std::move(a), std::move(b), std::move(c), std::move(d) );
			
			prn1D("d: ", d, prn);
			prn1D("y: ", res, prn );

			std::copy( res.begin(), res.end(), w[xi].begin()+1);
		}

		prn2D("w:\n", w, prn );
		if( prn ) std::cout << "--------------------------- Stop Yanenko method --------------------- \n";

		return w;
	}
}

