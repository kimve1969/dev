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
	@echo "make minimun - install minimum package: vim, gcc" 
	@echo "make upload  - upload all modified to github"
	@echo "You are su or sudo for install"
minimum:
	sudo apt update
	sudo apt install vim
	sudo apt install build-essential # GNU gcc
tools:
	sudo apt install make
	sudo apt install cmake
	sudo apt install man
	sudo apt install emacs
	sudo apt install clang	# clang
	sudo apt install openmpi-bin # open mpi
	sudo apt install valgrind	# valgring [--tool=memcheck | --tool=cache]
	sudo apt install gdb		# GNU debuger
	sudo apt install pkg-config
	sudo apt install doxygen
opencl:
	sudo apt install pocl-opencl-icd	# install "Portable Computing Language" platform
	sudo apt install ocl-icd-libopencl1 #
	sudo apt install clinfo	# utilite open CL, you can see platforn run clinfo
	sudo apt install ocl-icd-opencl-dev	# open CL, see headers files in /usr/include/CL
cuda:
	#CUDA toolkit for debian 11, see https://docs.nvidia.com/cuda/cuda-installation-guide-linux/index.html
	wget https://developer.download.nvidia.com/compute/cuda/repos/debian11/x86_64/cuda-keyring_1.1-1_all.deb
	sudo dpkg -i cuda-keyring_1.1-1_all.deb
	sudo add-apt-repository contrib
	sudo apt-get update
	sudo apt-get -y install cuda
	rm cuda-keyring_1.1-1_all.deb 
python:
	sudo apt install python3
	rm -f python			# delete sybmol link 
	sudo ln -fs /usr/bin/python3 /usr/bin/python	# create symbol link to python3
	sudo ln -fs /usr/bin/python /usr/bin/py
	#sudo apt install pip3
blas:
	#CBLAS, OPENBLAS, LAPACK and othes... 
	sudo apt install libblas-dev
	sudo apt install libopenblas-dev
	sudo apt install liblapack-dev
	#Install BLAS++ from github
test:
	mkdir ../build
	git clone https://github.com/icl-utk-edu/blaspp	../build/blaspp
	cd ../build/blaspp
	make && sudo make install
	make docs  # documentation available in docs/html/index.html
	#4. See directory /opt/slate                                                         
	#	/opt/slate/lib/libblaspp.so  # this is BLAS++ library
	#	/opt/slate/lib/pkgconfig/blaspp.pc  # this option for gcc
	#	/opt/slate/include
	# Create symbol link for blaspp.pc
	sudo ln -s /opt/slate/lib/pkgconfig/blaspp.pc /usr/lib/pkgconfig/blaspp.pc
	#check                       
	pkg-config --list-all | grep blaspp
	pkg-config blaspp --cflags
	#Run all tests from ./blaspp/test/ python run_tests.py
googletest:
	#Install Google Test https://habr.com/ru/post/667880
	sudo apt install googletest
	sudo apt install libgtest-dev libgmock-dev
	sudo apt install libtbb-dev                                                                                     
test2:	
	cd /usr/src/googletest                                                                                        
	sudo mkdir build
	cd build
	sudo cmake ..                                                                                      
	sudo make
	#copy libgmock.a libgmock_main.a libgtest.a libgtest_main.a
	sudo cp lib/* /usr/lib	
math:
	sudo apt install octave 		# see man octave, octave --help
upload:
	git push --set-upstream origin master

