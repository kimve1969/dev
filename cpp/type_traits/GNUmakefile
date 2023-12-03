CXX=gcc
CXXFLAGS=-std=c++17
CXXLIBS=-lstdc++

type_traits01: type_traits01.cpp
	$(CXX) type_traits01.cpp -o type_traits01 $1 $(CXXFLAGS) $(CXXLIBS)
	cat ./type_traits01.cpp
	@echo "----------------------------------------------"
	./type_traits01
	rm type_traits01	
rank:
	gcc rank.cpp -o rank -lstdc++ -std=c++17
	./rank
	rm rank
integral_constant:
	gcc integral_constant.cpp -o integral_constant -lstdc++ -std=c++17
	./integral_constant
	rm integral_constant
decltype:
	gcc decltype.cpp -o decltype -lstdc++ -std=c++17
	./decltype
	rm decltype

