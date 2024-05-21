#include<iostream>
#include<vector>
#include<algorithm>
/*
 *
 *  Explicit method solve 1-D heat equataion:
 *
 * 	( u[n+1][i] - u[n][i] ) / dt = alpha * ( u[n][i-1] - 2*u[n][i] + u[n][i+1] ) / (dx * dx)
 * 	n - step of time, i - step of x
 *
 * */

int main( int argc, char* argv[])
{
	std::cout<<"Start\n";

	constexpr double L = 1.0, T = 40.0, alpha = 0.005;
	constexpr int Nx = 100, Nt = 10000;
	
	// u[t,x]
	std::vector< std::vector<double> > u( Nt+1, std::vector<double>( Nx+1 ) );
	// set initial contidion: u[0,x]
	std::for_each( u[0].begin(), u[0].end(), [](auto &e){ e = 0.0; } );
	// set boundary condition: u[t,0], u[t, Nx]
	std::for_each( u.begin(), u.end(), []( auto &e ){ e[0] = 20.0; e[Nx] = 20.0; } );
	
	constexpr double dt = T/Nt, dx = L/Nx, r = (alpha * dt) / (dx * dx); 	
	// stability condition for explicit method for 1-d is r <= 0.5									    
	// if r > 0.5, example alpha = 0.015, then r = 0.6 and calc is not stable !
	if( r > 0.5 )
	{
		std::cout<<"Stop calc (r > 0.5): r = "<<r<<"\n";
		return EXIT_FAILURE;
	}

	// calc
	std::cout<<"dt = "<<dt<<", dx = "<<dx<<", r = "<<r<<"\n";
	for( int n = 0; n < Nt ; ++n ) // time layers
		for( int i = 1; i < Nx ; ++i ) // x-coords
			u[n+1][i] = u[n][i] + r * ( u[n][i-1] - 2*u[n][i] + u[n][i+1] );
	// print
	for( auto n: {0, 1, Nt/10, Nt/4, Nt/2, (Nt*3)/4, Nt} )
	{
		std::cout<<"\n\nTime step is "<<n<<" :\n";
		for( int i = 0; i <= Nx; ++i ) std::cout<<"{ "<<i<<", "<<u[n][i]<<" }, ";
	}

	std::cout<<"\nEnd\n";
	return EXIT_SUCCESS;
}
