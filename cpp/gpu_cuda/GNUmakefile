.PHONY: blas05 cpu05 cpu06 cpu06-trans cpu06-trans-sse cpu06-trans-avx512f cpu06-trans-scalar
OPT=-O3	
DIM=1000
NCORE=1


cpu:
	clear
	rm -rf blas05 cpu05 cpu06 cpu06-trans cpu06-trans-scalar cpu06-trans-sse cpu06-trans-avx
	make blas05
	make cpu05
	make cpu06
	make cpu06-trans
	make cpu06-trans-scalar
	make cpu06-trans-sse
	make cpu06-trans-avx
test:
	clear
	@echo '************************************ cpu0x-x $(DIM) noprint $(NCORE) *************************************************'
	./blas05 $(DIM) mul noprint
	@echo '************************************'
	./cpu05 $(DIM) mul noprint $(NCORE)
	@echo 'End ********************************'
	./cpu06 $(DIM) mul noprint $(NCORE)
	@echo 'End ********************************'
	./cpu06-trans $(DIM) mul noprint $(NCORE)
	@echo 'End ********************************'
	./cpu06-trans-scalar $(DIM) mul noprint $(NCORE)
	@echo 'End ********************************'
	./cpu06-trans-sse $(DIM) mul noprint $(NCORE)
	@echo 'End ********************************'
	./cpu06-trans-avx $(DIM) mul noprint $(NCORE)
	@echo 'End ********************************'
cpu06-trans-avx:
	# see lscpu | grep avx
	gcc cpu06-trans-avx.cpp -o cpu06-trans-avx -std=c++17 -lstdc++ -lm -fopenmp -mavx -mfma $(OPT)
	./cpu06-trans-avx 5 mul print 1
cpu06-trans-sse:
	# see lscpu | grep sse
	gcc cpu06-trans-sse.cpp -o cpu06-trans-sse -std=c++17 -lstdc++ -lm -fopenmp -mfma $(OPT)
	./cpu06-trans-sse 5 mul print 1
cpu06-trans-scalar:
	gcc cpu06-trans-scalar.cpp -o cpu06-trans-scalar -std=c++17 -lstdc++ -lm -fopenmp $(OPT)
	./cpu06-trans-scalar 5 mul print 1
cpu06-trans:
	gcc cpu06-trans.cpp -o cpu06-trans -std=c++17 -lstdc++ -lm -fopenmp $(OPT)
	./cpu06-trans 5 mul print 1
cpu06:
	gcc cpu06.cpp -o cpu06 -std=c++17 -lstdc++ -lm -fopenmp $(OPT)
	./cpu06 5 mul print 1
cpu05:
	gcc cpu05.cpp -o cpu05 -std=c++17 -lstdc++ -fopenmp $(OPT)
	./cpu05 5 mul print 1
blas05:
	gcc blas05.cpp -o blas05 -std=c++17 -lstdc++ `pkg-config blaspp --cflags --libs` -fopenmp $(OPT)
	./blas05 5 mul print 1
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

