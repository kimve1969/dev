build:
	@clear
	@make size_of_cash_line
	@echo
	@make optimize_loop
	@echo
	@make optimize_sub_matrix
	@echo
	@make optimize_sub_matrix_omp
optimize_loop:
	@echo "############ \033[31m  BEGIN EXAMPLE - OPTIMIZE LOOP \033[0m ####################################"
	g++ matrix01.cpp -o matrix01 -lstdc++ -std=c++17
	./matrix01
	rm matrix01
	@echo "############ \033[32m  END EXAMPLE \033[0m ######################################################"
optimize_sub_matrix:
	@echo "############ \033[31m  BEGIN EXAMPLE - OPTIMIZE SUB MATRIX \033[0m ##############################"
	g++ matrix02.cpp -o matrix02 -lstdc++ -std=c++17
	./matrix02
#valgrind --tool=cachegrind --D1=1024,2,64 ./matrix02 #--D1=32768,2,64 --L2=524288,2,64
#rm cachegrind.out.*
	rm matrix02
	@echo "############ \033[32m  END EXAMPLE \033[0m ######################################################"
optimize_sub_matrix_omp:
	@echo "####### \033[31m  BEGIN EXAMPLE - OPTIMIZE SUB MATRIX + OMP \033[0m #############################"
	g++ matrix03.cpp -o matrix03 -lstdc++ -std=c++17 -fopenmp
	./matrix03
	rm matrix03
	@echo "############ \033[32m  END EXAMPLE \033[0m ######################################################"
size_of_cash_line:
# Result of test:
# Intel Gold (yandex) t2\t1 = 8 (size of cash line)
# ARM Samsung Galaxy  t2\t1 = 4 (size of cash line)
	@echo "############ \033[31m  BEGIN EXAMPLE - SIZE OF CASH LINE \033[0m ################################"
	g++ matrix00.cpp -o matrix00 -lstdc++ -std=c++17
	./matrix00
	rm matrix00
	@echo "############ \033[32m  END EXAMPLE \033[0m ######################################################"


