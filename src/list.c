#include "list.h"
#include <stdlib.h>
#include <assert.h>

int isEmpty(List *list) {
    return list->length == 0;
}

List *nlist() {
    List *temp = malloc( sizeof(List) );
    temp->length = 0;
    temp->next = NULL;
    return temp;
}

void add(List *list, unsigned x, unsigned y, Direction dir) {
    list->length++;
    Entry *temp = malloc( sizeof(Entry) );
    temp->value.x = x;
    temp->value.y = y;
    temp->value.dir = dir;
    temp->next = list->next;
    list->next = temp;
}

Entry *getrandom(List *list) {
    assert(!isEmpty(list));
    int n = rand() % list->length;
    Entry *current = list->next;
    if (n == 0) {
        list->next = current->next;
        current->next = NULL;
        list->length--;
        return current;
    } else {
        Entry *previous = NULL;
        for(int i = 0; i < n; i++) {
            previous = current;
            current = current->next;
        }
        previous->next = current->next;
        current->next = NULL;
        list->length--;
        return current;
    }
}