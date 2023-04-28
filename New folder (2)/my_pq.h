#ifndef MY_PQ_H
#define MY_PQ_H
#include <stdlib.h>
#include "my_dll.h"
#include "my_graph.h"


typedef struct pq {
    dll_t* list;
    int (*compare)(void*, void*);
} pq_t;

pq_t* create_pq() {
    pq_t* pq = (pq_t*)malloc(sizeof(pq_t));
    if (pq == NULL) {
        return NULL;
    }

    pq->list = create_dll();
    if (pq->list == NULL) {
        free(pq);
        return NULL;
    }

    return pq;
}
int graph_node_compare(void* a, void* b) {
    graph_node_t* node_a = (graph_node_t*)a;
    graph_node_t* node_b = (graph_node_t*)b;

    return node_a->cost - node_b->cost;
}
int find_index(pq_t* pq, void* item) {
    dll_node_t* current = pq->list->head;
    int index = 0;

    while (current != NULL) {
        if (current->data == item) {
            return index;
        }
        current = current->next;
        index++;
    }

    return -1;
}
void swap(pq_t* pq, int index1, int index2) {
    dll_node_t* node1 = dll_get_node(pq->list, index1);
    dll_node_t* node2 = dll_get_node(pq->list, index2);
    void* temp = node1->data;
    node1->data = node2->data;
    node2->data = temp;
}
void bubble_up(pq_t* pq, int index) {
    if (index == 0) return;
    int parent_index = (index - 1) / 2;

    if (pq->compare(dll_get_node(pq->list, index)->data, dll_get_node(pq->list, parent_index)->data) < 0) {
        swap(pq, index, parent_index);
        bubble_up(pq, parent_index);
    }
}
void pq_decrease_key(pq_t* pq, void* item) {
    int index = find_index(pq, item);
    if (index != -1) {
        bubble_up(pq, index);
    }
}

void delete_pq(pq_t* pq) {
    if (pq == NULL) {
        return;
    }

    delete_dll(pq->list);
    free(pq);
}
void pq_enqueue(pq_t* pq, void* data) {
    if (pq == NULL) {
        return;
    }

    dll_push_back(pq->list, data);
}
void* pq_dequeue_min(pq_t* pq) {
    if (pq == NULL || pq->list->head == NULL) {
        return NULL;
    }

    dll_node_t* current = pq->list->head;
    dll_node_t* min_node = current;
    graph_node_t* current_data;
    graph_node_t* min_data = (graph_node_t*)min_node->data;

    while (current != NULL) {
        current_data = (graph_node_t*)current->data;
        if (current_data->cost < min_data->cost) {
            min_node = current;
            min_data = current_data;
        }
        current = current->next;
    }

    dll_remove_node(pq->list, min_node);
    return min_data;
}

int pq_is_empty(pq_t* pq) {
    if (pq == NULL || pq->list->head == NULL) {
        return 1;
    }

    return 0;
}

#endif // MY_PQ_H