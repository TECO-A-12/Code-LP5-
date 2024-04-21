#include<iostream>
#include<omp.h>
#include<vector>
#include<queue>

using namespace std;

void parallelBFS(const vector<vector<int>> &graph, int startNode) {
    int numNodes = graph.size();
    vector<bool> visited(numNodes, false);
    vector<int> traversalOrder;

    queue<int> bfsqueue;
    bfsqueue.push(startNode);
    visited[startNode] = true;

    while (!bfsqueue.empty()) {

        int currNode = bfsqueue.front();
        bfsqueue.pop();

        #pragma omp parallel for schedule(dynamic) shared(bfsqueue, visited, traversalOrder)
        for (int neighbor : graph[currNode]) {

            #pragma omp critical 
            if (!visited[neighbor]) {
                #pragma omp task
                {
                    bfsqueue.push(neighbor);
                    visited[neighbor] = true;
                }
            }
        }

        // Store the current node in the traversal order
        #pragma omp critical
        traversalOrder.push_back(currNode);
    }

    // Print the BFS traversal order
    cout << "BFS Traversal Order: ";
    for (int node : traversalOrder) {
        cout << node << " ";
    }
    cout << endl;
}
int main() {
    int numNodes;
    cout << "Enter the number of nodes in the graph: ";
    cin >> numNodes;

    vector<vector<int>> graph(numNodes);
    cout << "Enter the adjacency list for each node:\n";
    for (int i = 0; i < numNodes; ++i) {
        cout << "Enter neighbors of node " << i << " (-1 to end): ";
        int neighbor;
        while (cin >> neighbor && neighbor != -1) {
            graph[i].push_back(neighbor);
        }
    }

    int startNode;
    cout << "Enter the start node: ";
    cin >> startNode;
    
    cout << "Parallel BFS traversal :" << endl;
    parallelBFS(graph, startNode);

    return 0;
}
int main() {
    int numNodes = 7;
    vector<vector<int>> graph ={{1, 2}, {0, 3, 4}, {0, 5, 6}, {1}, {1}, {2}, {2}};
    
    cout << "Parallel BFS traversal :" << endl;
    parallelBFS(graph, 0);

    return 0;
}
