#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "dijkstra.h"

int main(void) {
    Graph *graph = init_graph();
    
    add_edge(graph, 0, 1, 4);
    add_edge(graph, 0, 2, 1);
    add_edge(graph, 2, 1, 2);
    add_edge(graph, 1, 3, 1);
    add_edge(graph, 2, 3, 5);

    int numVertices = 0;
    Path *paths = dijkstra(graph, 0, &numVertices);
    
    for (int i = 0; i < numVertices; ++i) {
        if (paths[i].next_hop != -1) {
            printf("Shortest path from node 0 to node %d has weight %.2f and next hop %d.\n", i, paths[i].weight, paths[i].next_hop);
        }
    }

    free(paths);
    free_graph(graph);

    graph = init_graph();
    add_edge(graph, 0, 1, 2);
    add_edge(graph, 0, 2, 6);
    add_edge(graph, 1, 2, 3);
    add_edge(graph, 1, 3, 7);
    add_edge(graph, 1, 4, 5);
    add_edge(graph, 2, 4, 1);
    add_edge(graph, 2, 3, 1);
    add_edge(graph, 3, 4, 5);

    numVertices = 0;
    paths = dijkstra(graph, 0, &numVertices);
    
    for (int i = 0; i < numVertices; ++i) {
        if (paths[i].next_hop != -1) {
            printf("Shortest path from node 0 to node %d has weight %.2f and next hop %d.\n", i, paths[i].weight, paths[i].next_hop);
        }
    }

    free(paths);
    free_graph(graph);

    return 0;
}
