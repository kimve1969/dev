build:
	@clear
	@make unroll
	@echo
	@make sse2
	@echo
	@make unroll+sse2
	@echo
	@make unroll+sse2+omp
build2:
	@clear
	@make band_width
	@echo
	@make unroll_template
	@echo
	@make big_unroll
	@echo
	@make test_cash

unroll+sse2+omp:
	@echo "##################### \033[31m  BEGIN EXAMPLE - UNROLLING LOOP + SIMD SSE2 + OMP \033[0m ########################################"
	g++ exam-000-unroll_sse2_omp.cpp -o exam-000-unroll_sse2_omp -lstdc++ -fopenmp -std=c++17 -O0
	./exam-000-unroll_sse2_omp
	rm exam-000-unroll_sse2_omp
	@echo "########################################## \033[32m  END EXAMPLE \033[0m ##################################################"
unroll+sse2:
	@echo "##################### \033[31m  BEGIN EXAMPLE - UNROLLING LOOP + SIMD SSE2 \033[0m ########################################"
	g++ exam-000-unroll_sse2.cpp -o exam-000-unroll_sse2 -lstdc++ -fopenmp -std=c++17 -O0
	./exam-000-unroll_sse2
	rm exam-000-unroll_sse2
	@echo "########################################## \033[32m  END EXAMPLE \033[0m ##################################################"
sse2:
	@echo "################################ \033[31m BEGIN EXAMPLE - CPU INTRINSIC SIMD SSE \033[0m #################################"
	g++ exam-002-p65.cpp -o exam-002-p65 -lstdc++ -fopenmp -std=c++17 -O0
	./exam-002-p65
	rm exam-002-p65
	@echo "########################################## \033[32m END EXAMPLE \033[0m ##################################################"
unroll:
	@echo "################################ \033[31m  BEGIN EXAMPLE - UNROLLING LOOP \033[0m ########################################"
	g++ exam-000-p61.cpp -o exam-000-p61 -lstdc++ -fopenmp -std=c++17 -O0
	./exam-000-p61
	rm exam-000-p61
	@echo "########################################## \033[32m  END EXAMPLE \033[0m ##################################################"

#THIS EXTENTION EXAMPLES

cpuinfo:
# use __builtin_cpu_supports
	g++ simdinfo.cpp -o simdinfo -lstdc++ -std=c++17
	./simdinfo
	rm simdinfo
band_width:
	@echo "##################### \033[31m  BEGIN EXAMPLE - BAND WIDTH \033[0m ########################################################"
	g++ exam-003-p66.cpp -o exam-003-p66 -lstdc++ -fopenmp -std=c++17 -O3
	./exam-003-p66
	rm exam-003-p66
	@echo "########################################## \033[32m  END EXAMPLE \033[0m ##################################################"
unroll_template:
# use template function for unrolling loop
	@echo "##################### \033[31m  BEGIN EXAMPLE - USE TEMPLATE FOR UNROLL LOOP \033[0m#######################################"
	g++ exam-001-p61.cpp -o exam-001-p61 -lstdc++ -std=c++17 -fopenmp -O3
	./exam-001-p61
	rm exam-001-p61
	@echo "########################################## \033[32m  END EXAMPLE \033[0m ##################################################"
gen_auto:
# auto generation unrolling loop
	@echo "##################### \033[31m  BEGIN EXAMPLE - AUTO GENERATE UNROOL LOOP \033[0m #########################################"
	g++ gen_exam.cpp -o gen_exam -lstdc++ -std=c++17
	./gen_exam 4
	rm gen_exam
	@echo "########################################## \033[32m  END EXAMPLE \033[0m ##################################################"
big_unroll:
	@echo "##################### \033[31m  BEGIN EXAMPLE - AUTO GENERATE BIG UNROOL LOOP \033[0m #####################################"
# use auto generation unrolling loop (see above)
	g++ gen_exam.cpp -o gen_exam -lstdc++ -std=c++17
	cp exam-000-p61-header.cpp exam-000-p61-auto.cpp
	./gen_exam 1 >> exam-000-p61-auto.cpp
	./gen_exam 64 >> exam-000-p61-auto.cpp
	./gen_exam 1024 >> exam-000-p61-auto.cpp
	cat exam-000-p61-footer.cpp >> exam-000-p61-auto.cpp
	rm gen_exam

	g++ exam-000-p61-auto.cpp -o exam-000-p61-auto-0 -lstdc++ -fopenmp -std=c++17 -O0
	./exam-000-p61-auto-0
	rm exam-000-p61-auto-0
	g++ exam-000-p61-auto.cpp -o exam-000-p61-auto-3 -lstdc++ -fopenmp -std=c++17 -O3
	./exam-000-p61-auto-3
	rm exam-000-p61-auto-3
	@echo "########################################## \033[32m  END EXAMPLE \033[0m ##################################################"
test_cash:
	@echo "##################### \033[31m  BEGIN EXAMPLE - TEST CASH \033[0m #########################################################"
	g++ exam-004.cpp -o exam-004 -lstdc++ -fopenmp -std=c++17 -O3
	./exam-004 4096
	./exam-004 8192
	./exam-004 16384
	./exam-004 65536
	./exam-004 262144
	./exam-004 524288
	./exam-004 1048576
	./exam-004 4194304
	@echo "**************************************** Valrind test **************************"
	@echo "	--tool=cachgrind"
	@echo ""
	valgrind --tool=cachegrind --D1=16384,2,64 --L2=524288,2,64 ./exam-004 16000 #
	valgrind --tool=cachegrind --D1=16384,2,64 --L2=524288,2,64 ./exam-004 17000 #
	valgrind --tool=cachegrind --D1=16384,2,64 --L2=524288,2,64 ./exam-004 524288 #
	valgrind --tool=cachegrind --D1=16384,2,64 --L2=524288,2,64 ./exam-004 550000 #
	valgrind --tool=memcheck ./exam-004 16000
	rm exam-004
	rm cachegrind.out.*
	@echo "########################################## \033[32m  END EXAMPLE \033[0m ##################################################"








