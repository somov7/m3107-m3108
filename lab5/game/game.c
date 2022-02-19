//
// Created by Rustam Tishkov on 10.01.2022.
//

#include <stdlib.h>
#include <stdbool.h>
#include "game.h"
#include "../helpers/helpers.h"

BMPHeader header;

short WHITE_POINT = 1;
short BLACK_POINT = 0;

// структура bmp - https://bit.ly/3IeCw3W
short **readData(FILE *file) {
    // считываем хедер
    fseek(file, 0, SEEK_SET);

    checkBfType(file);

    skipFileData(file, SEEK_CUR, 8);

    // считываем данные для хедера
    getHeaderData(file, &header);

    skipFileData(file, SEEK_CUR, 2);

    checkBitsForPixel(file);

    skipFileData(file, SEEK_SET, 54);

    // проверяем если BLACK_POINT = 0 или 1
    unsigned char table[4];
    if (fread(table, 4, 1, file) == 0)
        exit(EXIT_FAILURE);

    for (int i = 0; i < 4; i++) {
        printf("%d\n", table[i]);
    }

    if (table[0] == WHITE) {
        BLACK_POINT = 1;
        WHITE_POINT = 0;
    }

    getFileHeader(&header, file);

    // считываем картинку
    short **matrix = createMatrix(header);
    fseek(file, header.offset, SEEK_SET);
    int y = 0, x = 0, read = 0;
    while (y < header.height) {
        unsigned char chr = fgetc(file);
        read++;
        if (read == 4) {
            read = 0;
        }
        // обработка символа
        for (int bit = 7; bit >= 0; bit--) {
            // чекаем если точка закрашена
            if (((1 << bit) & chr) >> bit != WHITE_POINT) {
                matrix[y][x] = POINT;
            } else {
                matrix[y][x] = NO_POINT;
            }
            x++;
            // если строка закончилась то выходим
            if (x == header.width)
                break;
        }
        if (x == header.width) {
            // читаем до 4 байтов в одной строке
            while (read < 4) {
                fgetc(file);
                read++;
            }
            x = 0;
            read = 0;
            // переходим на следующую строку
            y++;
        }
    }
    return matrix;
}

void saveGeneration(short **data, int generation, Params params) {
    FILE *file = createFile(params, generation);

    fseek(file, 0, SEEK_SET);

    writeFileHeader(file, &header);

    int y = 0, x = 0, written = 0;
    while (y < header.height) {
        unsigned char character = (char) 0;
        for (int bit = 7; bit >= 0; bit--) {
            if (data[y][x] == POINT) {
                character = character | (BLACK_POINT << bit);
            } else {
                character = character | (WHITE_POINT << bit);
            }
            x++;
            if (x == header.width) {
                break; // строка закончилась
            }
        }
        if (fputc(character, file) == EOF) {
            exit(EXIT_FAILURE);
        }
        written++;
        if (written == 4) {
            written = 0;
        }
        if (x == header.width) {
            // записываем до 4 байтов в одну линию
            while (written < 4) {
                if (fputc((char) 0, file) == EOF) {
                    exit(EXIT_FAILURE);
                }
                written++;
            }
            written = 0;
            y++; // переходим на след строку
            x = 0;
        }
    }
    fclose(file);
}

int getNeighbourCells(short **matrix, BMPHeader header, int x, int y) {
    int neighbourCells = 0;
    for (int _x = x - 1; _x <= x + 1; _x++) {
        for (int _y = y - 1; _y <= y + 1; _y++) {
            if (_x == x && _y == y)
                continue;
            int tempX = _x, tempY = _y;
            checkCoords(header, &tempX, &tempY);
            if (matrix[tempX][tempY] == POINT)
                neighbourCells++;
        }
    }
    return neighbourCells;
}


int processGeneration(short **matrix) {
    short **tempMatrix = createMatrix(header); // создаем временную матрицу для хранения стейта
    // хуячим неправильные клетки и оживляем правильные
    bool isStateChanged = false;
    bool hasLive = false;
    for (int x = 0; x < header.height; x++) {
        for (int y = 0; y < header.width; y++) {
            int cells = getNeighbourCells(matrix, header, x, y);
            updateTempMatrix(matrix, tempMatrix, x, y, cells, &isStateChanged, &hasLive);
        }
    }

    fillMatrix(matrix, tempMatrix, header);
    freeMatrix(tempMatrix, header);

    if (isStateChanged == false || hasLive == false) {
        return true;
    }
    return false;
}

void start(FILE *file, Params params) {
    short **matrix = readData(file);
    // read init state from file
    fclose(file);
    int generation = 0;
    while (true) {
        bool gameOver = processGeneration(matrix);
        generation++;
        if (gameOver == false) {
            if (params.maxIter > 0 && generation == params.maxIter &&
                params.maxIter != -1) { // проверка на последнее поколение
                gameOver = true;
            }
        }
        if (generation % params.dumpFreq == 0 || gameOver == true) {
            saveGeneration(matrix, generation, params);
        }
        if (gameOver == true)
            break;
    }
    freeMatrix(matrix, header);
    free(header.fileHeader);
}
