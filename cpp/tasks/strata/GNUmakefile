heat_eq_2d:
	clear
	gcc heat_eq_2d.cpp -o heat_eq_2d -lstdc++ -lm -std=c++17
	./heat_eq_2d
	rm heat_eq_2d
heat_eq_1d_implicit:
	clear
	gcc heat_eq_1d_implicit.cpp -o heat_eq_1d_implicit -lstdc++ -std=c++17
	./heat_eq_1d_implicit
	rm heat_eq_1d_implicit
heat_eq_1d:
	clear
	gcc heat_eq_1d.cpp -o heat_eq_1d -lstdc++ -std=c++17
	./heat_eq_1d
	rm heat_eq_1d
test_tomas:
	clear
	gcc test_tomas.cpp -o test_tomas -lstdc++ -std=c++17
	./test_tomas
	rm test_tomas
all:
	make test_tomas
	make heat_eq_1d
	make heat_eq_1d_implicit
	make heat_eq_2d

