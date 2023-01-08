#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct List {
    char data;
    struct List *next;
} List;

void append(List **list, char key) {
    (*list)->data = key;
    (*list)->next = calloc(1, sizeof(**list));
    (*list)->next->next = NULL;
    (*list)->next->data = 0;
    (*list) = (*list)->next;
}

void rmv(List **list) {
    if (!(*list)) {
        return;
    } else if (!(*list)->next) {
        //        printf("%c ", (*list)->data);
        free(*list);
        return;
    }
    rmv(&((*list)->next));
    printf("%c", (*list)->data);
    free(*list);
}

int main(void) {
    List *list = calloc(1, sizeof(*list));
    List *root = list;
    list->next = NULL;
    list->data = 0;
    char c;
    while ((c = getchar()) != EOF) {
        append(&list, c);
    }
    rmv(&root);
    return 0;
}