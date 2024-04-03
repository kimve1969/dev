#include<iostream>
#include<memory>

struct my_tree
{
	class node
	{
	public:
		node(int d):_data(d), _left(nullptr), _right(nullptr){ std::cout<<"node::cnt:"<<_data<<std::endl; }
		~node(){ std::cout<<"node::dst:"<<_data<<std::endl; }
	private:
		std::unique_ptr<node> _left, _right;
		int _data;
	};

	std::unique_ptr<node> header;

	my_tree(): header( new node(0) ){ std::cout<<"my_tree::cnt"<<std::endl; }
	~my_tree(){ std::cout<<"my_tree::dst"<<std::endl; }

	void add(int d)
	{
		
	}
};

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";

	{
		my_tree tree;
	}

	std::cout<<"End\n";
	return EXIT_SUCCESS;
}
