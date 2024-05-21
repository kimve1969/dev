#pragma once

#include<iostream>
#include<vector>

template<typename T>
void prn1D(const char* str, T& vec1D)
{
	std::cout<<str;
	for(int i=0; i<vec1D.size(); ++i)
	{
		std::cout<<"{ "<<i<<", "<<vec1D[i]<<" }, ";
	}
	std::cout<<"\n";
};

template<typename T>
void prn2D(const char* str, T& vec2D, bool bindx = false)
{
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

// Tomas method
/* 	A is:
 *	b1	c1	0	0 ...				x1		d1
 *	a2	b2	c2	0	0 ...			x2		d2
 *	0	a3	b3	c3	0	0 ...		x3		d3
 *	...						X		=
 *
 *				0	an	bn		xn		dn
 *
 *	forward:
 *	p1 = -c1/b1, q1 = d1/b1
 *	pi = - ci / ( bi + ai*p[i-1])
 *	qi = ( di - ai*q[i-1] ) / ( bi + ai*p[i-1] ), i = 2,3,...,n, n = Nx-1
 *	
 *	backward:
 *	xn = qn, n = Nx-1
 *	xi = pi*x[i+1] + qi, i = n-1, n-2, ..., 1 
 * */
template<typename T>
std::vector<T> calc_Tomas(T a, T b, T c, const std::vector<T> &&d)
{
	int N = d.size();

	// forward run
	std::vector< T > p( N, 0.0), q( N, 0.0 );
	// see above p1 and q1 
	p[0] = -c/b;
	q[0] = d[0]/b;
	// see above pi and qi
	for(int i=1; i < N; ++i)
	{
		p[i] = - c / ( b + a*p[i-1] );
		q[i] = ( d[i] - a*q[i-1] ) / ( b + a*p[i-1] );
	}
	// backward run
	std::vector< T > x( N, 0.0); 
	x[N-1] = q[N-1];
	for(int i = N-2 ; i >= 0 ; --i)
	{
		x[i] = p[i] * x[i+1] + q[i];
	}

	return x;
};

