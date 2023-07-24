build:
	@clear
	@make size_of_cash_line
	@echo
	@make optimize_loop
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
	rm matrix02
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


