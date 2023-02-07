#
CXX = g++
CXXFLAGS = -lstdc++
OBJS = matrix.o
RUN = matrix

matrix: $(OBJS)
	$(CXX) $(OBJS) -o $(RUN) $(CXXFLAGS)
	ls -l
	./$(RUN)
matrix.o: matrix.cpp
	$(CXX) matrix.cpp -c
run:
	./$(RUN)
clean:
	rm *.o
	ls -l

