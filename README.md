// my token
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	15/07/2023
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
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	18/09/2023
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
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
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
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
