#include<iostream>
#include<type_traits>

int g(int x){
  std::cout<<"g("<<x<<")\n";
  return x+5;
};

int main(int argc, char *argv[])
{
	int a{5};
	decltype(a) b = a;

	std::cout<<std::boolalpha;
	std::cout<<"1. "<<std::is_same<decltype(a), decltype(b)>::value<<std::endl;
	std::cout<<"2. "<<std::is_same_v<decltype(a), decltype(b)><<"\n";
	std::cout<<"3. "<<std::is_same_v<decltype(g), int(int)><<std::endl;
	std::cout<<"4. "<<std::is_same_v<decltype(g(0)), int><<std::endl;

	int c[][3]={{1,2,3}};
	std::cout<<"5.1 "<<std::is_same_v< decltype(c[0]), int[3] ><<std::endl;
	std::cout<<"5.2 "<<std::is_same_v< decltype(c[0]), int(&)[3] ><<std::endl;
	std::cout<<"5.3 "<<std::is_same_v< std::remove_reference<decltype(c[0])>::type, int[3] ><<std::endl;


	auto f = [](int x){ return x+3; };

	decltype((g)) g1{g};
	int(*s)(int) = g;
	std::cout<<g1(2)<<std::endl;

	// g(0) is not called
	decltype(g(0)) g2 = 3;

	struct HasCnt{
		HasCnt(){
			std::cout<<"HasCnt()\n";
		}
		int foo(){return 1;}
	};

	// HasCnt() is not called
	decltype(HasCnt().foo()) g3 = 2;

	
	return 0;
}

