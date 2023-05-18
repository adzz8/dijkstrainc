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


void remove_edge(Graph *graph, int from, int to) {
    Node* curr;
    Node* prev; 
    Node* tmp;
    Vertex *from_vertex = find_vertex(graph, from);

    if(from_vertex == NULL) {
        fprintf(stderr,"Error: failed to remove edge. Could not find the vertex.\n");
        return;
    }

    prev = NULL;
    curr = from_vertex->edges->head;

    while(curr != NULL) {
        Edge *edge = curr->data;
        if(edge->vertex->id == to) {
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

void remove_edges(Graph *graph, int id)
{
    Vertex *vertex = find_vertex(graph, id);
    if(vertex == NULL) {
        fprintf(stderr,"Error: failed to remove edges.\n");
        return;
    }
    while(vertex->edges->head != NULL) {
        Node *curr = vertex->edges->head;
        vertex->edges->head = curr->next;
        free_edge((Edge*)curr->data);
        free(curr);
    }
}

Vertex *add_vertex(Graph *graph, int id)
{
    Vertex *vertex = find_vertex(graph, id);
    if(vertex != NULL) {
        return vertex;
    }
    vertex = init_vertex(id);
    if(graph->head == NULL) {
        graph->head = vertex;
    } else {
        Vertex *curr = graph->head;
        while(curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = vertex;
    }
    return vertex;
}

void remove_vertex(Graph *graph, int id) {
    Vertex *prev = NULL;
    Vertex *curr = graph->head;
    while(curr != NULL) {
        if(curr->id == id) {
            if(prev != NULL) {
                prev->next = curr->next;
            } else {
                graph->head = curr->next;
            }
            remove_edges(graph, id);
            free_vertex(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    fprintf(stderr, "Error: vertex does not exist.\n");
}

Edge *add_edge(Graph *graph, int from, int to, double weight)
{
    Vertex *from_vertex = add_vertex(graph, from);
    Vertex *to_vertex = add_vertex(graph, to);

    Edge *edge = init_edge();
    edge->vertex = to_vertex;
    edge->weight = weight;

    if(from_vertex->edges->head == NULL) {
        from_vertex->edges->head = edge;
    } else {
        Edge *curr = from_vertex->edges->head;
        while(curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = edge;
    }
    return edge;
}

void add_edge_undirected(Graph *graph, int from, int to, double weight)
{
    add_edge(graph, from, to, weight);
    add_edge(graph, to, from, weight);
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
