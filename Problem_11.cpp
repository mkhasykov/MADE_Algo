/*
Дан невзвешенный неориентированный граф. Найдите цикл минимальной длины.
Ввод: v: кол-во вершин (макс. 50000), n: кол-во ребер (макс. 200000), n пар реберных вершин.
Вывод: одно целое число равное длине минимального цикла. Если цикла нет, то вывести -1.
*/

#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
#include<assert.h>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::queue;
using std::min;

class Graph {
public:

	Graph(const unsigned int& v);
	~Graph();

	void AddEdge(const unsigned int& from, const unsigned int& to);
	int CalcMinCycleLen();

private:
	unsigned int numVertices;
	vector<vector<unsigned int>> edges;
};

Graph::Graph(const unsigned int& v) {
	numVertices = v;
	edges = vector<vector<unsigned int>>(v);
}

Graph::~Graph() {
}

void Graph::AddEdge(const unsigned int& from, const unsigned int& to) {
	assert (from < numVertices);
	assert (to < numVertices);

	if (find(edges[from].begin(), edges[from].end(), to) == edges[from].end())
		edges[from].push_back(to);
	if (find(edges[to].begin(), edges[to].end(), from) == edges[to].end())
		edges[to].push_back(from);
}

int Graph::CalcMinCycleLen() {

	int max_cycle_len = 1e6;
	int min_cycle_len = max_cycle_len; // To store length of the shortest cycle

	for (unsigned int i=0; i < numVertices; ++i) {
		queue<int> queue;
		vector<bool> visited(numVertices);
		vector<int> distances(numVertices), parents(numVertices);

		queue.push(i);
		visited[i] = true;
		parents[i] = -1;
		while (!queue.empty()) {
			int vertex = queue.front();
			queue.pop();
			for (unsigned int j=0; j < edges[vertex].size(); ++j) {
				int to = edges[vertex][j];
				if (!visited[to]) {
					visited[to] = true;
					queue.push(to);
					distances[to] = distances[vertex] + 1; // distance to this vertex
					parents[to] = vertex; // remember parent of this vertex
				} else if (parents[to]!=vertex && to!=parents[vertex]) {
					min_cycle_len = min(min_cycle_len, distances[vertex] + distances[to] + 1);
					break;
				}
			}
		}
	}

	if (min_cycle_len==max_cycle_len)
		return -1;

	return min_cycle_len;
}


int main() {

	unsigned int v;
	unsigned int n;
	cin >> v >> n;
	assert (n <= v*(v-1)/2);

	Graph graph(v);

	unsigned int from;
	unsigned int to;
	for (unsigned int i = 0; i < n; i++) {
		cin >> from >> to;
		graph.AddEdge(from, to);
	}

	cout << graph.CalcMinCycleLen();

	return 0;
}
