/* Types */
typedef enum {
    up, down, left, right
} Direction;

typedef struct {
    unsigned x, y;
    Direction dir; 
} Cell;

typedef struct entry {
    struct entry *next;
    Cell value;
} Entry;

typedef struct {
    unsigned length;
    Entry *next;
} List;

/* Prototypes */
int isEmpty(List *list);
List *nlist();
void add(List *list, unsigned x, unsigned y, Direction dir);
Entry *getrandom(List *list);