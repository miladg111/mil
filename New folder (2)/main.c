#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "my_dll.h"
#include "my_graph.h"
#include "my_pq.h"
void calculate_shortest(graph_t* graph, graph_node_t* source) {
    if (graph == NULL || source == NULL) {
        return;
    }
    dll_node_t* node_iter;
    graph_node_t* u;
    graph_node_t* v;
    graph_edge_t* edge;
    for (node_iter = graph->nodes->head; node_iter != NULL; node_iter = node_iter->next) {
        u = (graph_node_t*)node_iter->data;
        u->cost = INT_MAX;
        u->from = NULL;
    }
    source->cost = 0;
    pq_t* pq = create_pq();
    for (node_iter = graph->nodes->head; node_iter != NULL; node_iter = node_iter->next) {
        u = (graph_node_t*)node_iter->data;
        pq_enqueue(pq, u);
    }
    while (!pq_is_empty(pq)) {
        u = (graph_node_t*)pq_dequeue_min(pq);
        if (u->cost == INT_MAX) break;

        dll_node_t* edge_iter;
        for (edge_iter = u->neighbors->head; edge_iter != NULL; edge_iter = edge_iter->next) {
            edge = (graph_edge_t*)edge_iter->data;
            v = edge->dest;
            int alt = u->cost + edge->weight;
            if (alt < v->cost) {
                v->cost = alt;
                v->from = u;
                pq_decrease_key(pq, v);
            }
        }
    }
    delete_pq(pq);
}

int main() {
    // Read data from datafile city.dat
    FILE* city_data = fopen("city.dat", "r");
    if (!city_data) {
        perror("Error opening city.dat");
        return EXIT_FAILURE;
    }
    // Build an adjacency matrix based on the city.dat datafile
    graph_t* graph = create_graph();
    char city1[32], city2[32];
    int distance;
    while (fscanf(city_data, "%s %s %d", city1, city2, &distance) == 3) {
        graph_node_t* node1 = add_node(graph, strdup(city1));
        graph_node_t* node2 = add_node(graph, strdup(city2));
        add_edge(node1, node2, distance);
    }
    fclose(city_data);
    // Main loop to interact with the user
    while (true) {
        printf("Enter the source city: ");
        scanf("%s", city1);

        if (strcmp(city1, "exit") == 0) {
            break;
        }
        printf("Enter the destination city: ");
        scanf("%s", city2);

        if (strcmp(city2, "exit") == 0) {
            break;
        }
        graph_node_t* source = find_node_by_name(graph, city1);
        graph_node_t* destination = find_node_by_name(graph, city2);

        if (source == NULL || destination == NULL) {
            printf("Invalid city names. Please try again.\n");
        } else {
            calculate_shortest(graph, source);
            printf("The shortest distance between %s and %s is %d.\n", city1, city2, destination->cost);
            print_shortest_path(source, destination);
        }
    }
    delete_graph(graph);
    return EXIT_SUCCESS;
}