#include<iostream>
#include<vector>
#include<algorithm>
#include"utility.h"

/*
 *
 *  Implicit method solve 1-D heat equataion: 
 *
 * 	( u[n+1][i] - u[n][i] ) / dt = alpha * ( u[n+1][i-1] - 2*u[n+1][i] + u[n+1][i+1] ) / (dx * dx)
 * 	n [0 - Nt] - step of time, i [0 - Nx] - step of x
 *
 * 	transfer rigth side equatation to left side:
 *
 *	-r * u[n+1][i-1] + (1+2*r) * u[n+1][i] - r * u[n+1][i-1] = u[n][i]
 *	r = alpha * dt / (dx*dx) 
 *	u[n][i] - known, u[n+1][...] - unknown 
 *
 * 	Ax = b, 
 *	Au(n+1) = u(n):
 *
 *	(1+2r)	-r						u[n+1][1]		u[n][1] + u[n][0]
 *	-r	(1+2r)	-r					u[n+1][2]		u[n][2]
 *		-r	(1+2r)	-r
 *			...				  X	u[n+1][i]	= 	u[n][i]
 *
 *					-r	(1+2r)		u[n+1][Nx-1]		u[n][Nx-1] + u[n][Nx]
 *	
 *	A is 3-diagonal matrix (Nx-1) * (Nx-1):
 *	u(n+1) is vector (from 1 to Nx-1) u at time (n+1)*dt, 
 *	u(n) is vector (from 1 to Nx-1) u at time n*dt 
 *
 **/	

int main( int argc, char* argv[])
{
	std::cout<<"Start\n";
	
	constexpr double L = 1.0, T = 100.0, alpha = 0.005;
	constexpr int Nx = 100, Nt = 100;
	
	// u[t,x]
	std::vector< std::vector<double> > u( Nt+1, std::vector<double>( Nx+1 ) );
	// set initial contidion: u[0,x]
	std::for_each( u[0].begin(), u[0].end(), [](auto &e){ e = 0.0; } );
	// set boundary condition: u[t,0], u[t, Nx]
	std::for_each( u.begin(), u.end(), []( auto &e ){ e[0] = 20.0; e[Nx] = 20.0; } );
	
	constexpr double dt = T/Nt, dx = L/Nx, r = (alpha * dt) / (dx * dx); 
	std::cout<<"dt = "<<dt<<", dx = "<<dx<<", r = "<<r<<"\n";	
		
	// calc
	constexpr double a{-r}, b{1+2*r}, c{-r} /* see above */;
	std::cout<<"a = "<<a<<", b = "<<b<<", c = "<<c<<"\n";
	for(int n = 0; n < Nt ; ++n)
	{
		std::vector< double > d( Nx-1 );
		// copy current time layer
		std::copy_n( u[n].begin()+1, Nx-1, d.begin() );
		d[0] 	+= r * u[n][0]; 
		d[Nx-2] += r * u[n][Nx];
		// calc next time layer
		std::vector< double > x /* u(n+1) */ = calc_Tomas(a, b, c, std::move( d /* u(n) */));
		// copy next time layer
		std::copy_n( x.begin(), Nx-1, u[n+1].begin()+1 );
	}

	// print
	for( auto n: {0, 1, 2, 3, Nt/10, Nt/4, Nt/2, (Nt*3)/4, Nt} )
	{
		std::cout<<"\n\nTime step is "<<n<<" :\n";
		for( int i = 0; i <= Nx; ++i ) std::cout<<"{ "<<i<<", "<<u[n][i]<<" }, ";
	}

	std::cout<<"\nEnd\n";
	return EXIT_SUCCESS;
}
