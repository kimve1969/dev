#
CXX = g++
CXXFLAGS = -lstdc++
OBJS = matrix.o
RUN = matrix
2:
	g++ arr.cpp -o arr -lstdc++ -std=c++17
	./arr
	rm arr
matrix: $(OBJS)
	$(CXX) $(OBJS) -o $(RUN) $(CXXFLAGS)
	ls -l
	./$(RUN)
	rm $(RUN)
matrix.o: matrix.cpp
	$(CXX) matrix.cpp -c
run:
	./$(RUN)
clean:
	rm *.o
	ls -l

