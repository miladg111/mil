#ifndef MY_GRAPH_H
#define MY_GRAPH_H
#include <stdlib.h>
#include <string.h>
#include "my_dll.h"

typedef struct graph_node graph_node_t;
typedef struct graph_edge {
    graph_node_t* dest;
    int weight;
    struct graph_node* destination;
} graph_edge_t;

typedef struct graph_node {
    void* data;
    dll_t* neighbors;
    int cost; 
    struct graph_node* from;
    dll_t* edges;
} graph_node_t;

typedef struct graph {
    dll_t* nodes;
} graph_t;

graph_t* create_graph() {
    graph_t* graph = (graph_t*)malloc(sizeof(graph_t));
    if (graph == NULL) {
        return NULL;
    }

    graph->nodes = create_dll();
    if (graph->nodes == NULL) {
        free(graph);
        return NULL;
    }

    return graph;
}
void remove_edge(graph_node_t* source, graph_node_t* dest);
void delete_edge(graph_edge_t* edge);
void delete_graph(graph_t* g) {
    if (g == NULL) {
        return;
    }

    dll_node_t* current = g->nodes->head;
    dll_node_t* next;
    graph_node_t* graph_node;

    while (current != NULL) {
        graph_node = (graph_node_t*)current->data;
        delete_dll(graph_node->neighbors);
        next = current->next;
        free(graph_node);
        current = next;
    }

    delete_dll(g->nodes);
    free(g);
}
graph_node_t* add_node(graph_t* g, void* data) {
    if (g == NULL) {
        return NULL;
    }

    graph_node_t* new_node = (graph_node_t*)malloc(sizeof(graph_node_t));
    if (new_node == NULL) {
        return NULL;
    }

    new_node->data = data;
    new_node->neighbors = create_dll();
    if (new_node->neighbors == NULL) {
        free(new_node);
        return NULL;
    }

    dll_push_back(g->nodes, new_node);
    return new_node;
}
void add_edge(graph_node_t* source, graph_node_t* dest, int weight) {
    if (source == NULL || dest == NULL) {
        return;
    }

    graph_edge_t* new_edge = (graph_edge_t*)malloc(sizeof(graph_edge_t));
    if (new_edge == NULL) {
        return;
    }

    new_edge->dest = dest;
    new_edge->weight = weight;

    dll_push_back(source->neighbors, new_edge);
}
void remove_node(graph_t* g, graph_node_t* node) {
    if (g == NULL || node == NULL) {
        return;
    }

    // Remove edges connected to the node
    dll_node_t* current = g->nodes->head;
    graph_node_t* current_node;

    while (current != NULL) {
        current_node = (graph_node_t*)current->data;
        if (current_node != node) {
            remove_edge(current_node, node);
        }
        current = current->next;
    }

    // Delete the node's neighbors list
    delete_dll(node->neighbors);

    // Remove the node from the graph's nodes list
    dll_node_t* dll_node = dll_find(g->nodes, node);
    dll_remove_node(g->nodes, dll_node);

    // Free the node memory
    free(node);
}
void remove_edge(graph_node_t* source, graph_node_t* dest) {
    if (source == NULL || dest == NULL) {
        return;
    }

    dll_node_t* current = source->neighbors->head;
    graph_edge_t* edge;

    while (current != NULL) {
        edge = (graph_edge_t*)current->data;
        if (edge->dest == dest) {
            dll_remove_node(source->neighbors, current);
            delete_edge(edge);
            break;
        }
        current = current->next;
    }
}
graph_node_t* find_node_by_name(graph_t* graph, const char* name) {
    dll_node_t* current = graph->nodes->head;
    graph_node_t* node;

    while (current != NULL) {
        node = (graph_node_t*)current->data;
        if (strcmp((char*)node->data, name) == 0) {
            return node;
        }
        current = current->next;
    }

    return NULL;
}

void print_shortest_path(graph_node_t* source, graph_node_t* destination) {
    if (destination == NULL) {
        return;
    }

    if (destination == source) {
        printf("%s", (char*)source->data);
    } else {
        print_shortest_path(source, destination->from);
        printf(" -> %s", (char*)destination->data);
    }
}
void delete_edge(graph_edge_t* edge) {
    if (edge != NULL) {
        free(edge);
    }
}

#endif // MY_GRAPH_H