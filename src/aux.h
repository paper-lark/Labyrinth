/* Type */
typedef enum {
    Wall, Empty//, Player, Exit
} State;

typedef struct {
    unsigned x, y;
} Point;

/* Prototypes */
State **create_labyrinth(const unsigned size);

unsigned makeodd(unsigned x);

void free_m(State **map, const unsigned size);

State **create_m(const unsigned size);

Point *rand_position(State **map, const unsigned size);
