#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <SDL2/SDL.h>




unsigned int bytes_to_int(FILE *ptr, int n) {
    unsigned int output = 0;
    unsigned char *temp = malloc(sizeof(unsigned char) * n);
    for (int i = 0; i < n; i++) {
        temp[i] = fgetc(ptr);
        output += (0 + temp[i]) << (i * 8);
    }
    free(temp);
    return output;
}

void to_img(int width, int height, unsigned char *ptr) {
    for (int i = height - 1; i >= 0; i--) {
        for (int j = 0; j < width; j++) {
            if (ptr[i * width + j] == 0) {
                printf("*");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

unsigned char *assign_output_field(int width, int height, FILE *ptr, unsigned int array_offset) {
    fseek(ptr, array_offset, SEEK_SET);
    unsigned char *temp = malloc(sizeof(unsigned char) * width * height);
    unsigned long cnter = 0;
    int tem = width;
    unsigned char c;
    int padding =  (4 - (int)ceil((double)(width % 32) / 8)) % 4;
    for (int i = 0; i < height; i++) {
        if (tem > 8) {
            while ((tem / 8) > 0) {
                c = fgetc(ptr);
                for (int k = 7; k >= 0; k--) {
                    temp[cnter] = (c >> k) % 2;
                    cnter++;
                }
                tem -= 8;
            }
        }
        if (tem != 0) {
            c = fgetc(ptr);
            int k = 7;
            while (tem > 0) {
                temp[cnter] = (c >> k) % 2;
                cnter++;
                tem--;
                k--;
            }
        }
        tem = width;
        fseek(ptr, padding, SEEK_CUR);
    }
    return temp;
}

void next_gen_field(int width, int height, unsigned char *prev_gen_field, unsigned char *updated_field) {
    int neighbours[3] = {-1, 0, 1};
    for (unsigned int i = 0; i < width * height; i++) {
        int neighbour_count = 0;
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {

                int n_width = ((int) (i % width) + neighbours[k] + width) % (width);
                int n_height = ((int) (i / width) + neighbours[j]) % height;
                n_height = n_height < 0 ? (((n_height + height) % height) * width) : (n_height * width);
                unsigned int coords = n_width + n_height;
                neighbour_count+= prev_gen_field[coords];
            }
        }
        if (prev_gen_field[i] == 1 && neighbour_count == 6) {
            updated_field[i] = 0;
        } else if (prev_gen_field[i] == 0 && (neighbour_count < 5 || neighbour_count > 6)) {
            updated_field[i] = 1;
        } else {
            updated_field[i] = prev_gen_field[i];
        }
    }
}

void save_img(int width, int height, char *argv2, char *argv1, int raw_size, unsigned int array_offset,
              unsigned char *output_field, unsigned int size) {
    FILE *rptr = fopen(argv1, "rb");
    FILE *end = fopen(argv1, "rb");
    unsigned char *buffer = calloc(size + 1, sizeof(unsigned char));
    fread(buffer, 1, array_offset, rptr);
    fseek(rptr, array_offset, SEEK_SET);
    unsigned char c = 0;
    unsigned char k = 0;
    long long counter = 0;
    long long idk = array_offset;
    int temp = width;
    int padding =  (4 - (int)ceil((double)(width % 32) / 8))%4;
    for (int i = 0; i < height; i++) {
        while (temp / 8 > 0) {
            for (int j = 7; j >= 0; j--) {
                c = output_field[counter++];
                k |= (c << j);
            }
            temp -= 8;
            buffer[idk++] = k;
            k = 0;
        }
        if (temp != 0)
        {
            for (int j = 7; j > (7 - temp); j--) {
                c = output_field[counter++];
                k |= (c << j);
            }
            buffer[idk++] = k;
        }
        for (int p = 0; p < padding; p++) {
           buffer[idk++] = 120;
        }
        temp = width;
    }

    fseek(end, 0, SEEK_END);

    fpos_t temp2;
    fgetpos(end, &temp2);
    fseek(end, array_offset + raw_size , SEEK_SET);
    fpos_t temp1;
    fgetpos(end, &temp1);
    fread(buffer + (array_offset + raw_size), 1, temp2 - temp1, rptr);

    fclose(rptr);
    fclose(end);
    FILE *wptr = fopen(argv2, "wb");
    fwrite(buffer, 1, size , wptr);

    free(buffer);
    
    fclose(wptr);
}

int main(int argc, char *argv[]) {
    if (argc > 3) {
        
        char *input_file;
        char *output_file;
        int max_iter = 100;
        int dump_freq = 1;
        for (int i = 1; i < argc; i += 2)
        {
            if (strcmp(argv[i], "--input") == 0)
            {
                input_file = argv[i+1];
            }
            if (strcmp(argv[i], "--output") == 0)
            {
                output_file = argv[i+1];
            }
            if (strcmp(argv[i], "--max_iter") == 0)
            {
                max_iter = atoi(argv[i+1]);
            }
            if (strcmp(argv[i], "--dump_freq") == 0)
            {
                dump_freq = atoi(argv[i + 1]);
            }
        }
        printf("%s\n", input_file);

        FILE *ptr = fopen(input_file, "rb");
        if (ptr != NULL) {
            
            
            fseek(ptr, 2, SEEK_SET);
            unsigned int size = bytes_to_int(ptr, 4);
            fseek(ptr, 10, SEEK_SET);
            unsigned int array_offset = bytes_to_int(ptr, 4);
            
            fseek(ptr, 18, SEEK_SET);
            int width = bytes_to_int(ptr, 4);
            int height = bytes_to_int(ptr, 4);
            fseek(ptr, 34, SEEK_SET);
            int raw_size = bytes_to_int(ptr, 4);
            
            SDL_Init(SDL_INIT_VIDEO);
            SDL_Window *screen = NULL; 
            SDL_Surface *image = NULL;   
            SDL_Surface *gScreenSurface = NULL;
            SDL_Event *event = NULL;
            screen = SDL_CreateWindow("The Conway's Game of Life", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, width , height, SDL_WINDOW_ALLOW_HIGHDPI);

            gScreenSurface = SDL_GetWindowSurface(screen);            
            unsigned char *output = assign_output_field(width, height, ptr, array_offset);
            unsigned char *update = malloc(sizeof(unsigned char) * width * height);
            fclose(ptr);
            for (int i = 0; i < max_iter; i++) {
                image = SDL_LoadBMP(output_file);
                
                SDL_BlitSurface( image, NULL, gScreenSurface, NULL );
                SDL_UpdateWindowSurface(screen);

                save_img(width, height, output_file, input_file, raw_size, array_offset, output, size);
                
                SDL_FreeSurface(image);            

                next_gen_field(width, height, output, update);
                memcpy(output, update, width * height);
            }
        }
        else
        {
            printf("Error opening the file");
        }
    } else {
        printf("Might as well print a bit more arguments, fella.");
    }
    printf("Done!");
    return 0;
}
