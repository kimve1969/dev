#include<iostream>
#include<vector>
#include<algorithm>
#include<cstdio>
/*
 * 	Обход всех узлов неориентированного взвешенного графа в глубину - DFS, Deap First Seach
 * 	Вершины нумеруются на входе с 1, выводятся на печать тоже с 1, в векторе начинаются с 0 !!!
 *
 * */
bool is_vertex_processed( const std::vector<int> & was, int vertex )
{
	return was[ vertex ] != 0 ? true : false;
}

void dfs( const std::vector< std::vector< int > > & graph, int vertex , std::vector< int > & was)
{
	if( is_vertex_processed( was, vertex ) ) return;

	std::cout << (vertex + 1) <<" -> "; 	// Вход в рекурсию
	was[ vertex ] = 1;

	for( auto &e : graph[ vertex ] )
	{
		dfs( graph, e, was );
	}

	std::cout<< " <- ";			// Выход из рекурсии

	return;
}	

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";

	int N, start;
	std::cin >> N >> start;

	std::vector< std::vector<int> > graph(N);

	int from{}, to{};

	while( std::cin >> from >> to )
	{
		graph[ from - 1 ].push_back( to - 1 );
		graph[ to - 1 ].push_back( from - 1);
	}
	
	for(int from = 0;  from < graph.size(); ++from )
	{
		std::cout << (from + 1) << " : ";
		
		for(auto &to : graph[ from ] )
		{
			std::cout << " " << (to + 1);
		}
		std::cout<<"\n";
	}
	
	//------------------------------------------------------------
	std::vector<int> was(N, 0);			// контейнер оработанных узлов

	for( int vertex = 0; vertex < graph.size(); ++ vertex )
	{
		if(  not is_vertex_processed( was, vertex ) )
		{
			dfs( graph, vertex, was );
		}
	}

	std::cout<<"\nEnd\n";
	return EXIT_SUCCESS;
}
