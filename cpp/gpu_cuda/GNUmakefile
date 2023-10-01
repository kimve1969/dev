.PHONY: 01 02 03 04 05 06
07:
	gcc cpu06.cpp -o cpu06 -std=c++17 -lstdc++ -lm -fopenmp -O3
	./cpu06 4 mul print 1
06:
	gcc cpu05.cpp -o cpu05 -std=c++17 -lstdc++ -fopenmp -O3
	./cpu05 5 mul print 1
05:
	clear
	nvcc gpu05.cu -o gpu05 -std=c++17 -lstdc++ -lcudart -Xcompiler=-fopenmp
	@echo '************************ MATRIX OPERATION ***********************'
	@echo '************************ |C| = |A| ADD |B|, [ 5, 5] ********************'
	./gpu05 5 add print
	@echo '************************ |C| = |A| MUL |B|, [ 5, 5] ********************'
	./gpu05 5 mul print
	@echo '************************ |C| = |A| ADD |B|, [ 300, 300] ****************'
	./gpu05 300 add noprint
	@echo '************************ |C| = |A| MUL |B|, [ 300, 300] ****************'
	./gpu05 300 mul noprint
	@echo '************************ |C| = |A| ADD |B|, [ 3 000, 3 000 ] ************'
	./gpu05 3000 add noprint
	@echo '************************ |C| = |A| MUL |B|, [ 3 000, 3 000 ] ************'
	./gpu05 3000 mul noprint
04:
	nvcc gpu04.cu -o gpu04 -std=c++17 -lstdc++ -lcuda -Xcompiler=-fopenmp
	./gpu04 100
03:
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
02:
	nvcc gpu02.cu -o gpu02 -std=c++17 -lstdc++ -lcudart -Xcompiler=-fopenmp
	./gpu02 2 2 1 3 3 1
	./gpu02 2 2 1 3 3 1 > 02
	cat 02 | wc -l
#rm kernel02
01:
	nvcc gpu01.cu -o gpu01 -std=c++17 -lstdc++ -lcudart  -Xcompiler=-fopenmp
#для вызова метематических функций в кернеле нужно указывать флаг компилятора: --expt-relaxed-constexpr
	./gpu01
#rm kernel01
