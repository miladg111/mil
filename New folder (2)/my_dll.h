#ifndef MY_DLL_H
#define MY_DLL_H
#include <stdlib.h>

typedef struct dll_node {
    void* data;
    struct dll_node* prev;
    struct dll_node* next;
} dll_node_t;

typedef struct dll {
    dll_node_t* head;
    dll_node_t* tail;
    int size;
} dll_t;

dll_t* create_dll() {
    dll_t* list = (dll_t*)malloc(sizeof(dll_t));
    if (list == NULL) {
        return NULL;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
}
void delete_dll(dll_t* list) {
    if (list == NULL) {
        return;
    }

    dll_node_t* current = list->head;
    dll_node_t* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    free(list);
}
void dll_push_front(dll_t* list, void* data) {
    if (list == NULL) {
        return;
    }

    dll_node_t* new_node = (dll_node_t*)malloc(sizeof(dll_node_t));
    if (new_node == NULL) {
        return;
    }

    new_node->data = data;
    new_node->prev = NULL;
    new_node->next = list->head;

    if (list->head != NULL) {
        list->head->prev = new_node;
    } else {
        list->tail = new_node;
    }

    list->head = new_node;
    list->size++;
}
void dll_push_back(dll_t* list, void* data) {
    if (list == NULL) {
        return;
    }

    dll_node_t* new_node = (dll_node_t*)malloc(sizeof(dll_node_t));
    if (new_node == NULL) {
        return;
    }

    new_node->data = data;
    new_node->prev = list->tail;
    new_node->next = NULL;

    if (list->tail != NULL) {
        list->tail->next = new_node;
    } else {
        list->head = new_node;
    }

    list->tail = new_node;
    list->size++;
}
void* dll_pop_front(dll_t* list) {
    if (list == NULL || list->head == NULL) {
        return NULL;
    }

    dll_node_t* front_node = list->head;
    void* front_data = front_node->data;

    list->head = front_node->next;

    if (list->head != NULL) {
        list->head->prev = NULL;
    } else {
        list->tail = NULL;
    }

    free(front_node);
    list->size--;

    return front_data;
}
void* dll_pop_back(dll_t* list) {
    if (list == NULL || list->tail == NULL) {
        return NULL;
    }

    dll_node_t* back_node = list->tail;
    void* back_data = back_node->data;

    list->tail = back_node->prev;

    if (list->tail != NULL) {
        list->tail->next = NULL;
    } else {
        list->head = NULL;
    }

    free(back_node);
    list->size--;

    return back_data;
}
dll_node_t* dll_find(dll_t* list, void* data) {
    if (list == NULL || list->head == NULL) {
        return NULL;
    }

    dll_node_t* current = list->head;

    while (current != NULL) {
        if (current->data == data) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}
dll_node_t* dll_get_node(dll_t* list, int index) {
    if (list == NULL || index < 0 || index >= list->size) {
        return NULL;
    }

    dll_node_t* current = list->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }

    return current;
}
void dll_remove_node(dll_t* list, dll_node_t* node) {
    if (list == NULL || node == NULL) {
        return;
    }

    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }

    if (node->next != NULL) {
        node->next->prev = node->prev;
    } else {
        list->tail = node->prev;
    }

    free(node);
    list->size--;
}

#endif // MY_DLL_H