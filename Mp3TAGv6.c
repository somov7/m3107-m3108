#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct id3v2_header{
    unsigned char id[3];
    unsigned char version;
    unsigned char subversion;
    unsigned char flags;
    unsigned char size[4];
} id3v2_header;

typedef struct frame_header{
    unsigned char id[4];
    unsigned char size[4];
    unsigned char flags[2];
}frame_header;

void into_frame_size(uint32_t new_size, unsigned char *res){
    for(int i = 3; i > -1; i--){
        res[i] = new_size % 128;
        new_size /= 128;
    }
}

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "Russian");

    if (argc != 3 && argc != 4){
        printf("wrong number of parameters: %d\n", argc);
        return 1;
    }

    char *file_name;
    file_name = argv[1] + 11;
    printf("%s\n", file_name);
    FILE *test_mp3;
    test_mp3 = fopen(file_name, "r+b");

    if (test_mp3 == NULL){
        printf("Can not open file %s\n", file_name);
        return 1;
    }

    uint8_t do_show = 0, do_get = 0, do_set = 0;
    char *field_ptr;
    char *value_ptr;
    uint32_t value_size = 0;

    // test_mp3 = fopen("C418 - Sweden (Minecraft OST).mp3", "rb");

    for (int i = 2; i < argc; i++){
        // printf("%s\n", argv[i]);
        if (strcmp(argv[i], "--show") == 0){
            do_show = 1;
            break;
        }
        else if (strncmp(argv[i], "--get=", 6) == 0){
            do_get = 1;
            field_ptr = argv[i] + 6;
            // printf("%s\n", field_ptr);
            break;
        }
        else if (strncmp(argv[i], "--set=", 6) == 0){
            do_set = 1;
            field_ptr = argv[i] + 6;
            printf("%s\n", field_ptr);
        }
        else if (strncmp(argv[i], "--value=", 8) == 0){
            value_ptr = argv[i] + 8;
            value_size = strlen(value_ptr);
            printf("%s\n", value_ptr);
        }
        else{
            printf("Wrong ");
        }
    }

    uint32_t header_size = 0;
    char header_id[4];
    id3v2_header header;

    fread(&header, sizeof(char), sizeof(header), test_mp3);
    header_size = header.size[0] * 0x200000 +   //2^21
            header.size[1] * 0x4000 +     //2^14
            header.size[2] * 0x80 +       //2^7
            header.size[3] + 10;
    for (int i = 0; i < 3; i++){
        header_id[i] = header.id[i];
    }
    header_id[3] = '\0';
    printf("header: %sv2.%x.%x; %d bytes\n", header_id,  header.version, header.subversion, header_size);

    // printf("value size: %d\n", value_size);
    uint8_t end_of_frames = 0;
    uint32_t byte_sum = 0;
    while(byte_sum < header_size){
        uint64_t frame_size = 0;
        char frame_id[5];
        char frame_encoding;
        frame_header frame1;
        fread(&frame1, sizeof(char), sizeof(frame1), test_mp3);

        for (int i = 0; i < 4; i++){
            // printf("%x\n", frame1.id[i]);
            if ((frame1.id[i] >= '0' && frame1.id[i] <= '9') || (frame1.id[i] >= 'A' && frame1.id[i] <= 'Z')){
                frame_id[i] = frame1.id[i];
            }
            else{
                end_of_frames = 1;
                break;
            }
        }

        if (end_of_frames){
            // printf("end\n");
            break;
        }

        frame_id[4] = '\0';

        frame_size = frame1.size[0] * 0x200000 +   //2^21
                frame1.size[1] * 0x4000 +     //2^14
                frame1.size[2] * 0x80 +       //2^7
                frame1.size[3];

        byte_sum += 10;
        fread(&frame_encoding, sizeof(char), sizeof(frame_encoding), test_mp3);

        if (do_set && (strcmp(field_ptr, frame_id) == 0)){
            into_frame_size(value_size + 1, frame1.size);
            // store current position
            uint32_t file_ptr = ftell(test_mp3);

            // printf("file pointer: %ld\n", ftell(test_mp3));

            fseek(test_mp3, frame_size - 1, SEEK_CUR);

            // printf("file pointer: %ld\n", ftell(test_mp3));

            char *buf = calloc(header_size - file_ptr - value_size, sizeof (char));
            fread(buf, sizeof (char), header_size - file_ptr - value_size, test_mp3);
            // printf("%s   size: %lld\n",frame_id, frame_size);
            // printf("file pointer: %ld\n", ftell(test_mp3));
            fflush(test_mp3);
            // write new encoding
            char x[1];
            x[0] = '\0';
            fseek(test_mp3, file_ptr - 1, SEEK_SET);
            // printf("file pointer_enc: %ld\n", ftell(test_mp3));

            fwrite(x, sizeof(char), 1, test_mp3);
            fflush(test_mp3);
            // write new value
            // printf("file pointer_val: %ld\n", ftell(test_mp3));
            fseek(test_mp3, file_ptr, SEEK_SET);
            fwrite(value_ptr, sizeof(char), value_size, test_mp3);

            // write buffer
            // printf("file pointer_buf: %ld\n", ftell(test_mp3));
            fwrite(buf, sizeof (char), header_size - file_ptr - value_size, test_mp3);

            // write new frame size
            fseek(test_mp3, file_ptr - 7, SEEK_SET);
            fwrite(frame1.size, sizeof (char), sizeof (frame1.size), test_mp3);
            fflush(test_mp3);
            fclose(test_mp3);
            free(buf);
            return 0;
        }
        // printf("file pointer: %ld\n", ftell(test_mp3));
        // printf("encoding is: %x\n", frame_encoding);
        if (frame_encoding == 0 || frame_encoding == 3){
            if (do_show){
                char temp[frame_size];
                fgets(temp, (int)frame_size, test_mp3);
                printf("%s %s\n",frame_id, temp);
            }
            else if (do_get && (strcmp(field_ptr, frame_id) == 0)){
                char temp[frame_size];
                fgets(temp, (int)frame_size, test_mp3);
                printf("%s %s\n",frame_id, temp);
            }
            else if (do_set){
                char temp[frame_size];
                fgets(temp, (int)frame_size, test_mp3);
            }
        }

        else if (frame_encoding == 1){
            unsigned char bom[3];
            fread(&bom, sizeof(char), 2, test_mp3);
            bom[2] = '\0';
            if (do_show){
                wchar_t temp[frame_size];
                fgetws(temp, (int)frame_size / 2, test_mp3);
                // printf("%s ", frame_id);
                wprintf(L"%ls\n", temp);
            }
            else if (do_get && (strcmp(field_ptr, frame_id) == 0)){
                wchar_t temp[frame_size];
                fgetws(temp, (int)frame_size / 2, test_mp3);
                // printf("%s ", frame_id);
                wprintf(L"%ls\n", temp);
            }
            else if (do_set){
                wchar_t temp[frame_size];
                fgetws(temp, (int)frame_size / 2, test_mp3);
            }

        }
        else if (frame_encoding == 2){
            if (do_show){
                wchar_t temp[frame_size];
                fgetws(temp, (int)frame_size / 2, test_mp3);
                wprintf(L"%ls\n", temp);
            }
            else if (do_get && (strcmp(field_ptr, frame_id) == 0)){
                wchar_t temp[frame_size];
                fgetws(temp, (int)frame_size / 2, test_mp3);
                wprintf(L"%ls\n", temp);
            }
            else if (do_set){
                wchar_t temp[frame_size];
                fgetws(temp, (int)frame_size / 2, test_mp3);
            }
        }

        byte_sum += frame_size;
    }

    fclose(test_mp3);
    printf("completed successfully\n");
    return 0;
}