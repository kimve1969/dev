#include<iostream>
#include<cstdlib>
#include<string>
//#include<new>

/*
	Пример operator new
	см. соглашения при написании пользовательских new и delete 
	Правило 51, стр.252
	1. Бесконечный цикл для выделения памяти
	2. функция обработчик на случай, если не получается выделить память
	3. обработка выделение 0 байта
	4. для класс предусмотреть возможность выделения большего размера, чем ожидалось (в случае создания объекта производного класса) 
*/
void handler()
{
	std::cout<<"call from user-handler(): memory allocation failed, terminated\n";
	// TODO trying free memory...
	std::set_new_handler(nullptr);
}

void* operator new[](std::size_t s)
{
	if( s == 0 ) s = 1;

	std::cout<<"call ::operator new[]( "<<s<<" )\n";

	while(true)
	{
		if(void *p = std::malloc(s)) return p;
		else 
		{
			auto hndl = std::get_new_handler();
			if(hndl) (*hndl)();
			else throw( std::bad_alloc{} );
		}
	}
}

void operator delete[](void *p, std::size_t s)
{
	std::cout<<"call ::operator delete[]( "<<p<<", "<<s<<")\n";
	std::free(p);
}

struct A
{
	A(){};
	~A(){};
	char m_;
};

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";

	std::set_new_handler( handler );
	A *pa = new A[1'000'000'000'000];
	delete[] pa;

	std::cout<<"End\n";
	return EXIT_SUCCESS;
}
