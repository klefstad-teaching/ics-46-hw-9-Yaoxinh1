#include "dijkstras.h"
#include <queue>
#include <iostream>


vector<int> dijkstra_shortest_path(const Graph& G, int source, vector<int>& previous)
{
    int n = G.numVertices;
    vector<int> distance(n, INF);
    vector<bool> visited(n, false);


    using DistVertex = pair<int,int>;
    priority_queue<DistVertex, vector<DistVertex>, greater<DistVertex>> pq;

    
    distance[source] = 0;
    previous.assign(n, -1); 
    pq.push({0, source});

    while (!pq.empty()) {
        auto [distU, u] = pq.top();
        pq.pop();

        if (visited[u]) continue;
        visited[u] = true;

        
        for (auto& edge : G[u]) {
            int v = edge.dst;
            int weight = edge.weight;

            if (!visited[v] && distance[u] != INF && distance[u] + weight < distance[v]) {
                distance[v] = distance[u] + weight;
                previous[v] = u;
                pq.push({distance[v], v});
            }
        }
    }

    return distance;
}


vector<int> extract_shortest_path(const vector<int>& distances, const vector<int>& previous, int destination)
{
    vector<int> path;
    
    if (destination < 0 || destination >= (int)distances.size() || distances[destination] == INF) {
       
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
        cout << "No path found\n";
        return;
    }
    for (size_t i = 0; i < v.size(); i++) {
        cout << v[i];
        if (i + 1 < v.size()) cout << " ";
    }
    cout << "\nTotal cost is " << total << "\n\n";
}
