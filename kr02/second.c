#include "dlist.h"
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node *push(Node *place, int value) {
    if (!place) {
        Node *tmp = calloc(1, sizeof(*tmp));
        if (!tmp) {
            return NULL;
        }
        tmp->value = value;
        tmp->next = NULL;
        tmp->prev = NULL;
        return tmp;
    }
    if (!place->prev) {
        place->prev = calloc(1, sizeof(*(place->prev)));
        if (place->prev == NULL) {
            return NULL;
        }
        place->prev->next = place;
        place->prev->prev = NULL;
        place->prev->value = value;
        return place->prev;
    } else {
        Node *tmp = calloc(1, sizeof(*tmp));
        if (!tmp) {
            return NULL;
        }
        tmp->value = value;
        place->prev->next = tmp;
        tmp->prev = place->prev;
        tmp->next = place;
        place->prev = tmp;
        return tmp;
    }
}

int pop(Node *node) {
    if (node->next && node->prev) {
        node->next->prev = node->prev;
        node->prev->next = node->next;
    } else if (node->next) {
        node->next->prev = NULL;
    } else if (node->prev) {
        node->prev->next = NULL;
    }
    int tmp = node->value;
    free(node);
    return tmp;
}

// int main(void) {
//     Node * head = NULL;
//     head = push(head, 1);
//     head = push(head, 2);
//     pop(head->next) == 1;
//     pop(head) == 2;
//     return 0;
// }