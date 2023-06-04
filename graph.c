#include "graph.h"
#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>


/* initialise an empty graph */
/* return pointer to initialised graph */
Graph *init_graph(void)
{
    Graph *graph = initialise_linked_list();
    if(graph == NULL) {
        fprintf(stderr,"Error: failed to initialise graph.\n");
    }
    return graph;
}

/* release memory for graph */
void free_graph(Graph *graph)
{
  Node* curr;
    if(graph == NULL) {
        fprintf(stderr,"Error: failed to free graph.\n");
        return;
    }

    curr = graph->head;
    while(curr != NULL) {
        free_vertex(curr->data);
        curr = curr->next;
    }

    free_linked_list(graph);
}

/* initialise a vertex */
/* return pointer to initialised vertex */
Vertex *init_vertex(int id)
{
    Vertex *vertex = malloc(sizeof(Vertex));
    if(vertex == NULL) {
        fprintf(stderr,"Error: failed to initialise vertex.\n");
        return NULL;
    }
    vertex->id = id;
    vertex->edges = initialise_linked_list();
    if(vertex->edges == NULL) {
        fprintf(stderr,"Error: failed to initialise vertex edges.\n");
        return NULL;
    }
    return vertex;
}
/* release memory for initialised vertex */
void free_vertex(Vertex *vertex)
{
    Node* curr;
    if(vertex == NULL) {
        fprintf(stderr,"Error: failed to free vertex.\n");
        return;
    }
    curr = vertex->edges->head;
    while(curr != NULL) {
        free_edge(curr->data);
        curr = curr->next;
    }
    free_linked_list(vertex->edges);
    free(vertex);
}

/* initialise an edge. */
/* return pointer to initialised edge. */
Edge *init_edge(void)
{
    Edge *edge = malloc(sizeof(Edge));
    if(edge == NULL) {
        fprintf(stderr,"Error: failed to initialise edge.\n");
        return NULL;
    }
    edge->vertex = NULL;
    edge->weight = 0.00;
    return edge;
}

/* release memory for initialised edge. */
void free_edge(Edge *edge)
{
    if(edge == NULL) {
        fprintf(stderr,"Error: failed to free edge.\n");
        return;
    }
    free(edge);
}


/* remove all edges from vertex with id from to vertex with id to from graph. */
void remove_edge(Graph *graph, int from, int to) {
    Node* curr;
    Node* prev; 
    Node* tmp;
    Vertex *from_vertex = find_vertex(graph, from);
    Vertex *to_vertex = find_vertex(graph, to);

    if(from_vertex == NULL || to_vertex == NULL) {
        fprintf(stderr,"Error: failed to remove edge. Could not find one or both vertices.\n");
        return;
    }

    prev = NULL;
    curr = from_vertex->edges->head;

    while(curr != NULL) {
        Edge *edge = curr->data;
        if(edge->vertex == to_vertex) {
            if(prev != NULL) {
                prev->next = curr->next;
            } else {
                from_vertex->edges->head = curr->next;
            }
            tmp = curr->next;
            free_edge(edge);
            free(curr);
            curr = tmp;
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
}


/* remove all edges from vertex with specified id. */
void remove_edges(Graph *graph, int id)
{
	Edge *edge;
    Vertex *vertex = find_vertex(graph, id);
    if(vertex == NULL) {
        fprintf(stderr,"Error: failed to remove edges.\n");
        return;
    }
    while(vertex->edges->head != NULL) {
        edge = vertex->edges->head->data;
        free_edge(edge);
        remove_head_linked_list(vertex->edges);
    }
}

/* output all vertices and edges in graph. */
/* each vertex in the graphs should be printed on a new line */
/* each vertex should be printed in the following format: */
/* vertex_id: edge_to_vertex[weight] edge_to_vertex[weight] ... */
/* for example: */
/* 1: 3[1.00] 5[2.00] */
/* indicating that vertex id 1 has edges to vertices 3 and 5 */
/* with weights 1.00 and 2.00 respectively */
/* weights should be output to two decimal places */
void print_graph(Graph *graph)
{
    Node *curr;
     Node *edge_curr;
    if(graph == NULL) {
        fprintf(stderr,"Error: failed to print graph.\n");
        return;
    }
    curr = graph->head;
   
    
    while(curr != NULL) {
        Vertex *vertex = curr->data;
        printf("%d: ", vertex->id);
        edge_curr = vertex->edges->head;
        while(edge_curr != NULL) {
            Edge *edge = edge_curr->data;
            printf("%d[%.2f] ", edge->vertex->id, edge->weight);
            edge_curr = edge_curr->next;
        }
        printf("\n");
        curr = curr->next;
    }
}

/* find vertex with specified id in graph. */
/* return pointer to vertex, or NULL if no vertex found. */

Vertex *find_vertex(Graph *graph, int id)
{
    Node *curr;
    if(graph == NULL) {
        fprintf(stderr,"Error: failed to find vertex.\n");
        return NULL;
    }
    curr = graph->head;
    while(curr != NULL) {
        Vertex *vertex = curr->data;
        if(vertex->id == id) {
            return vertex;
        }
        curr = curr->next;
    }
    return NULL;
}

/* create and add vertex with specified id to graph. */
/* return pointer to vertex or NULL if an error occurs. */
/* if vertex with id already exists, return pointer to existing vertex. */
Vertex *add_vertex(Graph *graph, int id)
{
    Vertex *vertex;
    if(graph == NULL) {
        fprintf(stderr,"Error: failed to add vertex.\n");
        return NULL;
    }
    vertex = find_vertex(graph, id);
    if(vertex != NULL) {
        return vertex;
    }
    vertex = init_vertex(id);
    append_linked_list(graph, vertex);
    return vertex;
}

/* remove vertex with specified id from graph. */
/* remove all edges between specified vertex and any other vertices in graph. */

void remove_vertex(Graph *graph, int id) {
    Vertex *vertex;
    Node *curr;
    Node *prev;
    if(graph == NULL) {
        fprintf(stderr,"Error: failed to remove vertex.\n");
        return;
    }
    vertex = find_vertex(graph, id);
    if(vertex == NULL) {
        fprintf(stderr, "Error: vertex does not exist.\n");
        return;
    }

    curr = graph->head;
    while(curr != NULL) {
        Vertex *vertex_curr = curr->data;
        remove_edge(graph, vertex_curr->id, vertex->id);
        curr = curr->next;
    }
    
    while(vertex->edges->head != NULL) {
        Edge *edge = vertex->edges->head->data;
        free_edge(edge);
        remove_head_linked_list(vertex->edges);
    }

    if(graph->head->data == vertex) {
        curr = graph->head;
        graph->head = graph->head->next;
    } else {
        prev = graph->head;
        curr = graph->head->next;
        while(curr != NULL) {
            if(curr->data == vertex) {
                prev->next = curr->next;
                break;
            }
            prev = curr;
            curr = curr->next;
        }
    }
    free_vertex(vertex);
    free(curr);
}


/* add directed edge with specified weight between vertex with id from */
/* to vertex with id to. */
/* if no vertices with specified ids (from or to) exist */
/* then the vertices will be created. */
/* multiple edges between the same pair of vertices are allowed. */
/* return pointer to edge, or NULL if an error occurs found. */
Edge *add_edge(Graph *graph, int from, int to, double weight)
{
    Vertex *from_vertex;
    Vertex *to_vertex;
    Edge *edge;
    if(graph == NULL) {
        fprintf(stderr,"Error: failed to add edge.\n");
        return NULL;
    }
    from_vertex = add_vertex(graph, from);
    to_vertex = add_vertex(graph, to);

    edge = init_edge();
    edge->vertex = to_vertex;
    edge->weight = weight;

    append_linked_list(from_vertex->edges, edge);
    return edge;
}

/* add two edges to graph, one from vertex with id from to vertex with id to, */
/* and one from vertex with id to to vertex with id from. */
/* both edges should have the same weight */
/* if no vertices with specified ids (from or to) exist */
/* then the vertices will be created. */
/* multiple vertices between the same pair of vertices are allowed. */

void add_edge_undirected(Graph *graph, int from, int to, double weight)
{
    Vertex *from_vertex;
    Vertex *to_vertex;
    Edge *edge1,*edge2;
    if(graph == NULL) {
        fprintf(stderr,"Error: failed to add edge undirected.\n");
        return;
    }
    from_vertex = add_vertex(graph, from);
    to_vertex = add_vertex(graph, to);
    

    edge1 = init_edge();
    edge1->vertex = to_vertex;
    edge1->weight = weight;
    append_linked_list(from_vertex->edges, edge1);

    edge2 = init_edge();
    edge2->vertex = from_vertex;
    edge2->weight = weight;
    append_linked_list(to_vertex->edges, edge2);

}
/* return array of node ids in graph. */
/* array of node ids should be dynamically allocated */
/* set count to be the number of nodes in graph */
/* return NULL if no vertices in graph */
int *get_vertices(Graph *graph, int *count)
{
    int num_vertices = 0;
    int *vertices;
    int i = 0;
    Node *curr;
    if(graph == NULL) {
        fprintf(stderr,"Error: failed to get vertex.\n");
        return NULL;
    }
    
   curr = graph->head;
    while(curr != NULL) {
        num_vertices++;
        curr = curr->next;
    }

    if(num_vertices == 0) {
        return NULL;
    }

    vertices = malloc(num_vertices * sizeof(int));
    
    curr = graph->head;
    while(curr != NULL) {
        Vertex *vertex = curr->data;
        vertices[i] = vertex->id;
        i++;
        curr = curr->next;
    }

    *count = num_vertices;
    return vertices;
}

Edge **get_edges(Graph *graph, Vertex *vertex, int *count)
{
    int num_edges = 0;
    Edge **edges;
    int i = 0;
    Node *curr;
    if(graph == NULL) {
        fprintf(stderr,"Error: failed to get edges.\n");
        return NULL;
    }
    
    curr = vertex->edges->head;
    while(curr != NULL) {
        num_edges++;
        curr = curr->next;
    }

    if(num_edges == 0) {
        return NULL;
    }

    edges = malloc(num_edges * sizeof(Edge*));
    curr = vertex->edges->head;
    while(curr != NULL) {
        edges[i] = curr->data;
        i++;
        curr = curr->next;
    }

    *count = num_edges;
    return edges;
}

Edge *get_edge(Graph *graph, int from, int to)
{
    Vertex *from_vertex;
    Vertex *to_vertex;
    Node *curr;
    Edge *edge;
    if(graph == NULL) {
        fprintf(stderr,"Error: failed to get edge.\n");
        return NULL;
    }
    from_vertex = find_vertex(graph, from);
    if(from_vertex == NULL) {
        fprintf(stderr,"Error: failed to get edge.\n");
        return NULL;
    }
    to_vertex = find_vertex(graph, to);
    if(to_vertex == NULL) {
        fprintf(stderr,"Error: failed to get edge.\n");
        return NULL;
    }
    curr = from_vertex->edges->head;

    while(curr != NULL) {
        edge = curr->data;
        if(edge->vertex == to_vertex) {
            return edge;
        }
        curr = curr->next;
    }
    return NULL;
}

/* return id of destination node of edge. */
int edge_destination(Edge *edge)
{
    if (edge == NULL) {
        fprintf(stderr, "Error: NULL edge given to edge_destination\n");
        return -1;
    }

    return edge->vertex->id;
}

/* return weight of edge. */
double edge_weight(Edge *edge)
{
    if (edge == NULL) {
        fprintf(stderr, "Error: NULL edge given to edge_weight\n");
        return -1;
    }

    return edge->weight;
}
