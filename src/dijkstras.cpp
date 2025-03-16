#include "dijkstras.h"
#include <queue>
#include <iostream>
#include <algorithm> 

using namespace std;

vector<int> dijkstra_shortest_path(const Graph& G, int source, vector<int>& previous)
{
    int n = G.numVertices;
    vector<int> distance(n, INF);
    vector<bool> visited(n, false);

    
    using DistVert = pair<int,int>;
    priority_queue<DistVert, vector<DistVert>, greater<DistVert>> pq;

    distance[source] = 0;
    previous.assign(n, -1);

    pq.push({0, source});

    while (!pq.empty()) {
        auto [distU, u] = pq.top();
        pq.pop();

        if (visited[u]) continue;
        visited[u] = true;

        
        for (auto &edge : G[u]) {
            int v = edge.dst;
            int w = edge.weight;
            if (!visited[v] && distance[u] != INF && distance[u] + w < distance[v]) {
                distance[v] = distance[u] + w;
                previous[v] = u;
                pq.push({distance[v], v});
            }
        }
    }

    return distance;
}

vector<int> extract_shortest_path(const vector<int>& distances,
                                  const vector<int>& previous,
                                  int destination)
{
    vector<int> path;
    if (destination < 0 || destination >= (int)distances.size())
        return path;
    if (distances[destination] == INF) {
       
        return path;
    }
   
    for (int v = destination; v != -1; v = previous[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());
    return path;
}


void print_path(const vector<int>& v, int total)
{
    if (v.empty()) {
       
        cout << "\nTotal cost is " << total << "\n";
        return;
    }

    for (int vertex : v) {
        cout << vertex << " ";
    }
    cout << "\nTotal cost is " << total << "\n";
}
