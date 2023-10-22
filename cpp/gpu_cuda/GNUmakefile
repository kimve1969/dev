.PHONY: blas05 cpu05 cpu06 cpu06-trans cpu06-trans-sse cpu06-trans-avx512f cpu06-trans-scalar
OPT=-O3
DIM=1000
NCORE=8
#skylake, broadwell, haswell, ivybridge, x86-64
ARCH=-march=x86-64 -fPIC
#-g
DEBUG=

cpu:
	clear
	rm -rf cpu-blas cpu-noblock cpu-block
	make cpu-blas
	make cpu-noblock
	make cpu-block
test:
	clear
	@echo '************************************ cpu0x-x $(DIM) noprint $(NCORE) *************************************************'
	./cpu-blas $(DIM) mul noprint
	@echo '************************************'
	./cpu-noblock $(DIM) mul noprint $(NCORE)
	@echo '************************************'
	./cpu-block $(DIM) mul noprint $(NCORE)
	@echo '************************************'
	./cpu-block $(DIM) mul noprint $(NCORE) sse2
	@echo '************************************'
	./cpu-block $(DIM) mul noprint $(NCORE) avx
	@echo '************************************'
	./cpu-block $(DIM) mul noprint $(NCORE) avx512f
	@echo '************************************'
	./cpu-block $(DIM) mul noprint $(NCORE) trans
	@echo '************************************'
	./cpu-block $(DIM) mul noprint $(NCORE) trans+sse2
	@echo '************************************'
	./cpu-block $(DIM) mul noprint $(NCORE) trans+avx
	@echo '************************************'
	./cpu-block $(DIM) mul noprint $(NCORE) trans+avx512f
	@echo 'End ********************************'
cpu-block:
	# see lscpu | grep avx
	gcc cpu-block.cpp -o cpu-block -std=c++17 -lstdc++ -lm -fopenmp -mavx -mfma -mavx512f $(OPT) $(ARCH) $(DEBUG)
	./cpu-block 5 mul print 1 trans
cpu-noblock:
	gcc cpu-noblock.cpp -o cpu-noblock -std=c++17 -lstdc++ -fopenmp $(OPT) $(ARCH) $(DEBUG)
	./cpu-noblock 5 mul print 1
cpu-blas:
	gcc cpu-blas.cpp -o cpu-blas -std=c++17 -lstdc++ `pkg-config blaspp --cflags --libs` -fopenmp $(OPT) $(ARCH) $(DEBUG)
	./cpu-blas 5 mul print 1
gpu05:
	nvcc gpu05.cu -o gpu05 -std=c++17 -lstdc++ -lcudart -Xcompiler=-fopenmp $(OPT)
	./gpu05 5 mul print
gpu02:
	nvcc gpu02.cu -o gpu02 -std=c++17 -lstdc++ -lcudart -Xcompiler=-fopenmp
	./gpu02 2 2 1 3 3 1
gpu01:
	nvcc gpu01.cu -o gpu01 -std=c++17 -lstdc++ -lcudart  -Xcompiler=-fopenmp 
	#для вызова метематических функций в кернеле нужно указывать флаг компилятора: --expt-relaxed-constexpr
	./gpu01
a2b:
	gcc a2b.cpp -o a2b -lstdc++ -mavx -mavx2 -mavx512vl -std=c++17
	./a2b
	rm a2b
