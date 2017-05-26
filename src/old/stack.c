#include "stack.h"

typedef struct Entry {
    Point *value;
    struct Entry *next;
} Entry;

typedef struct st {
    Entry *next;
} Stack;

Stack *new_stack() {
    Stack *head = malloc(sizeof(Stack));
    head->next = NULL;
    return head;
}

void push(Stack *head, Point *data) {
    Entry *temp = malloc(sizeof(Entry));
    temp->value = data;
    temp->next = head->next;
    head->next = temp;
}

Point *pop(Stack *head) {
    assert(!isEmpty(head));
    Entry *temp = head->next;
    head->next = temp->next;
    Point *result = temp->value;
    free(temp);
    return result;
}

int isEmpty(Stack *head) {
    return head->next == NULL;
}