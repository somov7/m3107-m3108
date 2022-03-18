/* Orlov Aleksandr, 12-5, M3107,  11.10.2021 */

#define ARR_SIZE 35
#define STRING_SIZE 310
#define INT_LEN 9
#define MAX_INT 1000000000

typedef struct {
    int a[ARR_SIZE];
    int ranks;
} uint1024_t;

void scanf_value(uint1024_t* x);
uint1024_t from_uint(unsigned int x);
void printf_value(uint1024_t x);
uint1024_t add_op(uint1024_t x, uint1024_t y);
uint1024_t subtr_op(uint1024_t x, uint1024_t y);
uint1024_t mult_op(uint1024_t x, uint1024_t y);
