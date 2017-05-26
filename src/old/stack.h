#include <stdlib.h>
#include <assert.h>

typedef struct st Stack;
typedef struct pt {
    unsigned x, y;
} Point;

Stack *new_stack();
void push(Stack *head, Point *data);
Point *pop(Stack *head);
int isEmpty(Stack *head);