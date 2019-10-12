#include <iostream>
#include <vector>
#include <iomanip>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_traits.hpp"
#include "boost/graph/bellman_ford_shortest_paths.hpp"
#include <queue>
#include <time.h>
#include <ctime>

using namespace boost;

int main(int argc, char const *argv[]) {

	typedef std::pair < int, int >E;
	int n_edges = 58970, numNodes = 30000;

	std::clock_t start, stop;
  double duration;

	//E edge_array[] = {E(0,1), E(1,2), E(0,2) , E(1,3), E(3,1), E(3,2), E(1,4), E(4,3)};//no negative
	//int weight[n_edges] = {-1, 3, 4, 2, 1, 5, 2, -3};//no negative

	//E edge_array[] = {E(0,1), E(1,2), E(2,3), E(3,1), E(2,4)};//negative
	//int weight[n_edges] = {1, 1, 2, -5, 1};//negative

	//E edge_array[] = {E(0,1), E(1,2), E(2,3), E(3,1), E(0,4)};//negative
	//int weight[n_edges] = {0, 0, 0, -1, 1000};//negative


	typedef boost::adjacency_list<listS, vecS, bidirectionalS,
																property<boost::vertex_distance_t, int>,//property gia kombous
																property<boost::edge_weight_t, int>//property  gia akmes
																> Graph;

//******************************PLEGMA**********************************************
E edge_array[n_edges];
int weight[n_edges];

int k = 0;
for(int i = 0; i < 30; i++){
	for(int j=0; j < 1000; j++){
		int sub  = 0;
		if(i == 29){
			if(j<999){
				edge_array[k] = E(i,j+1);
				weight[k] = (rand() % 10 + 1) - sub;
				k++;
			}
		}
		else if(i <= 28){
			if(j<999){
				edge_array[k] = E(i,j+1);
				weight[k] = (rand() % 10 + 1) - sub;
				k++;
			}
			edge_array[k] = E(i,j+1000);
			weight[k] = (rand() % 10 + 1) - sub;
			k++;
		}
	}
}

//**********************************************


Graph g(edge_array, edge_array + n_edges, numNodes);
boost::adjacency_list<>::vertex_descriptor V[numNodes];



	typedef boost::graph_traits<Graph>::vertex_iterator vIter;
	std::pair<vIter, vIter> vip;

	graph_traits < Graph >::edge_iterator ei, ei_end;


  property_map<Graph, edge_weight_t>::type
    weight_pmap = get(edge_weight, g);

  int i = 0;
  for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei, ++i)
    weight_pmap[*ei] = weight[i];


	int distance[numNodes];
	std::vector<std::size_t> parent(numNodes);

	std::fill_n(distance, numNodes, INT_MAX);

	for (i = 0; i < numNodes; ++i)//numNodes
	  parent[i] = i;

	distance[0] = 0;


	start = std::clock();
	bool r = bellman_ford_shortest_paths(g, numNodes, weight_map(weight_pmap).distance_map(&distance[0]).predecessor_map(&parent[0]));

	if (r)
				std::cout << "True" << '\n';
  else
    std::cout << "negative cycle" << std::endl;

	stop = std::clock();
	duration = (stop - start) / (double)CLOCKS_PER_SEC;
	std::cout << "Runtime of Boost Bellman-Ford is: " << duration*1000.0 << " ms \n";


for (i = 0; i < numNodes; ++i)
	parent[i] = i;

std::fill_n(distance, numNodes, INT_MAX);
distance[0] = 0;

start = std::clock(); //h clock metraei kykloys cpu
/*
=======================================================================
													BELLMAN FORD
=======================================================================
*/
int phase = 0;
std::deque<int> Q;

bool in_Q[numNodes];
std::fill_n(in_Q, numNodes, false);

Q.push_front(0);
in_Q[0] = true;
Q.push_back(-1);//end marker

int u;
while( phase < numNodes ){
	u = Q.front();
	Q.pop_front();
	if ( u == -1){
		phase++;
		if ( Q.empty() ) {
					std::cout << "True" << '\n';
					stop = std::clock();
					duration = (stop - start) / (double)CLOCKS_PER_SEC; //dia to CLOCKS_PER_SEC pernoyme xrono
					std::cout << "Runtime of homemade Bellman-Ford is: " << duration*1000.0 << " ms \n";
			return 0;
		}
		Q.push_back(-1);
		continue;
	}
	else in_Q[u] = false;


	//ITERATOR DECLARE FOR OUT EDGES
	typedef boost::graph_traits<Graph>::out_edge_iterator oEdgeIter;
	typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
	oEdgeIter oei, oei_end;
	Vertex v;
	if(u != -1){
		Vertex s = *(boost::vertices(g).first + u);
		int du = distance[u];
		for(boost::tie(oei, oei_end) = boost::out_edges(s, g); oei != oei_end; ++oei){
			v = boost::target(*oei, g);
			int d = du + weight_pmap[*oei];
			if ( (parent[v] == -1 && v != 0) || d < distance[v] ){
				distance[v] = d;
				parent[v] = u;
				if ( !in_Q[v] ) {
					Q.push_back(v);
					in_Q[v] = true;
				}
			}
		}
	}
}
	std::cout << "negative cycle" << std::endl;

	stop = std::clock();
	duration = (stop - start) / (double)CLOCKS_PER_SEC; //dia to CLOCKS_PER_SEC pernoyme xrono
	std::cout << "Runtime of homemade Bellman-Ford is: " << duration*1000.0 << " ms \n";

	return 0;
}
