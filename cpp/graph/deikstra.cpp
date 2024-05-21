#include<iostream>
#include<vector>
#include<map>
#include<queue>
#include<cstdio>
#include<limits>
#include<algorithm>

constexpr int CNST_INFINITY = std::numeric_limits< int >::max();

struct edge_t
{
	int to;
	int cost;
};

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";

	int N, start;
	std::cin >> N >> start;
	
	std::vector< std::vector< edge_t > > graph( N );
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
		for( auto & [to, cost] : graph[ from ] ) std::cout<<" { "<< (to + 1) << ", " << cost << " }";
		std::cout<<"\n";
	}	
	
	// ----------------------------------------------	
	start--; 	// index of graph is start - 1 

	std::map< int, int > q;						// Очередь обрабатываемых вершин графа <Key, Value> Key is sum_costs, Value is index of vertex!
	std::vector< int > was( N, 0 ); 				// Обработанные вершины
	std::vector< int > order;					// Порядок обработки вершин
	std::vector< long > sum_costs( N, CNST_INFINITY );		// Стоимость вершин по результатам обхода. Вначале принимаем бесконечность
	std::vector< int > chains( N, -1 );

	q.insert( { 0, start } );		// start is vertex with sum cost is 0
	sum_costs[ start ] = 0;

	while( ! q.empty() )
	{
		auto iter = q.begin(); 	// to extract vertex with minimum cost
		auto [ sum_cost, from ] = *iter;
		q.erase ( iter );
		
		//if( was[ from ] == 1 ) continue;

		order.push_back( from );	// to write order by processing
		was[ from ] = 1; 		// to mark as processed

		// to push into queue child vertexes
		for( auto & [ to, cost ] : graph[ from ] ) 
		{
			//----------- processing ----------
			if(  sum_costs[ from ] + cost < sum_costs[ to ] )
			{
				sum_costs[ to ] = sum_costs[ from ] + cost;	// update sum costs of vertex
				chains[ to ] = from;									
			}

			if( was[ to ] != 1 ) q.insert( { sum_costs[ to ], to } );
		}
	}

	auto prn_path = [ & chains  ]( int finish )
	{
		std::vector< int > path;
		for( int v = finish; v != -1; v = chains[ v ] )
		{
			path.push_back( v );
		}
		std::reverse( path.begin(), path.end() );
		
		for( bool b{ false }; auto &v : path )
		{
			std::cout << ( b ? ", " : ( b = true, "" ) ) << (v + 1);
		}
	};

	std::cout << "vertex, sum of costs, { vertex path ... }:\n";
	for( int v = 0; v < sum_costs.size(); ++v )
	{
		std::cout << ( v + 1 ) << ", " << sum_costs[ v ] << ", { ";
		prn_path( v );
		std::cout << " }\n";
	}
	std::cout << "\n";

	std::cout << "Order proccesing of vertex:\n";
	for( int v = 0; v < order.size(); ++v )
	{
		std::cout << order[ v ] + 1 << " , ";
	}
	std::cout << "\n";

	std::cout<<"End\n";
	return EXIT_SUCCESS;
}
