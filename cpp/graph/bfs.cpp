#include<iostream>
#include<vector>
#include<queue>
#include<cstdio>

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";
		
	int N, start;
	std::cin >> N >> start;
	
	std::vector< std::vector< int > > graph( N );
	int a, b;

	while( std::cin >> a >> b )
	{
		a--;
		b--;
		graph[ a ].push_back( b );
		graph[ b ].push_back( a );
	}

	std::cout<<"Graph:\n";
	for( int i = 0; i < graph.size(); ++i )
	{
		std::cout<< (i + 1) <<" : ";
		for( auto &e : graph[ i ] ) std::cout<<" "<< (e + 1);
		std::cout<<"\n";
	}	
	
	// ----------------------------------------------	
	std::queue< int > q;		// Очередь обрабатываемых узлов графа
	std::vector< int > was( N, 0 ); // Обработанные узлы
	std::vector< int > order;	// Порядок обработки узлов
	
	start--; 	// index of graph is start - 1 

	q.push( start );

	while( ! q.empty() )
	{
		int vertex = q.front(); 	// extract vertex from queue
		q.pop();
		
		if( was[ vertex ] == 1 ) continue;

		order.push_back( vertex );	// write order by processing
		was[ vertex ] = 1; 		// check as processed

		// push into queue child vertex
		for( auto &e : graph[ vertex ] ) q.push( e );
	}

	std::cout<<"BFS:\n";
	for( auto &e : order ) std::cout << (e + 1) << " ";
	std::cout<<"\n";

	std::cout<<"End\n";
	return EXIT_SUCCESS;
}
