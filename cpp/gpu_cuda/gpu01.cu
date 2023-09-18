/************************************************************************
Copyright:      "Surgutneftegas" PJSC
Autors:         Created by Kim V.E. 2023/08/23
Target:         For education
Annotation:     Initialization device, copy data from host to device, run kernel

*************************************************************************/
#include<iostream>
#include<omp.h>

__global__ void VecAdd(int* pA, int* pB, int *pC){
	int i = threadIdx.x;
	printf("%d, ",i); // print() add time computation!
	pC[i] = pA[i] + pB[i];
}

/*
   	Ким В.Е.
	Аннотация:
	1. Первый вызов CUDA вызывает инициализацию.
	2. При задании максимального кол-ва потоков в блоке больше чем 1024 (максимально возможное), компилятор не выдает ошибки. Результат неопределенный.

	Дополнительные выводы:
	1. Код по семантике похож на Open CL, но намного проще (не требуется инициализация платформ и и девайсов)
 */
__host__ int main(int arc, char* argv[]){
	std::cout<<"Start"<<std::endl;
	double t0{0}, t1{0}, t2{0}, t3{0};
	const int N = 100; // max threads is 1024;

	int* h_A = new int[N];
	int* h_B = new int[N];
	int* h_C = new int[N];

	for(int i=0; i<N; ++i){
		h_A[i] = i*2;
		h_B[i] = i*3;
	}

	auto prn = [&](int *p, char ch='M'){
		for(int i=0; i<N; ++i){
			std::cout<<ch<<"["<<i<<"] = "<<p[i]<<", ";
		}
		std::cout<<std::endl;
	};

	std::cout<<"\nA:\n";
	prn(h_A, 'A');

	std::cout<<"\nB:\n";
	prn(h_B, 'B');

	// Cuda initialization
	int* d_dummy;
	t0 = omp_get_wtime();
	cudaMalloc((void**)&d_dummy, sizeof(int));

	// Allocate A,B,C vectors in device memory
	size_t size = N * sizeof(int);

	t1 = omp_get_wtime();

	int* d_A;
	cudaMalloc((void**)&d_A, size);
	int* d_B;
	cudaMalloc(&d_B, size);
	int* d_C;
	cudaMalloc(&d_C, size);

	// Copy A,B vectors from host to device
	cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
	cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

	t2 = omp_get_wtime();
	// Call kernel
	std::cout<<"\ncomputation on GPU...\n";
	VecAdd<<<1, N>>>(d_A, d_B, d_C);

	cudaEvent_t syncEvent;
	cudaEventCreate(&syncEvent);
	cudaEventRecord(syncEvent, 0);
	cudaEventSynchronize(syncEvent);
	std::cout<<"\ndone\n";

	t3 = omp_get_wtime();
	// Copy C vector from device to host
	cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

	std::cout<<"\nC:\n";
	prn(h_C, 'C');

	// Free cuda resources
	cudaEventDestroy(syncEvent);
	cudaFree(d_A);
	cudaFree(d_B);
	cudaFree(d_C);

	// Free host memory
	delete [] h_A;
	delete [] h_B;
	delete [] h_C;

	std::cout<<"\nTime initialization CUDA: "<<(t1-t0)<<" sec"<<std::endl;
	std::cout<<"Timing are common: "<<(t3-t1)<<" sec, calculate: "<<(t3-t2)<<" sec ( "<<(t3-t2)*100.0/(t3-t1)<<" % ), copy: "<<(t2-t1)<<" sec ("<<(t2-t1)*100.0/(t3-t1)<<"%)"<<std::endl;
	std::cout<<"NOTE: output (using print function) in kernel add many time in computation!!!"<<std::endl;
	std::cout<<"End"<<std::endl;
	return 0;
}
