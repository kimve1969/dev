.PHONY: 01 02 03 04 05 06
build:
	clear
	rm -rf cpu06 cpu06-2 cpu06-3
	make cpu06
	make cpu06-2
	make cpu06-3
	@echo '************************************ cpu0x 1000 nopint *************************************************'
	./cpu06 1000 mul noprint
	@echo 'End ********************************'
	./cpu06-2 1000 mul noprint
	@echo 'End ********************************'
	./cpu06-3 1000 mul noprint
	@echo 'End ********************************'
build2:
	clear
	rm cpu05 cpu06 cpu06-1
	make cpu05
	make cpu06
	make cpu06-1
	@echo '************************************ cpu0x 1000 nopint *************************************************'
	./cpu05 1000 mul noprint
	@echo 'End ********************************'
	./cpu06 1000 mul noprint
	@echo 'End ********************************'
	./cpu06-1 1000 mul noprint
	@echo '************************************ cpu0x 2000 nopint 8 ***********************************************'
	./cpu05 2000 mul noprint 8
	@echo 'End ********************************'
	./cpu06 2000 mul noprint 8
	@echo 'End ********************************'
	./cpu06-1 2000 mul noprint 8
cpu06-3:
	gcc cpu06-3.cpp -o cpu06-3 -std=c++17 -lstdc++ -lm -fopenmp -mavx512f -O3 
	./cpu06-3 5 mul print 1
cpu06-2:
	gcc cpu06-2.cpp -o cpu06-2 -std=c++17 -lstdc++ -lm -fopenmp -mfma -O3 
	./cpu06-2 5 mul print 1
cpu06-1:
	gcc cpu06-1.cpp -o cpu06-1 -std=c++17 -lstdc++ -lm -fopenmp -O3 
	./cpu06-1 5 mul print 1
cpu06:
	gcc cpu06.cpp -o cpu06 -std=c++17 -lstdc++ -lm -fopenmp -O3
	./cpu06 5 mul print 1
cpu05:
	gcc cpu05.cpp -o cpu05 -std=c++17 -lstdc++ -fopenmp -O3
	./cpu05 5 mul print 1
gpu05:
	nvcc gpu05.cu -o gpu05 -std=c++17 -lstdc++ -lcudart -Xcompiler=-fopenmp
	./gpu05 5 mul print
gpu04:
	nvcc gpu04.cu -o gpu04 -std=c++17 -lstdc++ -lcuda -Xcompiler=-fopenmp
	./gpu04 100
gpu03:
#max num_blocks = 31, max num_threads = 1024
	nvcc gpu03.cu -o gpu03 -std=c++17 -lstdc++ -lcudart -Xcompiler=-fopenmp
	@echo '************************ num_bloks : num_thrads ***********************'
	@echo '************************ 1 : 128 **************************************'
	./gpu03 1 128
	@echo '************************ 1 : 256 **************************************'
	./gpu03 1 256
	@echo '************************ 1 : 512 **************************************'
	./gpu03 1 512
	@echo '************************ 1 : 1024 *************************************'
	./gpu03 1 1024
	@echo '************************ 16 : 1024 ************************************'
	./gpu03 16 1024
	@echo '************************ 31 : 1024 ************************************'
	./gpu03 20 1024
gpu02:
	nvcc gpu02.cu -o gpu02 -std=c++17 -lstdc++ -lcudart -Xcompiler=-fopenmp
	./gpu02 2 2 1 3 3 1
gpu01:
	nvcc gpu01.cu -o gpu01 -std=c++17 -lstdc++ -lcudart  -Xcompiler=-fopenmp
	#для вызова метематических функций в кернеле нужно указывать флаг компилятора: --expt-relaxed-constexpr
	./gpu01

