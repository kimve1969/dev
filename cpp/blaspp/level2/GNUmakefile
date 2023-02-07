2:
	g++ gemv.cpp -o gemv -lstdc++ `pkg-config blaspp --cflags --libs`
	./gemv
	rm gemv
	octave gemv.m
1:
	g++ trsv.cpp -o trsv -lstdc++ `pkg-config blaspp --cflags --libs`
	./trsv
	rm trsv
	octave trsv.m


