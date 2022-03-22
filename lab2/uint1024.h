#define uint unsigned int

typedef struct {
    int* a;
    int size;
} uint1024_t;

int get_max(int a, int b);
void scanf_value(uint1024_t* a);
void printf_value(uint1024_t a);
uint1024_t from_uint(unsigned int a);
uint1024_t add_op(uint1024_t a, uint1024_t b);
uint1024_t subtr_op(uint1024_t a, uint1024_t b);
uint1024_t mult_op(uint1024_t a, uint1024_t b);
