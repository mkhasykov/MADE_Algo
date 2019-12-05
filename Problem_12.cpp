/*
Мосты.
Ребро неориентированного графа называется мостом, если удаление этого ребра из графа увеличивает число компонент связности.
Дан неориентированный граф, требуется найти в нем все мосты.
*/

#include<fstream>
#include<vector>
#include<algorithm>
#include<assert.h>

using std::vector;
using std::min;
using std::endl;

class Graph {
public:

	Graph(const unsigned int& n);
	~Graph();

	void AddEdge(const unsigned int& from, const unsigned int& to, const unsigned int& num);
	void FindBridges();
	unsigned int GetBridgesNum() const;
	vector<int>GetBridgesNums() const;

private:
	unsigned int numVertices;
	int timer;
	vector<vector<unsigned int>> edges;
	vector<vector<unsigned int>> edges_nums;
	vector<bool> visited;
	vector<int> entry, lowest;
	vector<int> bridges_nums;

	void DFS(unsigned int vertex, int parent, int num_prev);
};

Graph::Graph(const unsigned int& n) {
	numVertices = n;
	edges = vector<vector<unsigned int>>(n);
	edges_nums = vector<vector<unsigned int>>(n);
	timer = 0;
}

Graph::~Graph() {
}

void Graph::AddEdge(const unsigned int& from, const unsigned int& to, const unsigned int& num) {
	assert (from < numVertices);
	assert (to < numVertices);

	edges[from].push_back(to);
	edges_nums[from].push_back(num);

	edges[to].push_back(from);
	edges_nums[to].push_back(num);
}

void Graph::DFS(unsigned int vertex, int parent = -1, int num_prev = -1) {

    visited[vertex] = true;
    entry[vertex] = lowest[vertex] = timer++;

    for (unsigned int i=0; i < edges[vertex].size(); ++i) {
    	int to = edges[vertex][i];
    	int num_curr = edges_nums[vertex][i];
        if ((to == parent) && (num_curr == num_prev)) continue;
        if (visited[to]) {
        	lowest[vertex] = min(lowest[vertex], entry[to]);
        } else {
            DFS(to, vertex, num_curr);
            lowest[vertex] = min(lowest[vertex], lowest[to]);
            if (lowest[to] > entry[vertex])
            	bridges_nums.push_back(num_curr);
        }
    }
}

void Graph::FindBridges() {

    timer = 0;
    visited = vector<bool>(numVertices, false);
    entry = vector<int>(numVertices, -1);
    lowest = vector<int>(numVertices, -1);
    bridges_nums.clear();

    for (unsigned int i = 0; i < numVertices; ++i) {
        if (!visited[i]) DFS(i);
    }

    std::sort(bridges_nums.begin(), bridges_nums.end());
}

unsigned int Graph::GetBridgesNum() const {
	return bridges_nums.size();
}

vector<int> Graph::GetBridgesNums() const {
	vector<int> result(bridges_nums);
	return result;
}

int main() {

	std::ifstream infile("bridges.in");
	std::ofstream outfile("bridges.out");

	unsigned int n, m;
	infile >> n >> m;
	assert (m <= n*(n-1)/2);

	Graph graph(n);

	unsigned int from;
	unsigned int to;
	for (unsigned int i = 0; i < m; i++) {
		infile >> from >> to;
		graph.AddEdge(from-1, to-1, i+1);
	}

	graph.FindBridges();

	outfile << graph.GetBridgesNum() << endl;
	vector<int> bridges_nums = graph.GetBridgesNums();
	for (unsigned int i=0; i < bridges_nums.size(); ++i) {
		outfile << bridges_nums[i] << endl;
	}

	infile.close();
	outfile.close();
	return 0;
}
