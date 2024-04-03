#include<iostream>
#include<cstdlib>
/*
 
	Пример пользовательского глобального new, new[], delete, delete[]
	оператор new[] выделяет память на sizeof(long) больше для хранения информации о кол-ве эл. в массиве

 * */
#ifndef USER_OPERATOR_NEW
void* operator new(std::size_t s)
{
	std::cout<<"call ::operator new( "<<s<<" )\n";
	if(void *p = std::malloc(s)) return p;
	
	throw(std::bad_alloc{});
}

void* operator new[](std::size_t s)
{
	std::cout<<"call ::opartor new[]( "<<s<<" )\n";
	if(void* p = std::malloc(s)) return p;

	throw( std::bad_alloc{} );
}

void operator delete(void *p)
{
	std::cout<<"call ::operator delete( "<<p<<" )\n";
	std::free(p);
}

void operator delete[](void *p)
{
	std::cout<<"call ::operator delete[] ( "<<p<<" )\n";
	std::free(p);
}
#endif

struct A
{
	A(char c = 'X'):m_(c){ std::cout<<"cst A( "<<m_<<" )\n"; }
	~A(){ std::cout<<"dst ~A( "<<m_<<" )\n"; }
	char m_;
};

struct B
{
	B(char c):m_(c){};
	~B() = default;
	char m_;
};

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";

	int *p = new int;
	delete p;
	std::cout<<"----------------------------------------------------------------\n";

	int *pi = new int[2];
	delete[] pi;
	std::cout<<"----------------------------------------------------------------\n";

	A *pa = new A;
	delete pa;
	std::cout<<"----------------------------------------------------------------\n";

	A *parr = new A[2]{'A','B'};
	std::cout<<"IF class A has user-defined dst, THEN operator new[] allocates befor pointer information about N elements in array A\n"\
	"WHEN it has called operator delete[], compiler calls N times dst of class A\n"\
	"value of N compiler writes befor array A = "<<*reinterpret_cast<long*>(parr - sizeof(long))<<", A[0] = "<<parr[0].m_<<", A[1] = "<<parr[1].m_<<"\n";	
	delete[] parr;
	std::cout<<"value of N befor array A after delete[] = "<<*reinterpret_cast<long*>(parr - sizeof(long))<<"\n";
	std::cout<<"----------------------------------------------------------------\n";

	B *parrb = new B[2]{'C','D'};
	std::cout<<"IF class B has't user-defined dst, THEN operator new[] don't wrrite information about N elements in array B\n"\
	"WHEN it has called operator delete[], compiler realise memory whithout calls of dst B\n"\
	"value of N befor array B (compiler don't write) = "<<*reinterpret_cast<long*>(parrb - sizeof(long))<<", B[0] = "<<parrb[0].m_<<", B[1] = "<<parrb[1].m_<<"\n";	
	delete[] parrb;
	std::cout<<"value of N befor array B after delete[] = "<<*reinterpret_cast<long*>(parrb - sizeof(long))<<"\n";
	std::cout<<"----------------------------------------------------------------\n";

	std::cout<<"End\n";
	return EXIT_SUCCESS;
}
