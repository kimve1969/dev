#pragma once
#include"utility.h"
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
namespace
{
	template<typename T>
		vec1D_t<T> calc_Tomas(const vec1D_t<T> &&a, const vec1D_t<T> &&b, const vec1D_t<T> &&c, const vec1D_t<T> &&d)
		{
			int N = d.size();

			// forward run
			vec1D_t< T > p( N, 0.0), q( N, 0.0 );
			// see above p1 and q1 
			p[0] = -c[0]/b[0];
			q[0] = d[0]/b[0];
			// see above pi and qi
			for(int i=1; i < N; ++i)
			{
				p[i] = - c[i] / ( b[i] + a[i]*p[i-1] );
				q[i] = ( d[i] - a[i]*q[i-1] ) / ( b[i] + a[i]*p[i-1] );
			}
			// backward run
			vec1D_t< T > x( N, 0.0); 
			x[N-1] = q[N-1];
			for(int i = N-2 ; i >= 0 ; --i)
			{
				x[i] = p[i] * x[i+1] + q[i];
			}

			return x;
		};
}

