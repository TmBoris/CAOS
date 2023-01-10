#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct List {
    int data;
    struct List *next;
} List;

void append(List **list, int key) {
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
    printf("%d\n", (*list)->data);
	
    free(*list);
}

int main(void) {
    List *list = calloc(1, sizeof(*list));
    List *root = list;
    list->next = NULL;
    list->data = 0;
    int c;
    int i = 0;
    while (scanf("%d", &c) > 0) {
        append(&list, c);
        i += 1;
    }
    printf("1");
    rmv(&root);
    return 0;
}
