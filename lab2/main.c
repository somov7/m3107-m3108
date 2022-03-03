#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned long long UINT_MAX = 4294967296;

struct uint1024_t {
    unsigned array[32];
};

struct uint1024_t from_uint(unsigned x) { // generation uint1024_t from unsigned long
    
    struct uint1024_t result_number;
    for (int i = 0; i < 31; i++) {
        result_number.array[i] = 0;
    }
    result_number.array[31] = x;
    
    return result_number;
}


struct uint1024_t add_op(struct uint1024_t x, struct uint1024_t y) {  //сложение
    
    struct uint1024_t result_num;
    for (int i = 0; i < 32; i++) {
        result_num.array[i] = 0;
    }
    
    for (int i = 31; i > 0; i--) {
        unsigned long long x_long_long = x.array[i];
        unsigned long long y_long_long = y.array[i];
        result_num.array[i] += x_long_long + y_long_long;
        unsigned long long is_overflow = x_long_long + y_long_long;
        if (is_overflow >= UINT_MAX) {
            result_num.array[i - 1]++;
        }
    }
    
    result_num.array[0] += x.array[0] + y.array[0];
    return result_num;
}


struct uint1024_t subtr_op(struct uint1024_t x, struct uint1024_t y) { // вычитание
    struct uint1024_t result_num;
    
    for (int i = 0; i < 32; i++) {
        result_num.array[i] = 0;
    }
    
    for (int i = 31; i > 0; i--) {
        result_num.array[i] += x.array[i] - y.array[i];
        if (x.array[i] < y.array[i]) { // проверка на переполнение
            result_num.array[i - 1]--;
        }
    }
    result_num.array[0] += x.array[0] - y.array[0];
    
    return result_num;
}

unsigned mod_at_uint(struct uint1024_t x, unsigned y) { //остаток от деления
    
    struct uint1024_t result_num;
    for (int i = 0; i < 32; i++) {
        result_num.array[i] = 0;
    }
    
    unsigned long long remain = 0;
    for (int i = 0; i < 32; i++) {
        result_num.array[i] = (remain * UINT_MAX + x.array[i]) / y;
        remain = (remain * UINT_MAX + x.array[i]) % y;
    }
    
    return remain;
}


struct uint1024_t mult_at_uint(struct uint1024_t x, unsigned y) { // умножение uint1024 на unsigned
    
    struct uint1024_t result_num;
    for (int i = 0; i < 32; i++) {
        result_num.array[i] = 0;
    }
    
    for (int i = 31; i >=  0; i--) {
        unsigned long long x_long_long = x.array[i];
        unsigned long long y_long_long = y;
        unsigned long long temp_result_multiply = x_long_long * y_long_long;
        result_num.array[i] += temp_result_multiply % UINT_MAX;
        if (i != 0) {
            result_num.array[i - 1] += temp_result_multiply / UINT_MAX;
        }
    }
    
    return result_num;
}





struct uint1024_t div_at_uint(struct uint1024_t x, unsigned y) { //целая часть от деления 

    struct uint1024_t result_num;
    for (int i = 0; i < 32; i++) {
        result_num.array[i] = 0;
    }
    unsigned long long remain = 0;
    for (int i = 0; i < 32; i++) {
        result_num.array[i] = (remain * UINT_MAX + x.array[i]) / y;
        remain = (remain * UINT_MAX + x.array[i]) % y;
    }
    return result_num;
}



struct uint1024_t mult_op(struct uint1024_t x, struct uint1024_t y) {  // умножение
    struct uint1024_t result_num;
    
    for (int i = 0; i < 32; i++) {
        result_num.array[i] = 0;
    }
    
    for (int i = 31; i >= 0; i--) {
        if (y.array[i] != 0) {
            struct uint1024_t temp_result_of_mult = mult_at_uint(x, y.array[i]);
            int k = 31 - i;
            
            for (int j = k; j < 32; j++) { // сдвиг числа на разряд при умножении
                temp_result_of_mult.array[j - k] = temp_result_of_mult.array[j];
            }
            
            for (int j = 32 - k; j < 32; j++) {
                temp_result_of_mult.array[j] = 0;
            }
            result_num = add_op(result_num, temp_result_of_mult);
        }
    }
    
    return result_num;
}


void uint1024_t_printing(struct uint1024_t x) {  //вывод
    char buf[1024];
    for (int i = 0; i < 1024; i++) {
        buf[i] = '0';
    }
    
    for (int i = 0; i < 1024; i++) {
        unsigned mod = mod_at_uint(x, 10);
        buf[1023 - i] = mod + '0';
        x = div_at_uint(x, 10);
    }
    unsigned start = 0;
    
    while (buf[start] == '0') { //пропуск 0 в начале числа
        start++;
    }
    
    if (start == 1024) { //если все число - 0
        printf("0");
    }
    
    for (int i = start; i < 1024; i++) {
        printf("%c", buf[i]);
    }
}


void uint1024_t_input(struct uint1024_t* x) {  // ввод
    char string_current_num[1024];
    scanf("%s", string_current_num);
    
    for (int i = 0; i < 32; i++) {
        x -> array[i] = 0;
    }
    
    for (int i = 0; i < 1024 && string_current_num[i] != '\0'; i++) {
        *x = mult_at_uint(*x, 10); //сдвиг на разряд
        struct uint1024_t temp = from_uint(string_current_num[i] - '0'); //перевод символа в uint1024
        *x = add_op(*x, temp); //добавление символа к числу uint1024_t
    }
}

int main(int argc, const char * argv[]) {
    struct uint1024_t first_num, second_num;
    
    printf("Введите два числа через пробел: ");
    uint1024_t_input(&first_num);
    uint1024_t_input(&second_num);

    if (argc == 1) {
        printf("\nИтоговая сумма: ");
        uint1024_t_printing(add_op(first_num, second_num));
        
        printf("\nИтоговая разность: ");
        uint1024_t_printing(subtr_op(first_num, second_num));
        
        printf("\nЗначение произведения: ");
        uint1024_t_printing(mult_op(first_num, second_num));
        
        printf("\n");
    } else {
        fprintf(stderr, "Проверьте введенные параметры!");
        return EXIT_FAILURE;
    }

    return 0;
}
