
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define uchar unsigned char
#define ulong unsigned long
#define uint unsigned int

int main(int argc, char *argv[]) {

    uint iterationsNumber = 9999;
    long dumpFrequency = 1;
    ulong width = 0, height = 0;
    char *origOutputDir = "Steps\\";
    char *outputDir;

    outputDir = malloc(strlen(origOutputDir));
    strcpy(outputDir, origOutputDir);

    char *inputFile = "input_file.bmp";
    char *line = "----------------------------------------------------------------------------------------------------";

    printf("%s\n\n", line);
    //printf("Input \"help\" to get help with commands.\n");

    char *CorrectUsage = "Correct usage: program.exe --input input.bmp --output some_directory --max_iter (number) --dump_freq (number)\n\n";
    char *tipSkip = "All arguments are optional and can go in any order. Default values will be used then.\n\n";
    char *tipOutputPt1 = "You can skip \"--output some_directory\" section. This way pictures would be saved into default folder \"";
    char *tipOutputPt2 = "\" near the program.\n\n";
    char *tipInputPt1 = "You can skip \"--input input.bmp\" section. This way picture \"";
    char *tipInputPt2 = "\" will be opened.\n\n";
    char *tipMax_iter = "You can skip \"--max_iter (number)\" section. This way iterations may be infinite.\n\n";
    char *tipDump_freq = "You can skip \"--dump_freq (number)\" section. This way picture will be saved every step.\n\n";

    if ((argc == 2) && (!strcmp(argv[1], "help"))) {
        printf("%s%s%s%s%s%s%s%s%s%s", CorrectUsage, tipSkip, tipInputPt1, inputFile, tipInputPt2, tipOutputPt1,
               outputDir, tipOutputPt2, tipMax_iter, tipDump_freq);
        return 0;
    }

    if (argc > 9) {
        printf("Error: Too many arguments\n%s", CorrectUsage);
        return -1;
    }

    if (!(argc % 2)) {
        printf("Error: Wrong number of arguments\n%s", CorrectUsage);
        return -1;
    }

    short flagInput = 0, flagOutput = 0, flagMax_iter = 0, flagDump_freq = 0;

    for (int i = 1; i < argc; i += 2) {
        if ((argv[i][0] == '-') && (argv[i][1] == '-')) {
            if (!strcmp(argv[i], "--input")) {
                if (flagInput == 0) {
                    flagInput++;
                    inputFile = malloc(argv[i + 1]);
                    strcpy(inputFile, argv[i + 1]);
                } else {
                    printf("Error: Too many \"--input\" arguments.\n");
                    return -1;
                }
            } else if (!strcmp(argv[i], "--output")) {
                if (flagOutput == 0) {
                    flagOutput++;
                    outputDir = malloc(argv[i + 1]);
                    strcpy(outputDir, argv[i + 1]);
                } else {
                    printf("Error: Too many \"--output\" arguments.\n");
                    return -1;
                }
            } else if (!strcmp(argv[i], "--max_iter")) {
                if (flagMax_iter == 0) {
                    flagMax_iter++;
                    for (int j = 0; j < strlen(argv[i + 1]); j++) {
                        if (!isdigit(argv[i + 1][j])) {
                            printf("Error: Number of \"--max_iter\" isn't a positive number.\n");
                            return -1;
                        }
                        if (atoi(argv[i + 1]) <= 0) {
                            printf("Error: Number of \"--max_iter\" must be higher than zero.\n");
                            return -1;
                        }
                    }
                    iterationsNumber = atoi(argv[i + 1]);
                    if (iterationsNumber > 9999) iterationsNumber = 9999;
                } else {
                    printf("Error: Too many \"--max_iter\" arguments.\n");
                    return -1;
                }
            } else if (!strcmp(argv[i], "--dump_freq")) {
                if (flagDump_freq == 0) {
                    flagDump_freq++;
                    for (int j = 0; j < strlen(argv[i + 1]); j++) {
                        if (!isdigit(argv[i + 1][j])) {
                            printf("Error: Number of \"--dump_freq\" isn't a positive number.\n");
                            return -1;
                        }
                        if (atoi(argv[i + 1]) <= 0) {
                            printf("Error: Number of \"--dump_freq\" must be higher than zero.\n");
                            return -1;
                        }
                    }
                    dumpFrequency = atoi(argv[i + 1]);
                } else {
                    printf("Error: Too many \"--dump_freq\" arguments\n");
                    return -1;
                }
            }
        }
    }

    FILE *initial;
    initial = fopen(inputFile, "rb");
    if (!initial) {
        printf("Error: file \"%s\" could not be opened.\n", inputFile);
        return -1;
    }

    uchar *BMP_beginning = malloc(63);

    for (int k = 0; k < 18; k++) {
        BMP_beginning[k] = getc(initial);
    }

    for (int i = 0; i < 4; i++) {
        uchar currentChar = getc(initial);
        BMP_beginning[i + 18] = currentChar;
        width += currentChar * pow(256, i);
    }

    for (int i = 0; i < 4; i++) {
        uchar currentChar = getc(initial);
        BMP_beginning[i + 22] = currentChar;
        height += currentChar * pow(256, i);
    }

    printf("Width:  %u\nHeight: %u\n", width, height);

    for (int k = 0; k < 2; k++) {
        BMP_beginning[k + 26] = getc(initial);
    }

    int bitCount = 0;

    do {
        uchar a = getc(initial), b = getc(initial);
        BMP_beginning[28] = a;
        BMP_beginning[29] = b;
        bitCount += a + b * 256;
    } while (0);

    if (bitCount != 1) {
        printf("Error: image isn't monochrome");
        return -1;
    }

    for (int k = 0; k < 32; k++) {
        BMP_beginning[k + 30] = getc(initial);
    }

    ulong pixelsNumber = width * height, pixelsCountdown = pixelsNumber;

    if (pixelsNumber == 0) {
        if (width == 0) {
            printf("Error: zero width\n");
            return -1;
        } else {
            printf("Error: zero height\n");
            return -1;
        }
    }

    uchar original_plan[width * height];

    ulong widthInBytes = ceil(width / 8.0);
    ulong widthInBytesMinus1 = widthInBytes - 1;
    uchar offsetBytes = 4 - widthInBytes % 4;
    while (offsetBytes >= 4) offsetBytes -= 4;
    uchar offsetBits = 0;

    for (ulong curLine = 0; curLine < height; curLine++) {
        for (ulong i = 0; i < widthInBytesMinus1; i++) {
            uchar now = getc(initial);
            for (char j = 7; j >= 0; j--) {
                if (now >= pow(2, j)) {
                    original_plan[i * 8 + 7 - j + curLine * width] = 0;
                    now -= (ulong) pow(2, j);
                } else {
                    original_plan[i * 8 + 7 - j + curLine * width] = 1;
                }
            }
        }

        ulong bitsLeftOnLine = width - widthInBytesMinus1 * 8;
        offsetBits = 8 - bitsLeftOnLine;
        uchar now = getc(initial);
        if (bitsLeftOnLine != 0) {
            for (char j = 7; j >= 0; j--) {
                if (now >= pow(2, j)) {
                    original_plan[widthInBytesMinus1 * 8 + 7 - j + curLine * width] = 0;
                    now -= pow(2, j);
                } else {
                    original_plan[widthInBytesMinus1 * 8 + 7 - j + curLine * width] = 1;
                }
                bitsLeftOnLine--;
                if (bitsLeftOnLine == 0) break;
            }
        }
        for (uchar m = 0; m < offsetBytes; m++) {
            char useless = getc(initial);
        }
    }

    fclose(initial);

    printf("\n%s\n", line);

    FILE *outputFileCurrent;
    uchar outputChar[widthInBytes * height];

    int dumpTimer = 0;

    ulong alive[iterationsNumber];
    ulong stepCount = 0;
    uchar copied_plan[width * height];
    char neighbours = 0, flagOscillator = 0;

    for (ulong i = 0; i < width * height; i++) {
        copied_plan[i] = original_plan[i];
    }

    iterationsNumber++;

    for (uint n = 0; n < iterationsNumber; n++) {
        dumpTimer++;

        alive[n] = 0;

        for (ulong i = 0; i < width * height; i++) {
            original_plan[i] = copied_plan[i];
            alive[n] += original_plan[i];
        }

        if (alive[n] == 0) {
            printf("Game over: all cells are dead.");
            break;
        }



        for (ulong i = 0; i < widthInBytes * height; i++) {
            outputChar[i] = 0;

            if ((i + 1) % widthInBytes != 0) {
                for (char j = 7; j >= 0; j--) {
                    if (original_plan[i * 8 + 7 - j - offsetBits * (i / widthInBytes)] == 0) {
                        outputChar[i] += pow(2, j);
                    }
                }
            } else {
                for (char k = 0; k < 8 - offsetBits; k++) {
                    if (original_plan[i * 8 + k - offsetBits * (i / widthInBytes)] == 0) {
                        outputChar[i] += pow(2, 7 - k);
                    }
                }
            }
        }



        for (ulong i = 0; i < n; i++) {
            if (alive[i] == alive[n]) {
                FILE *old;

                char *nameFile;
                char *temp;
                temp = malloc(9);
                sprintf(temp, "%d", i);
                strcat(temp, ".bmp");
                nameFile = malloc(strlen(temp));
                strcpy(nameFile, temp);

                if (strlen(nameFile) < 8)
                    for (char m = strlen(nameFile); m < 8; m++) {
                        temp = malloc(strlen(nameFile) + strlen("0"));
                        strcpy(temp, "0");
                        strcat(temp, nameFile);
                        nameFile = malloc(strlen(temp));
                        strcpy(nameFile, temp);
                    }

                temp = malloc(strlen(origOutputDir) + strlen("\\") + strlen(nameFile));
                strcpy(temp, origOutputDir);

                char *temp2 = malloc(strlen(temp) + strlen((nameFile)));
                strcpy(temp2, temp);
                strcat(temp2, nameFile);
                nameFile = malloc(strlen(temp2));
                strcpy(nameFile, temp2);

                old = fopen(nameFile, "rb");

                for (char m = 0; m < 62; m++) {
                    char useless = fgetc(old);
                }

                for (ulong m = 0; m < widthInBytes * height; m++) {
                    uchar oldChar = fgetc(old);
                    if (oldChar != outputChar[m]) {
                        break;
                    }
                    if ((m + 1) % widthInBytes == 0) {
                        for (char k = 0; k < offsetBytes; k++) {
                            uchar useless = fgetc(old);
                        }
                    }
                    if ((m + 1) == widthInBytes * height) {
                        printf("This life simulation became an oscillator with a period of %d, started on %d.\n", n - i, i);
                        flagOscillator = 1;
                        break;
                    }
                }

                if (flagOscillator != 0) {
                    break;
                }

                fclose(old);

                // 62 + widthInBytes * height
            }
        }

        if (flagOscillator != 0) {
            break;
        }

        //сама игра жизнь

        for (int row = 0; row < height; row++) {
            for (int which = 0; which < width; which++) {

                neighbours = 0;
                int thisOne = row * width + which;

                if (row + 1 < height) {
                    neighbours += original_plan[thisOne + width];
                    if (which > 0) neighbours += original_plan[thisOne + width - 1];
                    if (which + 1 < width) neighbours += original_plan[thisOne + width + 1];
                }
                if (row > 0) {
                    neighbours += original_plan[thisOne - width];
                    if (which > 0) neighbours += original_plan[thisOne - width - 1];
                    if (which + 1 < width) neighbours += original_plan[thisOne - width + 1];
                }
                if (which > 0) neighbours += original_plan[thisOne - 1];
                if (which + 1 < width) neighbours += original_plan[thisOne + 1];

                // следующее поколение

                if (original_plan[thisOne] != 0) {
                    if ((neighbours < 2) || (neighbours > 3))
                        copied_plan[thisOne] = 0;
                } else if (neighbours == 3)
                    copied_plan[thisOne] = 1;
            }
        }

        if (dumpTimer % dumpFrequency == 0) {
            dumpTimer = 0;

            // Выбрал нумерацию через номер шага, а не последовательную, поскольку она более удобна для использования
            stepCount = n;

            do {
                char *numNow = "0000";
                char digit[5];

                sprintf(digit, "%d", stepCount);

                char *temp;

                if (stepCount < 10) {
                    numNow = "000";
                    temp = malloc(strlen(numNow));
                    strcpy(temp, numNow);
                    numNow = malloc(strlen(numNow) + strlen(digit));
                    strcpy(numNow, temp);
                } else if (stepCount < 100) {
                    numNow = "00";
                    temp = malloc(strlen(numNow));
                    strcpy(temp, numNow);
                    numNow = malloc(strlen(numNow) + strlen(digit));
                    strcpy(numNow, temp);
                } else if (stepCount < 1000) {
                    numNow = "0";
                    temp = malloc(strlen(numNow));
                    strcpy(temp, numNow);
                    numNow = malloc(strlen(numNow) + strlen(digit));
                    strcpy(numNow, temp);
                } else {
                    numNow = malloc(strlen(digit));
                }

                strcat(numNow, digit);

                outputDir = malloc(strlen(origOutputDir));
                strcpy(outputDir, origOutputDir);

                if (outputDir[strlen(outputDir) - 1] != '\\') {
                    temp = malloc(outputDir);
                    strcpy(temp, outputDir);
                    outputDir = malloc(strlen((outputDir) + strlen("\\")));
                    strcpy(outputDir, temp);
                    strcat(outputDir, "\\");
                }

                temp = malloc(outputDir);
                strcpy(temp, outputDir);
                outputDir = malloc(strlen((outputDir) + strlen(numNow)));
                strcpy(outputDir, temp);
                strcat(outputDir, numNow);

                temp = malloc(outputDir);
                strcpy(temp, outputDir);
                outputDir = malloc(strlen((outputDir) + strlen(".bmp")));
                strcpy(outputDir, temp);
                strcat(outputDir, ".bmp");

                outputFileCurrent = fopen(outputDir, "wb");

                for (uchar i = 0; i < 62; i++) {
                    fputc(BMP_beginning[i], outputFileCurrent);
                }

                //here

                do {
                    for (ulong i = 0; i < widthInBytes * height; i++) {

                        if ((i + 1) % widthInBytes != 0) {
                            fputc(outputChar[i], outputFileCurrent);
                        } else {
                            fputc(outputChar[i], outputFileCurrent);
                            for (char k = 0; k < offsetBytes; k++) {
                                fputc(0, outputFileCurrent);
                            }
                        }
                    }

                } while (0); // кластер перевода массива и записи в файл текущей итерации
                fclose(outputFileCurrent);
            } while (0);
        }
    }

    return 0;
}
