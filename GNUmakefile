# 
# @echo set username and password
# git config credential.helper store
# git push
# https://github.com: <username>
# https://kimve1969@github.com: <password>
# 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
# git config --global credential.helper 'cache --timeout 8640000'
# git config --global --edit
help:
	@echo "choice parameter:"
	@echo "make install - install all packets and library (emacs, gcc, blas, googletest and etc.) for NEW Linux" 
	@echo "make upload  - upload all modified to github"
	@echo "You are su or sudo for install"
install:
#1. Install common tools
	apt update
	apt install vim
	apt install man
	apt install emacs
	apt install build-essential # GNU gcc
	apt install clang	# clang
	apt install openmpi-bin # open mpi
	apt install slurm	# slurm
	apt install valgrind	# valgring [--tool=memcheck | --tool=cache]
	apt install gdb		# GNU debuger
	apt install pkg-config
	apt install intel-opencl-icd	# install "Intel Gen OCL Driver" platform 
	apt install beignet-opencl-icd # install "Clover" platform
	apt install pocl-opencl-icd	# install "Portable Computing Language" platform
	apt install ocl-icd-libopencl1 #
	apt install clinfo	# utilite open CL, you can see platforn run clinfo
	apt install ocl-icd-opencl-dev	# open CL, see headers files in /usr/include/CL
	apt install python3
	rm -f python			# delete sybmol link 
	ln -s /usr/bin/python3 /usr/bin/python	# create symbol link to python3
	ln -s /usr/bin/python /usr/bin/py
	apt install pip3
	apt install doxygen
#2. Install CBLAS, OPENBLAS, LAPACK and othes...                                                                                       
	apt install libblas-dev
	apt install libopenblas-dev
	apt install liblapack-dev

#3. Install BLAS++ from github
	mkdir build
	cd build
	git clone https://github.com/icl-utk-edu/blaspp	# /build/blaspp
	cd blaspp
	make && make install
	make docs  # documentation available in docs/html/index.html
	cd ../..
#4. See directory /opt/slate                                                         
#	/opt/slate/lib/libblaspp.so  # this is BLAS++ library
#	/opt/slate/lib/pkgconfig/blaspp.pc  # this option for gcc
#	/opt/slate/include
# Create symbol link for blaspp.pc                                                                                 
	ln -s /opt/slate/lib/pkgconfig/blaspp.pc /usr/lib/pkgconfig/blaspp.pc
#check return:                        
	pkg-config --list-all | grep blaspp                                                                                
	pkg-config blaspp --cflags                                                                                         

#Install Google Test https://habr.com/ru/post/667880
	apt install googletest
        apt install libgtest-dev libgmock-dev
        apt install libtbb-dev                                                                                       
        apt install cmake                                                                                             
        cd /usr/src/googletest                                                                                        
        mkdir build                                                                                                   
        cd build                                                                                                      
        cmake ..                                                                                                      
        make                                                                                                          
# copy libgmock.a libgmock_main.a libgtest.a libgtest_main.a                                                          
        cp lib/* /usr/lib


#5. Run all tests from ./blaspp/test/
	python run_tests.py
math:
	apt install octave 		# see man octave, octave --help
cppman:
	pip3 install cppman		# install man c++
	cppman -c 			# cache from cppreference.com and cplusplus.com, about 5300 files *.gz in directory: /root/.cache/cppman/cppreference.com
	cppman --use-mandb=true		# cppman adds manpages to mandb ?? not work form man
	cppman --pager=vim		# you can use: cppman vector
upload:
	git push --set-upstream origin master

clear:

