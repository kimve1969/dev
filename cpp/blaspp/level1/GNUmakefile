8:
	g++ rot.cpp -o rot -lstdc++ `pkg-config blaspp --cflags --libs`
	./rot
	rm rot
7:
	g++ swap.cpp -o swap -lstdc++ `pkg-config blaspp --cflags --libs`
	./swap
	rm swap
6:
	g++ nrm2.cpp -o nrm2 -lstdc++ `pkg-config blaspp --cflags --libs`
	./nrm2
	rm nrm2
5:
	g++ iamax.cpp -o iamax -lstdc++ `pkg-config blaspp --cflags --libs`
	./iamax
	rm iamax
4:
	g++ dot.cpp -o dot -lstdc++ `pkg-config blaspp --cflags --libs`
	./dot
	rm dot
3:
	g++ copy.cpp -o copy -lstdc++ `pkg-config blaspp --cflags --libs`
	./copy
	rm copy
2:
	g++ axpy.cpp -o axpy -lstdc++ `pkg-config blaspp --cflags --libs`
	./axpy
	rm axpy
1:
	g++ asum.cpp -o asum -lstdc++ `pkg-config blaspp --cflags --libs`
	./asum
	rm asum
list:
	pkg-config --list-all | grep blas # return blas
