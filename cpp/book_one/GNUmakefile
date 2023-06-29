5:
	g++ exam-005.cpp -o exam-005 -lstdc++ -fopenmp -std=c++17 -O3
	./exam-005
	rm exam-005
4:
	clear
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
3:
	g++ exam-003-p66.cpp -o exam-003-p66 -lstdc++ -fopenmp -std=c++17 -O3
	./exam-003-p66
	rm exam-003-p66
2:
	g++ exam-002-p65.cpp -o exam-002-p65 -lstdc++ -fopenmp -std=c++17 -O1
	./exam-002-p65
	rm exam-002-p65
1:
	g++ exam-001-p61.cpp -o exam-001-p61 -lstdc++ -std=c++17 -fopenmp -O3
	./exam-001-p61
	rm exam-001-p61
0-2:
# autocode generation
	g++ gen_exam.cpp -o gen_exam -lstdc++ -std=c++17
	cp exam-000-p61-header.cpp exam-000-p61-auto.cpp
	./gen_exam 1 >> exam-000-p61-auto.cpp
	./gen_exam 2 >> exam-000-p61-auto.cpp
	./gen_exam 4 >> exam-000-p61-auto.cpp
	./gen_exam 8 >> exam-000-p61-auto.cpp
	./gen_exam 16 >> exam-000-p61-auto.cpp
	./gen_exam 32 >> exam-000-p61-auto.cpp
	./gen_exam 64 >> exam-000-p61-auto.cpp
	./gen_exam 128 >> exam-000-p61-auto.cpp
	./gen_exam 256 >> exam-000-p61-auto.cpp
	./gen_exam 512 >> exam-000-p61-auto.cpp
	./gen_exam 1024 >> exam-000-p61-auto.cpp
#./gen_exam 2048 >> exam-000-p61-auto.cpp
#./gen_exam 4096 >> exam-000-p61-auto.cpp
#./gen_exam 16384 >> exam-000-p61-auto.cpp
#./gen_exam 32768  >> exam-000-p61-auto.cpp
	cat exam-000-p61-footer.cpp >> exam-000-p61-auto.cpp
	rm gen_exam

	g++ exam-000-p61-auto.cpp -o exam-000-p61-auto-0 -lstdc++ -fopenmp -std=c++17 -O0 #-freorder-blocks -ftree-vectorize -funroll-loops
	./exam-000-p61-auto-0 #> exam-000-p61-auto-opt0.res
	rm exam-000-p61-auto-0
	g++ exam-000-p61-auto.cpp -o exam-000-p61-auto-3 -lstdc++ -fopenmp -std=c++17 -O3
	./exam-000-p61-auto-3 #> exam-000-p61-auto-opt3.res
	rm exam-000-p61-auto-3
0-1:
	g++ exam-000-p61.cpp -o exam-000-p61 -lstdc++ -fopenmp -std=c++17 -O0
	./exam-000-p61
	rm exam-000-p61
	g++ exam-000-p61.cpp -o exam-000-p61 -lstdc++ -fopenmp -std=c++17 -O3
	./exam-000-p61
	rm exam-000-p61
gen:
	g++ gen_exam.cpp -o gen_exam -lstdc++ -std=c++17
	./gen_exam 4
	rm gen_exam
simd:
	g++ simdinfo.cpp -o simdinfo -lstdc++ -std=c++17
	./simdinfo
	rm simdinfo







