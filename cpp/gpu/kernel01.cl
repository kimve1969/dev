#pragma REDUCTION_LWS 128			// set size of local group is 128
#pragma OPENCL EXTENSION cl_khr_fp64 : enable	// support double
// x = ax + by

__kernel void knlAXPBY(int n, __global double *x, __global double *y, double a, double b){
	 const int gid = get_global_id(0);
	 printf("gid = %d\n", gid);
	 if( gid >=n ) return;
	 //x[gid] = a*x[gid] + b*y[gid];
	 x[gid]=x[gid] + y[gid];
}
 