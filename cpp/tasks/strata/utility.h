#pragma once
#include<iostream>
#include<vector>

namespace
{
	template<typename T>
		struct vec1D_t : public std::vector< T >
	{
		vec1D_t( int Nx ) : std::vector< T > (Nx) {}
		vec1D_t( int Nx, T val ) : std::vector< T > ( Nx, val ) {}
		vec1D_t( std::initializer_list< T > l ) : std::vector< T > (l) {}
	};

	template<typename T>
		struct vec2D_t : public std::vector< std::vector< T > >
	{
		vec2D_t( int Nx, int Ny ) : std::vector< std::vector< T > >(Nx, std::vector< T >( Ny )) {}
		vec2D_t( int Nx, int Ny, T val ) : std::vector< std::vector< T > >(Nx, std::vector< T >( Ny, val )) {}
	};

	template<typename T>
		void prn1D(const char* str, T& vec1D, bool prn = true)
		{
			if( not prn ) return;

			std::cout<<str;
			for(int i=0; i<vec1D.size(); ++i)
			{
				std::cout<<"{ "<<i<<", "<<vec1D[i]<<" }, ";
			}
			std::cout<<"\n";
		};

	/*
	 *	vec2D_t(Nx, Ny), Nx - col, Ny - row
	 *
	 *	print as:
	 *
	 * 	0
	 * 	---------------> y
	 *	|
	 *	|
	 *	|
	 *	|
	 *	v x
	 *	
	 * */
	template<typename T>
		void prn2D(const char* str, T& vec2D, bool prn = true, bool bindx = false)
		{
			if( not prn ) return;

			std::cout<<str;
			for(int i = 0; i < vec2D.size(); ++i)
			{
				for(int j = 0; j < vec2D[i].size(); ++j)
				{
					if( bindx ) std::cout << "{ [ "<<i<<", "<<j<<" ] = " << vec2D[i][j] << " }, ";
					else std::cout << vec2D[i][j] << ", ";
				}
				std::cout<<"\n";
			}
			std::cout<<"\n";
		};
}

