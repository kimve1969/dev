// my token
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	02/04/2023
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	25/04/2023
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
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
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
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



// set user and mail
git config --global user.name=kimve1969
git config --global user.mail=kimve@mail.ru
git config --global --edit


// set username and password
git config credential.helper store
git push
https://github.com: <username>
https://kimve1969@github.com: <password>
git config --global credential.helper 'cache --timeout 8640000'
git config --global --edit


//help how story username and password:
git help credentials

// name of call in file
nm file

// name and path libriries
ldd <file>

//
clang file.cpp -o file -lstdc++ -pthread
-shared // create shared library
-E // preproccesing

// 
nm <file>
c++filt <simbol from file>

// ThreadSanitizer is a fast and easy-to-use data race detector into Clang > 3.1 and GCC > 4.7
// complie with -fsanitize=thread -fPIC
// link with -fsanitize=thread
// run and get data races printed to stderr
clang file.cpp -o file -lstdc++ -pthread -fPIC -fsanitize=thread

// file.ii is listing afetr preprocessing #if, #define 
clang -E file.cpp -o file.ii

// file.asm is assembler code
clang -S file.cpp -o file.asm

// compile debug version and run in gdb
clang -g file.cpp -o file
gdb ./file
(gdb) run