.PHONY: blas05 cpu05 cpu06 cpu06-trans cpu06-trans-sse cpu06-trans-avx512f cpu06-trans-scalar
OPT=-O3
DIM=2000
NCORE=8

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
	./cpu-block $(DIM) mul noprint $(NCORE) trans
	@echo '************************************'
	./cpu-block $(DIM) mul noprint $(NCORE) trans+red
	@echo '************************************'
	./cpu-block $(DIM) mul noprint $(NCORE) trans+sse2
	@echo '************************************'
	./cpu-block $(DIM) mul noprint $(NCORE) trans+avx
	@echo '************************************'
	./cpu-block $(DIM) mul noprint $(NCORE) trans+avx512f
	@echo 'End ********************************'
cpu-block:
	# see lscpu | grep avx
	gcc cpu-block.cpp -o cpu-block -std=c++17 -lstdc++ -lm -fopenmp -mavx -mfma -mavx512f $(OPT)
	./cpu-block 5 mul print 1 trans
cpu-noblock:
	gcc cpu-noblock.cpp -o cpu-noblock -std=c++17 -lstdc++ -fopenmp $(OPT)
	./cpu-noblock 5 mul print 1
cpu-blas:
	gcc cpu-blas.cpp -o cpu-blas -std=c++17 -lstdc++ `pkg-config blaspp --cflags --libs` -fopenmp $(OPT)
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

