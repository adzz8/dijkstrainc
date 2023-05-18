#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "graph.h"
#include "dijkstra.h"
#include <limits.h>

typedef struct PriorityQueue {
    int *nodes;
    double *weights;
    int size;
} PriorityQueue;

int ExtractMin(PriorityQueue *queue) {
    int minIndex = 0;
    double minWeight = DBL_MAX;
    for (int i = 0; i < queue->size; ++i) {
        if (queue->weights[i] < minWeight) {
            minWeight = queue->weights[i];
            minIndex = i;
        }
    }
    int minNode = queue->nodes[minIndex];
    queue->nodes[minIndex] = queue->nodes[queue->size - 1];
    queue->weights[minIndex] = queue->weights[queue->size - 1];
    queue->size--;
    return minNode;
}

Path *dijkstra(Graph *graph, int source, int *pnEntries) {
    int maxNodeId = 0;
    for (int i = 0; i < 256; ++i) {
        if (find_vertex(graph, i) && i > maxNodeId) {
            maxNodeId = i;
        }
    }

    Path *paths = (Path*)calloc(maxNodeId + 1, sizeof(Path));
    PriorityQueue queue = {calloc(maxNodeId + 1, sizeof(int)), calloc(maxNodeId + 1, sizeof(double)), 0};

    for (int i = 0; i <= maxNodeId; ++i) {
        if (find_vertex(graph, i)) {
            paths[i].next_hop = -1;
            paths[i].weight = DBL_MAX;
            queue.nodes[queue.size] = i;
            queue.weights[queue.size] = i == source ? 0 : DBL_MAX;
            queue.size++;
        }
    }

    while (queue.size > 0) {
        int node = ExtractMin(&queue);
        if (paths[node].weight == DBL_MAX) break; 
        Edge *edge = graph->edges[node];
        while (edge) {
            double newWeight = paths[node].weight + edge->weight;
            if (newWeight < paths[edge->dest].weight) {
                paths[edge->dest].weight = newWeight;
                paths[edge->dest].next_hop = node == source ? edge->dest : paths[node].next_hop;
                for (int i = 0; i < queue.size; ++i) { 
                    if (queue.nodes[i] == edge->dest) {
                        queue.weights[i] = newWeight;
                        break;
                    }
                }
            }
            edge = edge->next;
        }
    }

    *pnEntries = maxNodeId + 1;
    free(queue.nodes);
    free(queue.weights);
    return paths;
}
