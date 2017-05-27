/* Type */
typedef enum {
    Wall, Empty//, Player, Exit
} State;

typedef struct {
    unsigned x, y;
} Point;

typedef signed char Hidden;

/* Prototypes */
State **create_labyrinth(const unsigned size);

Hidden **create_hid(const unsigned size);

void free_st(State **map, const unsigned size);

void free_hid(Hidden **map, const unsigned size);

void reveal(Hidden **fog, const Point *player, const unsigned size);

unsigned makeodd(unsigned x);

State **create_st(const unsigned size);

Point *rand_position(State **map, const unsigned size);
