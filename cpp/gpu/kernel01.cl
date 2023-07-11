//#pragma REDUCTION_LWS 16			// set size of local group is 128
#pragma OPENCL EXTENSION cl_khr_fp64 : enable	// support double
// x = ax + by

__kernel void knlAXPBY(int n, __global float *x, __global float *y, float a, float b){
	 const int gid = get_global_id(0);
	 const int lid = get_local_id(0);
	 const int group_id = get_group_id(0);
	 printf("gid = %d, group id = %d, lid =%d, ", gid, group_id, lid); 
	 if( gid >=n ) return;
	 printf("a*x[%d] + b*y[%d] = %5.2f * %5.2f + %5.2f * %5.2f = %5.2f\n",gid, gid, a, x[gid], b, y[gid], x[gid] = a*x[gid] + b*y[gid]);
}
 