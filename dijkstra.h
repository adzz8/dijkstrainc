#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"

typedef struct PriorityQueue {
    int *nodes;
    double *weights;
    int size;
} PriorityQueue;

typedef struct Path {
    int next_hop;
    double weight;
} Path;

int ExtractMin(PriorityQueue *queue);
Path *dijkstra(Graph *graph, int source, int *pnEntries);

#endif /* DIJKSTRA_H */
