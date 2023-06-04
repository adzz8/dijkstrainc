#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "graph.h"
#include "dijkstra.h"

/* find shortest paths between source node id and all other nodes in graph. */
/* upon success, returns an array containing a table of shortest paths.  */
/* return NULL if *graph is uninitialised or an error occurs. */
/* each entry of the table array should be a Path */
/* structure containing the path information for the shortest path between */
/* the source node and every node in the graph. If no path exists to a */
/* particular desination node, then next should be set to -1 and weight */
/* to DBL_MAX in the Path structure for this node */

 Path *dijkstra(Graph *graph, int id, int *pnEntries)
{
    Path *table = NULL;
    int max = 0;
    int i, x;
    int current_node = 0;
    double current_weight = DBL_MAX;
    Edge* temp_edge;
    int curr,prev;

    typedef struct table_record{
        int Visited;
        int Node;
        int prev_Node;
        double weight;
    }table_record;
    table_record* record;
   
    for(i = 0; i < 256; i++){
        if(find_vertex(graph,i)){
            if( i > max){
                max = i;
            }
        }
    }
    max++;

    table = (Path*)calloc(max, sizeof(Path));
    record = calloc(max, sizeof(table_record));

    if(!find_vertex(graph, id)){
        fprintf(stderr, "ERROR: NOT VALID\n");
        return NULL;
    }

    for(i = 0; i < max; i++){
        if(find_vertex(graph,i)){
            record[i].Node = i;
            if(record[i].Node == id){
                record[i].Visited = 1;
                record[i].weight = DBL_MAX;
                record[i].prev_Node = -1;
                table[i].next_hop = -1;
                table[i].weight = DBL_MAX;
            }
            else{
                temp_edge = get_edge(graph, id , i);
                if(!temp_edge){
                    record[i].weight = DBL_MAX;
                    record[i].prev_Node = -1;
                    record[i].Visited = 0;

                    table[i].next_hop = -1;
                    table[i].weight = DBL_MAX;

                }
                if(temp_edge){
                    record[i].weight = temp_edge->weight;
                    record[i].prev_Node = id;
                    record[i].Visited = 0;
                    table[i].next_hop = i;
                    table[i].weight = temp_edge->weight;
                }
            }
        }
    }

    for(x = 0; x < max; x++){
        for(i = 0; i < max; i++){
            if(find_vertex(graph, i)){
                if(!record[i].Visited){
                    if(record[i].weight < current_weight){
                        current_weight = record[i].weight;
                        current_node = record[i].Node;
                    }
                }
            }
        }
        for(i = 0; i < max; i++){
            if(!record[i].Visited){
                
                temp_edge = get_edge(graph, current_node, i);
                if(temp_edge){
                    current_weight = record[current_node].weight;
                    current_weight+= temp_edge->weight;
                    if(current_weight < record[i].weight){
                        record[i].weight = current_weight;
                        table[i].weight = current_weight;
                        record[i].prev_Node = current_node;
                        table[i].next_hop = current_node;
                        
                    }
                }
            }
        }
        record[current_node].Visited = 1;
        current_weight = DBL_MAX; 
    }

    for(i = 0; i < max; i++){
        if(record[i].Node != 0){
            if(record[i].prev_Node != -1){
                curr = record[i].Node;
                prev = record[i].prev_Node;
                while(prev != id){
                    curr = prev;
                    prev = record[prev].prev_Node;
                }
                table[i].next_hop = curr;
            }
        }
    }
    *pnEntries = max;
    free(record);
    return table;
}