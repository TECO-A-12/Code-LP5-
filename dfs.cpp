#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

void dfsHelper(const vector<vector<int>>& graph, int currentNode, vector<bool>& visited, vector<int>& traversalOrder) {
    visited[currentNode] = true;
    traversalOrder.push_back(currentNode);

    #pragma omp parallel for schedule(dynamic) shared(graph, visited, traversalOrder)
    for (int neighbor : graph[currentNode]) {
        #pragma omp critical
        {
            if (!visited[neighbor]) {
                #pragma omp task
                {
                    dfsHelper(graph, neighbor, visited, traversalOrder);
                }
            }
        }
    }
}

void parallelDFS(const vector<vector<int>>& graph, int startNode) {
    int numNodes = graph.size();
    vector<bool> visited(numNodes, false);
    vector<int> traversalOrder;

    #pragma omp parallel
    #pragma omp single nowait
    {
        dfsHelper(graph, startNode, visited, traversalOrder);
    }

    // Print the traversal order
    cout << "DFS Traversal Order: ";
    for (int node : traversalOrder) {
        cout << node << " ";
    }
    cout << endl;
}

int main() {
    int numNodes = 7;
    vector<vector<int>> graph = {{1, 2}, {0, 3, 4}, {0, 5, 6}, {1}, {1}, {2}, {2}};

    cout << "Parallel DFS traversal:" << endl;
    parallelDFS(graph, 0);

    return 0;
}
