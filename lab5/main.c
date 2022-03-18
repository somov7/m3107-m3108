#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
// добавить чекер наличия файла в мейн
//
//
//
//

typedef struct BMP {
    unsigned long long  width;
    unsigned long long height;
    unsigned long long size;
    unsigned long long size_to_read;
    unsigned char ** array;
    int checker;
} BMP;

int findneighbors(BMP new, int pos_y, int pos_x) {
    int neighbors = 0;
    if (pos_y == 0) {
        if (pos_x == 0) {
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    if ((new.array[pos_y + i][pos_x + j] == 1) && ((i!= 0) && (j != 0)))
                        neighbors++;
                }
            }
        }
        else if (pos_x == new.width - 1) {
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    if ((new.array[pos_y + i][pos_x + j - 1] == 1) && ((i!= 0) && (j != 1)))
                        neighbors++;
                }
            }
        }
        else {
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 3; j++) {
                    if ((new.array[pos_y + i][pos_x + j - 1] == 1) && ((i!= 0) && (j != 1)))
                        neighbors++;
                }
            }
        }
    }
    else if (pos_x == 0) {
        if (pos_y == 0) {
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    if ((new.array[pos_y + i][pos_x] == 1) && ((i!= 0) && (j != 0)))
                        neighbors++;
                }
            }
        }
        else if (pos_y == new.height - 1) {
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    if ((new.array[pos_y + i - 1][pos_x + j] == 1) && ((i!= 1) && (j != 0)))
                        neighbors++;
                }
            }
        }
        else {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 2; j++) {
                    if ((new.array[pos_y + i - 1][pos_x + j] == 1) && ((i!= 1) && (j != 0)))
                        neighbors++;
                }
            }
        }
    }
    else if (pos_y == new.height - 1) {
        if (pos_x == 0) {
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    if ((new.array[pos_y + i - 1][pos_x + j] == 1) && ((i!= 1) && (j != 0)))
                        neighbors++;
                }
            }
        }
        else if (pos_x == new.width - 1) {
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    if ((new.array[pos_y + i - 1][pos_x + j - 1] == 1) && ((i!= 0) && (j != 0)))
                        neighbors++;
                }
            }
        }
        else {
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 3; j++) {
                    if ((new.array[pos_y + i - 1][pos_x + j - 1] == 1) && ((i!= 1) && (j != 1)))
                        neighbors++;
                }
            }
        }
    }
    else if (pos_x == new.width - 1) {
        if (pos_y == 0) {
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    if ((new.array[pos_y + i][pos_x + j - 1] == 1) && ((i!= 0) && (j != 1)))
                        neighbors++;
                }
            }
        }
        if (pos_y == new.height - 1) {
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    if ((new.array[pos_y + i - 1][pos_x + j - 1] == 1) && ((i!= 0) && (j != 0)))
                        neighbors++;
                }
            }
        }
        else {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 2; j++) {
                    if ((new.array[pos_y + i - 1][pos_x + j - 1] == 1) && ((i!= 1) && (j != 1)))
                        neighbors++;
                }
            }
        }
    }
    else {
        for (int i = 0; i<3; i++) {
            for (int j = 0; j < 3; j++) {
                if ((new.array[pos_y + i - 1][pos_x + j - 1] == 1) && ((i!= 1) || (j != 1)))
                    neighbors++;
            }
        }
    }
    
    return neighbors;
}

void lifegame(BMP old, BMP new) {
    int neighbors;
    for (int i = 0; i < old.height; i++) {
        for (int j = 0; j < old.width; j++) {
            neighbors = findneighbors(old, i, j);
            if (old.array[i][j] == 1) {
                if (neighbors >= 2 && neighbors < 4)
                    new.array[i][j] = 1;
                else
                    new.array[i][j] = 0;
            }
            else {
                if (neighbors == 3)
                    new.array[i][j] = 1;
                else
                    new.array[i][j] = 0;
            }
        }
    }
}

BMP reading(char *name) {
    FILE *fin = fopen(name, "rb");
    BMP bmp;
    bmp.checker = 0;
    unsigned char temp;
    unsigned long long multiplicity = 0;
    if (fin == NULL) {
        fprintf(stderr, "%s", "ERROR: the file does not exist");
        bmp.checker = 1;
        return bmp;
    }
    bmp.height = 0;
    bmp.width = 0;
    for (int i = 0; i<2; i++)
        fgetc(fin);
    for (int i = 0; i < 4; i++) {
        temp = fgetc(fin);
        bmp.size += temp << 8 * i;
    }
    for (int i = 0; i<12; i++)
        fgetc(fin);
    for (int i = 0; i<4; i++) {
        temp = fgetc(fin);
        bmp.width += temp << 8 * i;
    }
    for (int i = 0; i<4; i++) {
        temp = fgetc(fin);
        bmp.height += temp << 8 * i;
    }
    for (int i = 0; i<36; i++)
        fgetc(fin);
    bmp.size_to_read = bmp.size - 62;
    bmp.array = (unsigned char **)malloc(bmp.height * sizeof(unsigned char*));
    for (int i = 0; i < bmp.height; i++)
        bmp.array[i] = (unsigned char *)malloc(bmp.width * sizeof(unsigned char));
    if ((bmp.width % 8) == 0)
        multiplicity = bmp.width / 8;
    else
        multiplicity = bmp.width / 8 + 1;
    for (int i = bmp.height - 1; i >= 0; i--) {
        for (int j = 0; j < multiplicity; j++) {
            unsigned char current_char = fgetc(fin);
            if (j != multiplicity - 1) {
                for (int k = 0; k < 8; k++)
                    bmp.array[i][j * 8 + k] = (current_char >> (7-k)) % 2;
            }
            else if (j == multiplicity - 1) {
                for (int k = 0; k < bmp.width - (multiplicity-1) * 8; k++)
                    bmp.array[i][j * 8 + k] = (current_char >> (7-k)) % 2;
            }
        
            }
        if (multiplicity % 4 != 0) {
            for(int k = 0; k < 4 - (multiplicity % 4); k++)
                fgetc(fin);
        }
    }
    return bmp;
}
void newbmp(BMP bmp, char *old_file, char *new_file) {
    unsigned long long multiplicity = 0;
    
    FILE *fin;
    fin = fopen(old_file, "rb");
    FILE *fout;
    fout = fopen(new_file, "wb");
    for (int i = 0; i < 62; i++)
    fputc(fgetc(fin), fout);
    if ((bmp.width % 8) == 0)
        multiplicity = bmp.width / 8;
    else
        multiplicity = bmp.width / 8 + 1;
    for (int i = bmp.height - 1; i >= 0; i--) {
        for (int j = 0; j < multiplicity; j++) {
            unsigned char current_char = 0;
            if (j != multiplicity - 1) {
                for (int k = 7; k >= 0; k--)
                    current_char += bmp.array[i][j* 8 + 7 - k] << k;
            }
            else if (j == multiplicity - 1) {
                for (int k = 7; k >= 8-(int)(bmp.width-(multiplicity-1)*8); k--)
                    current_char += bmp.array[i][j * 8 + 7 - k] << k;
            }
            fputc((int)current_char,fout);
        }
        if (multiplicity % 4 != 0) {
            for(int k = 0; k < 4 - (multiplicity % 4); k++)
                fputc(0, fout);
        }
    }
}

int main(int argc, char **argv) {
    setlocale(LC_ALL, "Rus");
    char* filename;
    char* directory;
    int max_interations = 0;
    int save_images = 1;
    int file_derictory_checker = 0;
    if (argc % 2 == 0) {
        fprintf(stderr, "%s", "ERROR: invalid number of arguments");
        return 1;
    }
    for (int i = 1; i < argc - 1; i = i + 2) {
        if (memcmp(argv[i], "--input",7) == 0) {
            filename = argv[i + 1];
            file_derictory_checker++;
        }
        else if (memcmp(argv[i], "--output",8) == 0) {
            directory = argv[i + 1];
            file_derictory_checker++;
        }
        else if (memcmp(argv[i], "--max_iter",10) == 0)
            max_interations = atoi(argv[i + 1]);
        else if (memcmp(argv[i], "--dump_freq",11) == 0)
            save_images = atoi(argv[i + 1]);
    }
    if (file_derictory_checker != 2) {
        fprintf(stderr, "%s", "ERROR: there is not derictory or file");
    }
    //filename = "/Users/mikhno/prog/progaLAB/LAB5/im.bmp";
    //directory = "/Users/mikhno/prog/progaLAB/LAB5/";
    BMP old = reading(filename);
    if (old.checker == 1) {
        return 1;
    }
    BMP new;
    //    int N = 5;
    // max_interations = 5;
    char new_image_adress[100];
    char name_of_file[100];
    new.height = old.height;
    new.width = old.width;
    new.array = (unsigned char **)malloc(new.height * sizeof(unsigned char *));
    for (int i = 0; i < old.height; i++)
        new.array[i] = (unsigned char *)malloc(new.width * sizeof(unsigned char));
    //  FILE *fin;
    //  fin = fopen(argv[3], "rb");
    //  fread(new_image, sizeof(char), 62, fin)
    for (int i = 0; i<max_interations; i++) {
            memmove(new_image_adress,directory, strlen(argv[4])+1);
            sprintf(name_of_file, "%d", i+1);
            if ((i+1)%save_images == 0) {
                lifegame(old, new);
                for (int i = 0; i<new.height; i++) {
                    for (int j = 0; j<new.width; j++) {
                        old.array[i][j] = new.array[i][j];
                    }
            }
                strncat(name_of_file, ".bmp ", 5);
                strncat(new_image_adress, name_of_file, strlen(name_of_file)+1);
                newbmp(new, argv[2], new_image_adress);
       }
        
    }
    return 0;
}
