3:
	g++ asum.cpp -o asum -lstdc++ -std=c++17 -pg `pkg-config blaspp --cflags --libs`
	./asum
	gprof asum gmon.out
	rm asum

2:
	g++ atomic_spin.cpp -o atomic_spin -lstdc++ -std=c++17 -pthread -pg
	./atomic_spin
	gprof atomic_spin gmon.out
	rm atomic_spin

1:
	g++ prof1.cpp -o prof1 -lstdc++ -std=c++17 -pg
	./prof1
	gprof prof1 gmon.out
	rm prof1


