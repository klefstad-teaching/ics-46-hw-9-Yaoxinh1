#include "dijkstras.h"
#include <iostream>
#include <string>

using namespace std;


int main(int argc, char* argv[])
{
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    string filename = argv[1];
    Graph G;

    try {
        file_to_graph(filename, G);
    } catch (const exception& e) {
        cerr << "Error reading graph: " << e.what() << "\n";
        return 1;
    }

    vector<int> previous;
  
    vector<int> distances = dijkstra_shortest_path(G, 0, previous);


    for (int v = 0; v < G.numVertices; v++) {
        vector<int> path = extract_shortest_path(distances, previous, v);
       
        if (!path.empty()) {
        
            for (size_t i = 0; i < path.size(); i++) {
                cout << path[i];
                if (i + 1 < path.size()) cout << " ";
            }
            cout << "\nTotal cost is " << distances[v] << "\n\n";
        } else {
            cout << v << "\nNo path found\n\n";
        }
    }

    return 0;
}
