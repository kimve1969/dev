2:
	mpic++ openmpi02.cpp -o openmpi02 -lstdc++ -std=c++17
	mpiexec -n 3 ./openmpi02
	rm openmpi02
1:
	mpic++ openmpi01.cpp -o openmpi01 -lstdc++ -std=c++17
	#g++ openmpi01.cpp -o openmpi01 -lstdc++ -std=c++17 `pkg-config mpi --cflags --libs-only-l`
	mpiexec -n 3 ./openmpi01
	rm openmpi01

