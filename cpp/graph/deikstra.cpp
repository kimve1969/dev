#include<iostream>
#include<vector>
#include<queue>
#include<cstdio>
#include<limits>

constexpr int CNST_INFINITY = std::numeric_limits< int >::max();

struct to_t
{
	int to;
	int cost;
};

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";

	int N, start;
	std::cin >> N >> start;
	
	std::vector< std::vector< to_t > > graph( N );
	{
		int from, to, cost;
		while( std::cin >> from >> to >> cost )
		{
			from--;
			to--;
			graph[ from ].push_back( { to, cost } );
			graph[ to ].push_back( { from, cost } );
		}
	}

	std::cout << "Graph:\nfrom : { to, cost } ... \n";
	for( int from = 0; from < graph.size(); ++from )
	{
		std::cout << (from + 1) <<" : ";
		for( auto &e : graph[ from ] ) std::cout<<" { "<< (e.to + 1) << ", " << e.cost << " }";
		std::cout<<"\n";
	}	
	
	// ----------------------------------------------	
	start--; 	// index of graph is start - 1 

	std::queue< int > q;						// Очередь обрабатываемых вершин графа
	std::vector< int > was( N, 0 ); 				// Обработанные вершины
	std::vector< int > order;					// Порядок обработки вершин
	std::vector< long > sum_costs( N, CNST_INFINITY );		// Стоимость вершин по результатам обхода. Вначале принимаем бесконечность
	std::vector< std::vector< int > > routers( N, {start} );	// Все маршруты от стартовой позиции 
	
	q.push( start );
	sum_costs[ start ] = 0;

	while( ! q.empty() )
	{
		int from = q.front(); 	// to extract vertex from queue
		q.pop();
		
		if( was[ from ] == 1 ) continue;

		order.push_back( from );	// to write order by processing
		was[ from ] = 1; 		// to mark as processed

		// to push into queue child vertexes
		for( auto &e : graph[ from ] ) 
		{
			q.push( e.to );
			//----------- processing ----------
			if(  sum_costs[ from ] + e.cost < sum_costs[ e.to ] )
			{
				sum_costs[ e.to ] = sum_costs[ from ] + e.cost;	// update sum costs of vertex
				routers[ from ].push_back( e.to );		// 
			}
		}
	}

	std::cout << "Order proceesing of vertex:\n";
	for( auto &e : order )
	{
		std::cout << ( e + 1 ) << " , ";
	}
	std::cout << "\n";

	std::cout << "Deikstra algorithm:\nfrom , to , sum_costs, router\n";
	for( int to = 0; to < sum_costs.size(); ++to ) 
	{
		std::cout << ( start + 1) << " , " << ( to + 1 ) << " , " << sum_costs[ to ] << " , { ";
		for( auto &e : routers[ to ] )
		{
			std::cout << ( e + 1 ) << " , ";
		}
		std::cout << " }\n";
	}
	std::cout<<"\n";

	std::cout<<"End\n";
	return EXIT_SUCCESS;
}
