#include "arena.h"
#include "linear.h"

typedef struct Data Data;

struct Data
{
    int value;
};

void process_arena()
{
    Arena a = {0};

    Data* p1 = arena_alloc(&a, sizeof(Data));
    Data* p2 = arena_alloc(&a, sizeof(Data));

    arena_free(&a);
}

void process_linear()
{
    Linear l = {0};

    Data* p1 = linear_alloc(&l, sizeof(Data));
    Data* p2 = linear_alloc(&l, sizeof(Data));
}

int main(void)
{
    process_linear();
    process_arena();

    return 0;
}
