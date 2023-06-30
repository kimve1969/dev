#include<iostream>
#include<string>
#include<vector>
#define CL_TARGET_OPENCL_VERSION 300
#include<CL/cl.h>
#define CL_HPP_TARGET_OPENCL_VERSION 300
#include<CL/opencl.hpp>
#include<cstdio>
#include<cmath>
#include<cstdlib>

void CL_ASSERT(const char* pstr_mess, cl_int clErr){
  if( clErr != CL_SUCCESS ){
      std::cout<<pstr_mess<<clErr<<"\n";
      exit(1);
  }
}

void CL_ERROR(const char* pstr_mess){
  std::cout<<pstr_mess<<"\n";
  exit(1);
}

void CL_ERROR(const char* pstr_mess1, const char *pstr_mess2){
  std::cout<<pstr_mess1<<pstr_mess2<<"\n";
  exit(1);
}

int main(int argc, char* argv[]){
  std::cout<<"Start\n\n";
  
  const cl_int devID = 0; //
  std::string platformName = "Portable Computing Language"; //"Intel Gen OCL Driver"; // "NVIDIA CUDA"  "Intel(R) OpenCL"  "AMD"

  cl_context clContext;
  cl_command_queue clQueue;
  cl_program clProgram;
  cl_int clErr;

  {// OpenCL initialization
    std::cout<<"OpenCL initialization\n";

    // PLATFORM
    // How many platforms?
    cl_uint platformCount = 0;
    clErr = clGetPlatformIDs(0, 0, &platformCount);
    CL_ASSERT("clGetPlatformIDs error: ", clErr);

    if(platformCount <= 0) CL_ERROR("No platform found");

    std::cout<<"clGetPlatformIDs: "<<platformCount<<" platforms\n";

    // Get list of platforms
    cl_platform_id *platformList = new cl_platform_id[platformCount];
    clErr = clGetPlatformIDs(platformCount, platformList, 0);
    CL_ASSERT("clGetPlatformIDs", clErr);

    // Seek my platform
    const int STR_SIZE = 1024;
    char nameBuf[STR_SIZE];
    cl_int platform_id = -1;
    for(cl_int i=0; i<platformCount ; ++i){
      clErr = clGetPlatformInfo( platformList[i], CL_PLATFORM_NAME, STR_SIZE, nameBuf, 0);
      CL_ASSERT("clGetPlatformInfo error: ", clErr);
      std::cout<<"\tPlatform "<<i<<" : "<<nameBuf<<"\n";

      if( std::string(nameBuf) == platformName ) platform_id = i; // found
    }

    if( platform_id <0 ) CL_ERROR("Can't find platform");
    std::cout<<"Platform "<<platform_id<<" ("<<platformName<<") selected\n";

    // DEVICE
    // How many device platform have?
    cl_uint deviceCount = 0;
    clErr = clGetDeviceIDs( platformList[platform_id], CL_DEVICE_TYPE_ALL, 0, NULL, &deviceCount);
    CL_ASSERT("clGetDeviceIDs error: ", clErr);
    std::cout<<deviceCount<<" device found\n";

    if( devID >= deviceCount ) CL_ERROR("Wrong device selected: ");

    // Get list of devices
    cl_device_id *deviceList = new cl_device_id[deviceCount];
    clErr = clGetDeviceIDs( platformList[platform_id], CL_DEVICE_TYPE_ALL, deviceCount, deviceList, NULL);
    CL_ASSERT("clGetDeviceIDs error: ", clErr);

    delete[] platformList; // don't need more...

    for(int i=0; i<deviceCount ; ++i){
      clErr = clGetDeviceInfo( deviceList[i], CL_DEVICE_NAME, STR_SIZE, nameBuf, 0);
      CL_ASSERT("clGetDeviceInfo error: ", clErr);
      std::cout<<"\tDevice "<<i<<": "<<nameBuf<<"\n";
    }

    // CONTEXT
    clContext = clCreateContext(NULL, 1, &deviceList[devID], 0, 0, &clErr);
    CL_ASSERT("clCreateContext error: ", clErr);

    // COMMAND QUEUE
    clQueue = clCreateCommandQueue(clContext, deviceList[devID], 0, &clErr);
    CL_ASSERT("clCreateCommandQueue error: ", clErr);

    // PROGRAM
    const char *cKernelFile = "kernel01.cl";
    std::cout<<"Loading program from "<<cKernelFile<<" file\n";

    // user defines
    const char *cDefines = "";

    char *cSourceCL = NULL;
    {
      std::FILE *fd = std::fopen(cKernelFile, "rb");
      if( !fd ) CL_ERROR( "Can't open kernel file ", cKernelFile );

      std::fseek(fd, 0, SEEK_END); // get size of file
      size_t fileSize = std::ftell(fd);
      std::rewind(fd);

      cSourceCL = new char[fileSize + 1/* zero-terminated */];
      size_t nd = std::fread( cSourceCL, 1, fileSize, fd);
      if( nd != fileSize ) CL_ERROR("Failed to read kernel-program from file ",  cKernelFile);
      cSourceCL[ fileSize ] = '\0';
      std::cout<<"Text program = "<<cSourceCL<<"\n";
    }
    if( cSourceCL == NULL ) CL_ERROR("Can't get kernel-program from file ", cKernelFile);

    // load kernel to OpenCL
    size_t szKernelLength = strlen( cSourceCL );
    clProgram = clCreateProgramWithSource( clContext, 1, (const char **) &cSourceCL, &szKernelLength, &clErr);
    CL_ASSERT("clCreatProgramWithSource error: ", clErr);

    // compile kernel-program
    std::cout<<"clBuildProgram...";
    clErr = clBuildProgram(clProgram, 0, NULL, "-cl-mad-enable", NULL, NULL);
    CL_ASSERT("clBuildProgram error: ", clErr);
    std::cout<<"done\n";

    // get log after compile
    int LOG_S = 0;
    clErr = clGetProgramBuildInfo( clProgram, deviceList[devID], CL_PROGRAM_BUILD_LOG, 0, NULL, (size_t*) &LOG_S );
    CL_ASSERT("clGetProgramBuildInfo error: ", clErr);

    std::cout<<"LOG_S = "<<LOG_S<<"\n";
    if( LOG_S>8 ){
      char *programLog = new char[LOG_S];
      clErr = clGetProgramBuildInfo( clProgram, deviceList[devID], CL_PROGRAM_BUILD_LOG, LOG_S, programLog, 0 );
      CL_ASSERT("clGetProgramBuildInfo error: ", clErr);
      std::cout<<programLog<<"\n";
      delete[] programLog;
    }
    CL_ASSERT( "Compilation failed with error: ", clErr );
    delete[] cSourceCL;
  }

  // KERNELS
  std::cout<<"Create kernels knlAXPBY ...";
  cl_kernel knlAXPBY; // x=ax+y
  knlAXPBY = clCreateKernel( clProgram, "knlAXPBY", &clErr );
  CL_ASSERT("clCreateKernel", clErr);
  std::cout<<"done\n";

  const int N=32; //1'000'123; // size of test vectors

  // create BUFFERS on GPU
  std::cout<<"Creating openCL buffers for X and Y...";
  cl_mem clX = clCreateBuffer( clContext, CL_MEM_READ_WRITE, N*sizeof(double), NULL, &clErr );
  CL_ASSERT("clCreateBuffer", clErr);
  cl_mem clY = clCreateBuffer( clContext, CL_MEM_READ_WRITE, N*sizeof(double), NULL, &clErr );
  CL_ASSERT("clCreateBuffer", clErr);
  std::cout<<"done\n";
  std::cout<<"size of clX = "<<sizeof(clX)<<"\n";
  // TEST EXECUTION
  // data on CPU side
  double *X = new double[N];
  double *Y = new double[N];
  const double a = 1.234, b = 3.456;

  // fill vectors
  for(int i=0; i<N; ++i){
    X[i] = (i*1.23)*(i*4.56);
    Y[i] = (i*2.48)*(i*1.34);
  }

  // copy vectors to device
  std::cout<<"Copy vectors X & Y to device...";
  clErr = clEnqueueWriteBuffer( clQueue, clX, CL_TRUE, 0, N*sizeof(double), X, 0, NULL, NULL );
  CL_ASSERT("clEnqueueWriteBuffer", clErr);
  clErr = clEnqueueWriteBuffer( clQueue, clY, CL_TRUE, 0, N*sizeof(double), Y, 0, NULL, NULL );
  CL_ASSERT("clEnqueueWriteBuffer", clErr);
  std::cout<<"done\n";

  // set run parameters
  size_t lws = 32; // size of group
  size_t gws = N; // number of tasks
  if(gws % lws > 0) gws += -(gws % lws) + lws; // padding on rigth band
  
  // set arguments for kernels
  std::cout<<"Set arguments for kernels...";
  clSetKernelArg( knlAXPBY, 0, sizeof(int), (void*) &N );
  clSetKernelArg( knlAXPBY, 1, sizeof(cl_mem), (void*) &clX );
  clSetKernelArg( knlAXPBY, 2, sizeof(cl_mem), (void*) &clY );
  clSetKernelArg( knlAXPBY, 3, sizeof(double), (void*) &a );
  clSetKernelArg( knlAXPBY, 4, sizeof(double), (void*) &b );
  std::cout<<"done\n";
  
  // send to run
  std::cout<<"Send kernels to run...";
  clErr = clEnqueueNDRangeKernel( clQueue, knlAXPBY, 1, NULL, &gws, &lws, 0, NULL, NULL );
  CL_ASSERT("clEnqueueNDRangeKernel", clErr);
  std::cout<<"done\n"<<std::endl;
  clFinish(clQueue); //wait finish
  std::cout<<"done\n";

  // do same work on CPU
  for(int i=0; i<N; ++i){
    X[i] = a*X[i] + b*Y[i];
  }

  // get result from device
  double *R = new double[N];
  clErr = clEnqueueReadBuffer( clQueue, clX, CL_TRUE, 0, N*sizeof(double), R, 0, NULL, NULL );
  CL_ASSERT("clEnqueueReadBuffer", clErr);

  // compare result CPU & GPU
  double sum = 0;
  for(int i=0; i<N; ++i){
    sum += std::fabs( R[i] - X[i] );
  }
  std::cout<<"Test execution done\n Error = "<<sum/N<<"\n";

  delete [] R;
  delete [] X;
  delete [] Y;

  clErr = clReleaseMemObject(clX);
  CL_ASSERT("clReleaseMemObject", clErr);
  clErr = clReleaseMemObject(clY);
  CL_ASSERT("clReleaseMemObject", clErr);

  std::cout<<"\n\nEnd\n";
  
  return 0;
}
