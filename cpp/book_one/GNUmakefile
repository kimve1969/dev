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
	g++ exam-002-p65.cpp -o exam-002-p65 -lstdc++ -fopenmp -std=c++17 -O3
	./exam-002-p65
	rm exam-002-p65
1:
#	g++ exam-001-p61.cpp -o exam-001-p61 -lstdc++ -fopenmp -std=c++17 -funroll-loops -ftree-vectorize -freorder-functions
#	./exam-001-p61
#	rm exam-001-p61
	@echo "compile with optimization -O3"
	g++ exam-001-p61.cpp -o exam-001-p61 -lstdc++ -fopenmp -std=c++17 -O3
	./exam-001-p61
	rm exam-001-p61
0:
	g++ exam-000-p61.cpp -o exam-000-p61 -lstdc++ -fopenmp -std=c++17 
	./exam-000-p61
	rm exam-000-p61
	@echo "compile with optimization -O3"
	g++ exam-000-p61.cpp -o exam-000-p61 -lstdc++ -fopenmp -std=c++17 -O3
	./exam-000-p61
	rm exam-000-p61


