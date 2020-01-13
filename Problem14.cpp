/*
14. «MST». Дан неориентированный связный граф. Требуется найти вес минимального остовного дерева в этом графе.
Вариант 1. С помощью алгоритма Прима.
*/

#include<fstream>
#include<iostream>
#include<vector>
#include<set>
#include<assert.h>

using std::cout;
using std::endl;
using std::vector;
using std::set;

const int Inf = 1e6;

template<class T>
class Graph {
public:

	Graph(const unsigned int& n);
	~Graph();

	void AddEdge(const int& from, const int& to, const T& weight);
	T Prim();

private:

	struct Edge {
	    T weight = Inf;
	    int to = -1;

	    Edge() = default;
	    Edge(const T& weight, const int& to) : weight(weight), to(to) {}

	    bool operator<(Edge const& another) const {
	    	if (weight != another.weight)
	    		return weight < another.weight;
	    	return to < another.to;
	    }
	};

	int numVertices;
	vector<vector<Edge>> edges;

};

template<class T>
Graph<T>::Graph(const unsigned int& n) {
	numVertices = n;
	edges = vector<vector<Edge>>(n);
}

template<class T>
Graph<T>::~Graph() {
}

template<class T>
void Graph<T>::AddEdge(const int& from, const int& to, const T& weight) {
	assert (from < numVertices);
	assert (to < numVertices);

	edges[from].push_back(Edge(weight, to));
	edges[to].push_back(Edge(weight, from));
}

template<class T>
T Graph<T>::Prim() {

    T mst_weight = 0;
    int num_selected = 0;
    vector<Edge> min_edges(numVertices); // smallest weight edge from vertex v to an already selected vertex
    min_edges[0].weight = 0;
    set<Edge> queue; // priority queue of edges to not yet selected vertices
    queue.insert(Edge(0, 0));
    vector<bool> selected(numVertices, false);

    while (num_selected < numVertices) {

    	while (selected[queue.begin()->to]) {
    		queue.erase(queue.begin());
    	}

        int vertex = queue.begin()->to;
        selected[vertex] = true;
        mst_weight += queue.begin()->weight;
        queue.erase(queue.begin());
        ++num_selected;

        for (Edge edge : edges[vertex]) {
            if (!selected[edge.to] && edge < min_edges[edge.to]) {
                min_edges[edge.to] = Edge(edge.weight, vertex);
                queue.insert(edge);
            }
        }
    }

    return mst_weight;
}


int main() {

	std::ifstream infile("kruskal.in");
	std::ofstream outfile("kruskal.out");

	unsigned int n, m;
	infile >> n >> m;
	assert (m <= n*(n-1)/2);

	Graph<int> graph(n);

	unsigned int b, e, w;
	for (unsigned int i = 0; i < m; i++) {
		infile >> b >> e >> w;
		graph.AddEdge(b-1, e-1, w);
	}

	outfile << graph.Prim() << endl;

	infile.close();
	outfile.close();

	return 0;
}
