1:
	gcc gdb1.cpp -o gdb1 -lstdc++ -std=c++17 -g3
	ulimit -c unlimited # needed for core dump
	gdb --core=core ./gdb1
	rm gdb1
