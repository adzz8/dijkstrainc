#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "graph.h"
#include "dijkstra.h"

Path *dijkstra(Graph *graph, int id, int *pnEntries)
{
    int max = 0;
    int i, current_node = -1;
    double current_weight;
    Edge* temp_edge;

    typedef struct table_record{
        int Visited;
        int Node;
        int prev_Node;
        double weight;
    }table_record;

    table_record* record;

    for(i = 0; i < 256; i++) {
        if(find_vertex(graph,i)) {
            max = (i > max) ? i : max;
        }
    }
    max++;

    Path *table = (Path*)calloc(max, sizeof(Path));	
    record = calloc(max, sizeof(table_record));

    if(!find_vertex(graph, id)) {
        fprintf(stderr, "ERROR: NOT VALID\n");
        return NULL;
    }

    for(i = 0; i < max; i++) {
        if(find_vertex(graph,i)) {
            record[i].Node = i;
            record[i].prev_Node = -1;
            record[i].Visited = 0;
            record[i].weight = DBL_MAX;

            table[i].next_hop = -1;
            table[i].weight = DBL_MAX;

            if(i == id) {
                record[i].Visited = 1;
            } else {
                temp_edge = get_edge(graph, id , i);
                if(temp_edge){
                    record[i].weight = temp_edge->weight;
                    record[i].prev_Node = id;

                    table[i].next_hop = i;
                    table[i].weight = temp_edge->weight;
                }
            }
        }
    }

    for(int x = 0; x < max; x++) {
        current_weight = DBL_MAX;
        for(i = 0; i < max; i++) {
            if(find_vertex(graph, i) && !record[i].Visited && record[i].weight < current_weight) {
                current_weight = record[i].weight;
                current_node = record[i].Node;
            }
        }

        if(current_node != -1) {
            for(i = 0; i < max; i++) {
                if(!record[i].Visited) {
                    temp_edge = get_edge(graph, current_node, i);
                    if(temp_edge) {
						current_weight = record[current_node].weight + temp_edge->weight;
						if(current_weight < record[i].weight) {
							record[i].weight = current_weight;
							record[i].prev_Node = current_node;
							table[i].weight = current_weight;
							int prev_node = current_node;
							while(record[prev_node].prev_Node != id && record[prev_node].prev_Node != -1) {
								prev_node = record[prev_node].prev_Node;
							}
							table[i].next_hop = prev_node;
						}

                    }
                }
            }
            record[current_node].Visited = 1;
        }
    }

    *pnEntries = max;
    free(record);
    return table;
}
