#include<iostream>
#include<initializer_list>
#include<algorithm>
#include<vector>

class A
{
public:
	A():n_(0), arr_(nullptr){}

	A(int n):n_(n), arr_(new int(n))
	{
		for(int i=0; i < n_; ++i)
		{
			arr_[i] = -1;
		}
	}

	A( std::initializer_list<int> l ):n_( l.size() ) , arr_( new int(n_) )
	{
		std::copy( l.begin(), l.end(), arr_ );
	}

	~A()
	{
		delete arr_;
	}

	friend std::ostream& operator<<(std::ostream& out, const A& a);

private:
	int n_;
	int* arr_;
};

std::ostream& operator<<(std::ostream& out, const A& a)
{
	for(int i=0; i < a.n_; ++i)
	{
		out<<a.arr_[i]<<(i < a.n_-1 ? "," : "");
	}
	return out;
};

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";
	A a1(3);
	std::cout<<a1<<std::endl;

	A a2{1,2,3,4,5,6,7,8,9};
	std::cout<<a2<<std::endl;

	return 0;
}
