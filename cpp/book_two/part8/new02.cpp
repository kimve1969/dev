#include<iostream>
#include<cstdlib>
/*
	Пример опеартора new, delete для класса
 * */
struct A
{
	A(char c = 'X'):m_(c){ std::cout<<"cst A( "<<m_<<" )\n"; }
	~A(){ std::cout<<"dst ~A( "<<m_<<" )\n"; }
	char m_;

	void *operator new(std::size_t s)
	{
		std::cout<<"call A::operator new( "<<s<<" )\n";
		if(void *p = malloc(s)) return p;
		
		throw(std::bad_alloc{});
	}
	
	/*void operator delete(void *p)
	{
		std::cout<<"call A::operator delete( "<<p<<" )\n";
		std::free(p);
	}*/
	
	void operator delete(void *p, std::size_t s)
	{
		std::cout<<"call A::operator delete( "<<p<<", "<<s<<" )\n";
		std::free(p);
	}
};

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";

	A *pa = new A;
	delete pa;

	std::cout<<"End\n";
	return EXIT_SUCCESS;
}
