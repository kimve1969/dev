#1. Befor install BLASPP
   su #enter password su
   apt update
   
#2. Install CBLAS, OPENBLAS, LAPACK and othes...

   apt install libblas-dev
   apt install libopenblas-dev
   apt install liblapack-dev
   
   apt install python
   apt install git
   apt install doxygen
   
   exit #exit su	
   
#3. Install BLAS++ from github
   
   git clone https://github.com/icl-utk-edu/blaspp
   cd blaspp
   make
   
   su #enter password su
   make install
   exit #exit su

   make docs  # documentation available in docs/html/index.html

#4. See directory /opt/slate
   /opt/slate/lib/libblaspp.so	# this is BLAS++ library
   /opt/slate/lib/pkgconfig/blaspp.pc  # this option for gcc
   /opt/slate/include		       # this is header BLAS++ for gcc

#4.1 Create symbol link for blaspp.pc
   ln -s /opt/slate/lib/pkgconfig/blaspp.pc /usr/lib/x86_64-linux-gnu/pkgconfig/blaspp.pc
   # check return:
   pkg-config --list-all | grep blaspp
   pkg-config blaspp --cflags

#5. Run all tests from ./blaspp/test/

   python run_tests.py


   
   
   
   

