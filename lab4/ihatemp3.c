#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
struct vector {
    unsigned char *array;
    int counter;
    int size;
    unsigned char *last_el;
};


void vector_init(struct vector *arr) {
    arr->array = calloc(1, sizeof(char));
    arr->counter = 1;
    arr->last_el = arr->array;
    arr->size = 0;
}

void vector_add(struct vector *arr, int index, unsigned char val) {
    if (arr->size >= arr->counter) {
        unsigned char *temp = calloc(arr->counter * 2, sizeof(char));
        memcpy(temp, arr->array, arr->counter);
        free(arr->array);
        arr->array = temp;
        arr->counter *= 2;
    }
    arr->array[index] = val;
    arr->size++;
    arr->last_el = &arr->array[arr->size];
}

unsigned char *vector_get(struct vector *arr, int index) {
    if (index <= arr->size) {
        return &arr->array[index];
    } else {
        return NULL;
    }
}

void vector_replace(struct vector *arr, int index, unsigned char val) {
    if (index <= arr->size) {
        arr->array[index] = val;
    }
}

int vector_size(struct vector arr) {
    return arr.size;
}

struct header {
    char main[11];
    char extended[11];
};

char *offset_scale[54] = {"TXXX", "ILPS", "COMM", "WXXX", "MCDI", "ETCO", "MLLT", "STYC", "USLT", "SYLT", "RVAD",
                          "EQUA", "RVRB", "APIC", "GEOB", "PCNT", "POPM", "RBUF", "AENC", "LINK", "POSS", "USER",
                          "OWNE", "COMR", "ENCR", "GRID", "PRIV",
                          "1", "1", "4", "0", "S", "S", "S", "S", "1", "4", "S", "S", "S", "S", "S",
                          "S", "S", "S", "S", "S", "S", "4", "S", "S", "S", "S", "S"};


unsigned char encoding(FILE *pntr) {
    unsigned char enc;
    enc = fgetc(pntr);
    fseek(pntr, -1, SEEK_CUR);
    return enc;
}

int out_of_bounds(unsigned char *str) {
    for (int i = 0; i < 27; i++) {
        if (strcmp(str, offset_scale[i]) == 0 || (str[0] == 'T' && offset_scale[i][0] == 'T') ||
            (str[0] == 'W' && offset_scale[i][0] == 'W')) {
            return 0;
        }
    }
    return 1;
}

void set_tag_size(struct vector *buffer, int difference, int tag_size) {
    tag_size = tag_size + difference;
    unsigned new_size = 0;
    for (int i = 3; i >= 0; i--) {
        for (int j = 7; j >= 0; j--) {
            if ((tag_size >> (i * 8 + j)) % 2 == 1) {
                new_size += (1 << (i * 8 + j));
                if (j == 7 && i != 3) {
                    new_size = new_size + (1 << (((3 - i) * 8) + (7 - j) + 1));
                    new_size = new_size - (1 << (((3 - i) * 8) + (7 - j)));
                }
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        vector_replace(buffer, 6 + i, tag_size >> ((3 - i) * 8));
    }
}


int skip(FILE *pntr, unsigned char *str, int frame_size) {
    int offset;
    char exists = '0';
    for (int i = 0; i < 27; i++) {
        if (strcmp(str, offset_scale[i]) == 0 || (str[0] == 'T' && offset_scale[i][0] == 'T') ||
            (str[0] == 'W' && offset_scale[i][0] == 'W')) {
            offset = offset_scale[i + 27][0] - 48;
            exists = '1';
            break;
        }
    }
    if (exists != '0') {
        if (offset != ('S' - 48)) {
            fseek(pntr, offset, SEEK_CUR);
        } else {
            fseek(pntr, frame_size, SEEK_CUR);
            offset = frame_size;
        }
        return (frame_size - offset);
    } else {
        return -1;
    }
}

int get_frame_s(FILE *pntr) {
    unsigned char size[4];
    for (int i = 0; i < 4; i++) {
        size[i] = fgetc(pntr);
    }
    int frame_size = 0;
    for (int i = 3; i >= 0; i--) {
        for (int j = 7; j >= 0; j--) {
            if ((size[i] >> (7 - j)) % 2 == 1) {
                frame_size = frame_size + (1 << (8 * (3 - i) + (7 - j)));
            }
        }
    }
    return frame_size;
}

void set_frame_s(int frame_size, struct vector *buff, int unsync) {
    for (int i = 0; i < 4; i++) {
        unsigned char temp = frame_size >> ((3 - i) * 8);
        vector_add(buff, vector_size(*buff), temp);
        if ((temp == 255 || temp == 254) && unsync == 1) {
            vector_add(buff, vector_size(*buff), '\0');
        }
    }
}

int tag_size(unsigned char *str) {
    int num = 0;
    int counter = 0;
    for (int i = 3; i >= 0; i--) {
        for (int k = 7; k > 0; k--) {
            if ((str[i] >> (7 - k)) % 2 == 1) {
                num += (1 << counter);
            }
            counter++;
        }
    }
    return num;
}

char extend_header_check(struct header head) {
    if ((head.main[5] >> 5) % 2 == 1) {
        return 1;
    } else {
        return 0;
    }
}

char unsynchronization_check(struct header head) {
    if ((head.main[5] >> 6) % 2 == 1) {
        return 1;
    } else {
        return 0;
    }
}

void get_frame_name(FILE *pntr, unsigned char name[]) {
    for (int i = 0; i < 4; i++) {
        name[i] = fgetc(pntr);
    }
    name[4] = '\0';
    if (out_of_bounds(name)) {
        fseek(pntr, -4, SEEK_CUR);
    }
}

int frame_read(FILE *pnter, unsigned char *str, char read, char unsync) {
    unsigned char flags[2] = "\0";
    int size = get_frame_s(pnter);
    for (int i = 0; i < 2; i++) {
        flags[i] = fgetc(pnter);
        if (unsync == 1 && flags[i] == 255) {
            fgetc(pnter);
        }
    }
    char enc = encoding(pnter);
    int temp = skip(pnter, str, size);
    if (temp >= 0) {
        unsigned char c;
        int cnter = 0;
        if (enc == 1 || enc == 2) {
            fseek(pnter, 2, SEEK_CUR);
            while (cnter < (size - (size - temp) - 2)) { //разбиение на encoding
                c = fgetwc(pnter);
                cnter += 2;
                if (read != 0) {
                    wprintf(L"%c", c);
                }
                if (unsync == 1 && c == 255) {
                    fgetc(pnter);
                    cnter += 1;
                }
            }
            if (read != 0) {
                printf("\n");
            }
            if (cnter < (size - (size - temp)) - 2) {
                fseek(pnter, size - (size - temp) - cnter, SEEK_CUR);
            }
        } else if (enc == 0 || enc == 3) {
            while (cnter < (size - (size - temp))) {
                c = fgetc(pnter);
                cnter++;
                if (read != 0) {
                    printf("%c", c);
                }
                if (unsync == 1 && c == 255) {
                    fgetc(pnter);
                    cnter++;
                }
            }
            if (read != 0) {
                printf("\n");
            }
            if (cnter < (size - (size - temp)) - 1) {
                fseek(pnter, size - (size - temp) - cnter, SEEK_CUR);
            }
            return 1;
        }
    } else {
        return 0;
    }
}


void get(FILE *pntr, unsigned char *input, char unsync) {
    unsigned char frame[5];
    get_frame_name(pntr, frame);
    int check = out_of_bounds(frame);
    while (strcmp(input, frame) != 0 && check != 1) {
        frame_read(pntr, frame, 0, unsync);
        get_frame_name(pntr, frame);
        check = out_of_bounds(frame);
    }
    if (check != 1) {
        frame_read(pntr, frame, 1, unsync);
    } else {
        printf("404 Not found");
    }
}

void show(FILE *pntr, char unsync) {
    unsigned char fr_name[5] = "\0";
    get_frame_name(pntr, fr_name);
    printf("%s ", fr_name);
    int check = out_of_bounds(fr_name);
    while (check != 1) {
        frame_read(pntr, fr_name, 1, unsync);
        get_frame_name(pntr, fr_name);
        printf("%s ", fr_name);
        check = out_of_bounds(fr_name);
    }
}

void set(FILE *pntr, struct vector *replace_s, const char *file_o,
         unsigned char *input, char unsync, int tag_size, struct vector *buffer) {
    unsigned char frame[5];
    get_frame_name(pntr, frame);
    int check = out_of_bounds(frame);
    while (strcmp(input, frame) != 0 && check != 1) {
        frame_read(pntr, frame, 0, unsync);
        get_frame_name(pntr, frame);
        check = out_of_bounds(frame);
    }
    int difference;
    if (check == 1) { // если тэг не был найден - создает новый.
        FILE *ptr = fopen(file_o, "rb"); //доходим до начала паддинга, создаем фрейм, вписываем его в вектор
        while (ftell(pntr) != ftell(ptr)) {
            vector_add(buffer, vector_size(*buffer), fgetc(ptr));
        }
        for (int i = 0; i < 4; i++) {
            vector_add(buffer, vector_size(*buffer), input[i]);
        }

        set_frame_s(replace_s->size + 1, buffer, unsync);
        for (int i = 0; i < 3; i++) {
            vector_add(buffer, vector_size(*buffer), '\0');
        }

        int cntr = 0;
        while (cntr != vector_size(*replace_s)) {
            vector_add(buffer, vector_size(*buffer), replace_s->array[cntr]);
            if ((replace_s->array[cntr] == 255 || replace_s->array[cntr] == 254) && unsync == 1) {
                vector_add(buffer, vector_size(*buffer), '\0');
            }
            cntr++;
        }


        fseek(pntr, 0, SEEK_END); //дампаем концовку файла до конца
        while (ftell(pntr) != ftell(ptr)) {
            vector_add(buffer, vector_size(*buffer), fgetc(ptr));
        }

        set_tag_size(buffer, replace_s->size + 1, tag_size);
        ptr = fopen(file_o, "wb"); //дампаем в файл
        fwrite(buffer->array, vector_size(*buffer), 1, ptr);
        fclose(ptr);
    } else { //если все ок - то меняет сайз фрейма, сам фрейм и сайз тэга

        FILE *ptr = fopen(file_o, "rb");
        while (ftell(pntr) != ftell(ptr)) {
            vector_add(buffer, vector_size(*buffer), fgetc(ptr));
        }
        int size = get_frame_s(pntr);
        int old_size = size;
        difference = replace_s->size - size;
        set_frame_s(size + (difference + 1), buffer, unsync);
        for (int i = 0; i < 3; i++) {
            vector_add(buffer, vector_size(*buffer), '\0');
        }
        int cntr = 0;

        while (cntr != replace_s->size) // добавить строку в вектор
        {
            vector_add(buffer, vector_size(*buffer), replace_s->array[cntr]);
            if ((replace_s->array[cntr] == 255 || replace_s->array[cntr] == 254) && unsync == 1) {
                vector_add(buffer, vector_size(*buffer), '\0');
            }
            cntr++;
        }
        fseek(pntr, old_size + 2, SEEK_CUR); //скипнуть флаги
        fseek(ptr, 0, SEEK_END); //перекинуть один указатель в конец файла, чтобы читать до него с начала нового тэга
        unsigned char c = fgetc(pntr);

        while (ftell(pntr) != ftell(ptr)) {
            vector_add(buffer, vector_size(*buffer), c);
            if ((c == 255 || c == 254) && unsync == 1) {
                vector_add(buffer, vector_size(*buffer), '\0');
            }
            c = fgetc(pntr);
        }
        set_tag_size(buffer, difference, tag_size);
        fclose(pntr);
        fclose(ptr);

        ptr = fopen(file_o, "wb"); //дампаем из оперативы в файл
        fwrite(buffer->array, vector_size(*buffer), 1, ptr);
        fclose(ptr);
    }
}

int main(int argc, const char *argv[]) {
    FILE *ptr;
    setlocale(LC_ALL, "Russian");
    struct vector filepath;
    vector_init(&filepath);

    struct vector option;
    vector_init(&option);

    struct vector frame;
    vector_init(&frame);

    struct vector val;
    vector_init(&val);

    if (argc >= 3) {
        char *temp;
        temp = argv[1] + 2;
        while (*temp != '\0') {
            vector_add(&filepath, vector_size(filepath), *temp);
            if (*temp == '=') {
                filepath.array[vector_size(filepath) - 1] = '/';
            }
            temp++;
        }
        vector_add(&filepath, vector_size(filepath), '\0');
        temp = argv[2] + 2;
        char *c = strstr(argv[2], "=");
        if (c == NULL) {
            while (*temp != '\0') {
                vector_add(&option, vector_size(option), *temp);
                temp++;
            }
            vector_add(&option, vector_size(option), '\0');
        } else {
            while (*temp != '=') {
                vector_add(&option, vector_size(option), *temp);
                temp++;
            }
            vector_add(&option, vector_size(option), '\0');
            temp++;
            while (*temp != '\0') {
                vector_add(&frame, vector_size(frame), *temp);
                temp++;
            }
            vector_add(&frame, vector_size(frame), '\0');

        }

        if (argc >= 4) {
            temp = strstr(argv[3], "=");
            temp++;
            while (*temp != '\0') {
                vector_add(&val, vector_size(val), *temp);
                temp++;
            }
            temp++;
        }
        ptr = fopen(filepath.array, "rb");
        if (ptr != NULL) {
            struct header head = {0};
            head.main[10] = '\0';
            head.extended[10] = '\0';
            for (int i = 0; i < 10; i++) {
                head.main[i] = fgetc(ptr);
            }
            char tag_s_str[4];
            memcpy(tag_s_str, &head.main[6], 4);
            int tag_s = tag_size(tag_s_str);

            char unsync = 0;

            if (extend_header_check(head)) {
                for (int i = 0; i < 10; i++) {
                    head.extended[i] = fgetc(ptr);
                }
                unsync = unsynchronization_check(head);
            }

            if (strcmp(option.array, "show") == 0) {
                show(ptr, unsync);
            } else if (strcmp(option.array, "get") == 0) {
                get(ptr, frame.array, unsync);
            } else if (strcmp(option.array, "set") == 0) {
                struct vector buffer = {0};
                vector_init(&buffer);
                set(ptr, &val, filepath.array, frame.array, unsync, tag_s, &buffer);
            } else {
                printf("Incorrect argument input");
            }
        } else {
            printf("There was a problem opening the file.");
        }
    } else {
        printf("Insufficient amount of arguments");
    }
    return 0;
}
