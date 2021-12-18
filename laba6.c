#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ФУНКЦИИ ДЛЯ ДЕРЕВА

struct Node{ //ура узел
    int freq; // тут хранится частота с которой встречается значение
    unsigned char value; // тут хранится значение
    struct Node *left, *right; // тут оно не разрешает мне объявить это без слова struct поэтому я его напишу.
};

struct Node *join(struct Node *left, struct Node *right) { //склеивание веток в одно дерево и добавление мнимого узла
    struct Node *c = (struct Node *) malloc(sizeof(struct Node));
    c->right = right;
    c->left = left;
    c->freq = right->freq + left->freq;
    c->value = -1;
    return c;
}

void next(struct Node *node, char *string, int stringsize, char **dict) { //с днем словарей
    if (node->right == NULL) {
        strcpy(dict[(int) node->value], string);
        return;
    }
    string[stringsize + 1] = '\0';
    string[stringsize] = '1';
    next(node->right, string, stringsize + 1, dict); //ну вот это хаффман типа ходим по дереву
    string[stringsize] = '0';
    next(node->left, string, stringsize + 1, dict); // потихоньку увеличиваем размер строки поке не дойдем до самого правого
}

void bubble_sort(struct Node **mas, int n) { //как обычный но мы перекладываем узлы по частоте
    int f = 1;
    while (f) {
        f = 0;
        for (int i = 0; i < n - 1; ++i) {
            if (mas[i]->freq < mas[i + 1]->freq) {
                struct Node *temp = (struct Node *) malloc(sizeof(struct Node));
                temp = mas[i];
                mas[i] = mas[i + 1];
                mas[i + 1] = temp;
                f = 1;
            }
        }
    }
}

// ФУНКЦИИ ПРО ХАФФМАНА

void to_char(int huf, char *dict, int k, int p) { //что за школа без прикола
    for (int i = 0; i < p; ++i) { // из кода хаффмана делаем строку, обычно он числом
        dict[k + p - 1 - i] = huf % 2 + '0'; //плюс ноль делает из числа строку
        huf >> 2;
    }
    dict[k + p] = '\0';
}

// ФУНКЦИИ МУСОРНЫЕ
int reverse(int x) { //перевернули число
    int res = 0;
    for (int i = 0; i < 8; ++i) {
        res = res * 2 + x % 2;
        x >> 2;
    }
    return res;
}


// ФУНКЦИИ АРХИВАЦИИ

void arh(char *inputFileName, char *outputFileName) {
    FILE *inputFile = fopen(inputFileName, "rb");
    FILE *outputFile = fopen(outputFileName, "wb");

    char **dict = (char **) malloc(256 * sizeof(char *)); // с днем словарей
    for (int i = 0; i < 256; ++i){ //вряд ли наверное у нас будет больше этого алфавита
        dict[i] = (char *) malloc(256 * sizeof(char)); // сразу в ячейку символа кладем его код хаффмана
    }
    int *frequency = (int *) calloc(256, sizeof(int)); //массив частот

    struct Node **tree = (struct Node **) malloc(256 * sizeof(struct Node *)); //удивительное дерево из узлов

    while (!feof(inputFile)){
        frequency[(unsigned char) getc(inputFile)]++; //в ИНДЕКСЕ чара хранится его частота
    }

    int k = 0; // это текущее количество узлов в дереве
    for (int i = 0; i < 256; ++i) {
        if (frequency[i] != 0) {
            struct Node *node = (struct Node *) malloc(sizeof(struct Node)); //делаем узлы для всего
            node->value = (unsigned char) i;
            node->freq = frequency[i];
            node->right = NULL;
            tree[k] = node; //кидаем в дерево узлы по порядку
            k++;
        }
    }

    int numofnods = k; // это целое количество узлов в дереве
    bubble_sort(tree, numofnods);

    while (k != 1) {
        bubble_sort(tree, k);
        tree[k - 2] = join(tree[k - 2], tree[k - 1]);
        k--;
    }

    char *string = (char *) calloc(256, sizeof(char));
    int strSize = 0;

    next(tree[0], string, strSize, dict);

    fseek(inputFile, 0, SEEK_SET);

    putc(numofnods - 1, outputFile); //запись начинаем с получившегося количества узлов

    for (int i = 0; i < 256; ++i) {
        // записываем в словарь такую табличку: символ, длина его кода хаффмана, его код хаффмана
        if (frequency[i] != 0) {
            int count = 0, t = 0;
            putc(i, outputFile);
            int n = strlen(dict[i]);
            putc(n, outputFile);
            for (int j = 0; j < n; ++j) {
                t = t * 2 + (dict[i][j] == '0' ? 0 : 1); // проходимся по коду хаффмана от символа и записываем его в t
                count++;
                if (count == 8) {
                    count = 0;
                    putc(t, outputFile); //побайтово
                    t = 0;
                }
            }
            if (count != 0) {
                putc(t, outputFile); //остаток
            }
        }
    }

    int sum = 0, count = 0;
    unsigned char nextchar;
    while (!feof(inputFile)) { //начинаем собственно информацию записывать
        nextchar = getc(inputFile); //берем символ
        for (int i = 0; dict[nextchar][i] != '\0'; ++i) {
            sum = sum * 2 + (dict[nextchar][i] - '0'); //вместо символа пишем его словарное значение а еще умножаем на два
            count += 1;
            if (count == 8) { // это потому что мы хотим все записать в один байт
                putc((char) sum, outputFile);
                count = 0;
                sum = 0;
            }
        }
    }

    if (count != 0) {
        putc((char) sum << (8 - count), outputFile); // остаток если остался
    }

    putc(8 - count, outputFile); //написали

    for (int i = 0; i < 256; ++i){
        free(dict[i]); //все удалили нафиг все
    }
    free(dict);
    fclose(inputFile);
    fclose(outputFile);
    printf("anyway we arhived this one\n");
}

void dearh(char *archiveName, char *arhivedfileName) {
    FILE *archive = fopen(archiveName, "rb");
    FILE *arhivedfile = fopen(arhivedfileName, "wb");
    unsigned char **dict = (unsigned char **) malloc(256 * sizeof(unsigned char *));
    for (int i = 0; i < 256; ++i)
        dict[i] = (unsigned char *) malloc(256 + sizeof(char));
    int numofnods = fgetc(archive); //считываем сколько у нас в словаре символов
    numofnods += 1;

    for (int i = 0; i < numofnods; ++i) {
        int ch = getc(archive); //считываем символ
        int len = getc(archive); //считываем длину его кода хаффмана
        for (int j = 0; j < len / 8; ++j) {
            int huf = getc(archive); //считываем его код хаффмана
            to_char(huf, dict[ch], 8 * j, 8); //из кода хаффмана создаем символ опять и кладем в словарь
        }
        if (len % 8 != 0) {
            int huf = getc(archive);
            to_char(huf, dict[ch], len / 8 * 8, len % 8); //считываем остаточек если остался
        }
    } //теперь в словаре лежат символы!!!
    // считали алфавит, начинаем считывать информацию

    unsigned char *str = (unsigned char *) calloc(256, sizeof(char));
    int lasttwo[2];
    int t = 0;
    int nextchar = getc(archive);
    lasttwo[1] = nextchar;
    nextchar = fgetc(archive);
    while (!feof(archive)) { //мне так плохо уже
        lasttwo[0] = lasttwo[1];
        lasttwo[1] = nextchar;
        nextchar = getc(archive);

        int x = (unsigned char) lasttwo[0];
        x = reverse(x);
        for (int i = 0; i < 8; ++i) {
            str[t] = (x % 2) + '0'; //делаем из инта по битам байт и строчку
            t++;
            str[t] = '\0';
            x >> 2;

            int flag = 0;
            int j = 0; // тут мы проверяем, есть ли такой код хаффмана
            for (; j < 256; ++j) //проходим по всему словарю кодов хаффмана
                if (strcmp(dict[j], str) == 0) {
                    flag = 1;
                    break;
                }
            if (flag){
                putc(j, arhivedfile);//если такой код есть то выписываем символ!!!
                t = 0;
                strcpy(str, ""); //и начинаем с начала
            }
        }
    }

    int x = (unsigned char) lasttwo[0];
    x = reverse(x);
    for (int i = 0; i < 8 - (lasttwo[1] % 8); ++i) { //это тоже самое для оставшегося кусочка опять
        str[t] = (x % 2) + '0';
        t++;
        str[t] = '\0';
        x >> 2;

        int j = 0;
        for (; j < 256; ++j)
            if (strcmp(dict[j], str) == 0) {
                putc(j, arhivedfile);
                t = 0;
            }
    }

    fclose(archive);
    fclose(arhivedfile);
}


// ПОЛНЫЕ ФУНКЦИИ КОТОРЫЕ ПРОСИЛИ

void create(char *outname, char **firstFile, char **lastFile) { //ммм файлы
    FILE *output = fopen(outname, "wb");
    FILE *curfile;

    char **nextfile = firstFile; //указатель на тот файл на котором мы сейчасс
    int size;
    int numoffiles = lastFile - firstFile;
    fprintf(output, "%d\n", numoffiles); //сначала записываем в вывод сколько мы хотим архивировать файлов
    for (; nextfile != lastFile; nextfile++) { //двигаем указатель вот это туса капец
        // printf("%s\n", *nextfile); //можно посмотреть где упал если упал
        curfile = fopen(*nextfile, "rb");
        fseek(curfile, 0, SEEK_END);
        size = ftell(curfile);
        fseek(curfile, 0, SEEK_SET);
        fprintf(output, "%s\n%d\n", *nextfile, size); //пишем имя и размер
        for (int j = 0; j < size; ++j) {
            char c = getc(curfile);
            fprintf(output, "%c", c); //ну файл переписываем просто
        }
        fprintf(output, "\n");
        fclose(curfile);
    }

    fclose(output);
}

void extract(char *archive) {
    FILE *input = fopen(archive, "rb");
    int numoffiles;
    fscanf(input, "%d", &numoffiles); //теперь мы смотрим сколько мы заархивировали
    for (int i = 0; i < numoffiles; ++i) {
        char title[128];
        int size;
        fscanf(input, "%s\n%d\n", title, &size); //считываем название-размер
        FILE *output = fopen(title, "wb");
        int nextchar;
        for (int j = 0; j < (int) size; ++j) {
            nextchar = getc(input);
            fprintf(output, "%c", nextchar); //ну и файл по порядку
        }
        fclose(output);
    }

    fclose(input);
}

void list(char *archive) {
    FILE *input = fopen(archive, "rb"); //хожу просто по архиву считываю заголовки файлов
    int numoffiles;
    fscanf(input, "%d", &numoffiles);
    printf("Files:\n");
    for (int i = 1; i <= numoffiles; ++i) {
        int size;
        char title[128];
        fscanf(input, "%s\n%d\n", title, &size);
        printf(" %d name %s\n", i, title);
        fseek(input, size, SEEK_CUR); // черед данные перепрыгиваем
    }
    fclose(input);
}


int main(int argc, char *argv[]) {
    char *archive;
    char *finalfile = "filetodearh.txt";
    char *shadowfile = "filetoarh.txt";

    for (int i = 0; i < argc; ++i) {
        if (strstr(argv[i], "--file")) {
            archive = argv[i + 1];
        } else if (strstr(argv[i], "--create")) {
            create(shadowfile, &argv[i + 1], &argv[argc]); //argc это последний!!
            arh(shadowfile, archive);
        } else if (strstr(argv[i], "--extract")) {
            dearh(archive, finalfile);
            extract(shadowfile);
        } else if (strstr(argv[i], "--list")) {
            dearh(archive, finalfile);
            list(shadowfile);
        }
    }

    return 0;
}