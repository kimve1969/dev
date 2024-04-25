#include<iostream>
#include<typeinfo>

struct Base{ virtual ~Base(){}  };

struct Derived : public Base { virtual ~Derived() override {} };

struct NoDerived { virtual ~NoDerived(){} };

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";

	Derived d;
	NoDerived nd;

	try
	{
		Base &bd = dynamic_cast<Base&>(d); 	// upcast
		Base &bnd = dynamic_cast<Base&>(nd);

	}
	catch(const std::bad_cast& e)
	{
		std::cout<<"e.what() : "<<e.what()<<"\n";
	}

	std::cout<<"End\n";
	return EXIT_SUCCESS;
}
