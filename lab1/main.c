#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINES_COUNT_OPTION 0
#define FILE_SIZE_OPTION 1
#define WORDS_COUNT_OPTION 2

#define WRONG_QUERY_ERROR 3
#define WRONG_FILENAME_ERROR 4
#define WRONG_OPTIONS_ERROR 5
#define ALLOCATION_ERROR 6

void helpMessage() {
    printf("For correct program's work you need send similar query: ./WordCount <options> filename\n");
    printf("Options:\n-l | --lines to get lines count of file\n");
    printf("-c | --bytes to get size of file in bytes\n-w | --words to get count of words of file\n");
    exit(EXIT_SUCCESS);
}

void errorMessage(char flag) {
    switch (flag) {
        case WRONG_QUERY_ERROR:
            printf("Error. Expected query: ./WordCount <options> filename\n");
            break;
        case WRONG_FILENAME_ERROR:
            printf("Error. File not exist\n");
            break;
        case WRONG_OPTIONS_ERROR:
            printf("Error. All options are wrong\n");
            break;
        case ALLOCATION_ERROR:
            printf("Allocation error\n");
            break;
        default:
            printf("Error\n");
            break;
    }
    printf("Use -h | --help options for more information\n");
    exit(EXIT_FAILURE);
}

void parseUnknownOptions(char **argArray, int argSize, const int indexesArray[], int optionsSize) {
    printf("Used unknown options: ");
    for (int i = 0; i < optionsSize; ++i) {
        printf("%s", argArray[indexesArray[i]]); // вывод неверно введённых опций
        if (i < optionsSize - 1)
            printf(", ");
    }
    printf("\n");
    if (optionsSize == argSize - 2)
        errorMessage(WRONG_OPTIONS_ERROR); // все опции некорректны
}

void parseOptions(char **argArray, int argSize, int optionsArray[]) {
    char options[8][8] = {"-l", "--lines", "-c", "--bytes", "-w", "--words", "-h", "--help"}; // все возможные опции
    int unknownOptions[argSize - 2]; // массив индексов неверно введённых опций
    int unkOptionsCnt = 0; // счётчик некорректных опций
    for (int i = 0; i < 3; ++i)
        optionsArray[i] = 0;
    for (int i = 1; i < argSize - 1; ++i) {
        char isOptionCorrect = 0;
        for (int j = 0; j < 8; ++j) {
            if (strcmp(argArray[i], options[j]) == 0) { // проверка корректности i-ой опции
                if (j / 2 < 3)
                    optionsArray[j/2] = 1;
                isOptionCorrect = 1;
                break;
            }
        }
        if (isOptionCorrect == 0) { // найдена некорректная опция
            unknownOptions[unkOptionsCnt] = i;
            unkOptionsCnt++;
        }
    }
    if (unkOptionsCnt > 0)
        parseUnknownOptions(argArray, argSize, unknownOptions, unkOptionsCnt);
}

void getOptions(int argSize, char **argArray, int neededOptions[]) {
    if (argSize > 1 && (strcmp(argArray[1], "-h") == 0 || strcmp(argArray[1], "--help") == 0))
        helpMessage();
    if (argSize < 3)
        errorMessage(WRONG_QUERY_ERROR); // некорректный ввод
    if (!neededOptions) // для массива не выделяется места в памяти
        errorMessage(ALLOCATION_ERROR);
    parseOptions(argArray, argSize, neededOptions);
}

FILE* getFile(char* filename) {
    FILE *file;
    file = fopen(filename, "r"); // открытие файла для чтения
    if (file == NULL) // файла не существует
        errorMessage(WRONG_FILENAME_ERROR);
    return file;
}

void findLinesCount(FILE *file) {
    fseek(file, 0, SEEK_SET); // перемещение курсора на начало файла
    unsigned int linesCount = 0;
    char currentChar;
    while (!feof(file)) {
        currentChar = (char) fgetc(file); // использую fgetc() вместо fgets(), т.к. fgets() может считать
        if (currentChar == '\n' || currentChar == EOF) // строку с длиной < UINT16_MAX,
            linesCount++;                 // что не позволит сделать точное вычисление если line > UINT16_MAX
    }
    printf("File have %u lines\n", linesCount); // вывод результата
}

void findFileSize(FILE *file) {
    fseek(file, 0, SEEK_END); // перемещение курсора на конец файла
    unsigned long fileSize = ftell(file); // запись кол-ва байт от начала файла до положения курсора
    printf("Size of file: %lu bytes\n", fileSize); // вывод результата
}

void findWordsCount(FILE *file) {
    fseek(file, 0, SEEK_SET); // перемещение курсора на начало файла
    unsigned int wordsCount = 0;
    char currentChar = (char) fgetc(file);
    char previousChar;
    while (!feof(file)) {
        previousChar = currentChar;
        currentChar = (char) fgetc(file);
        if ((currentChar == ' ' || currentChar == '\n' || currentChar == EOF)
            && (previousChar != ' ' && previousChar != '\n'))
            wordsCount++;
    }
    printf("File have %u words\n", wordsCount); // вывод результата
}

void solve(FILE *file, const int options[]) {
    if (options[LINES_COUNT_OPTION] == 1)
        findLinesCount(file); // вычисление кол-ва строк
    if (options[FILE_SIZE_OPTION] == 1)
        findFileSize(file); // вычисление размера файла
    if (options[WORDS_COUNT_OPTION] == 1)
        findWordsCount(file); // вычисление кол-ва слов
}

int main(int argc, char **argv) {
    int options[3]; // массив для хранения требуемых опций
    getOptions(argc, argv, options); // проверка на корректность ввода и запись опций в массив
    FILE *file = getFile(argv[argc-1]); // получение запрошенного файла
    solve(file, options); // выполнение требуемых опций
    fclose(file);
    return 0;
}
