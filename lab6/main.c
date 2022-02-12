#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define buffer_size 1048576 // 1Mb

typedef struct file_metadata
{
    int file_size;
    char *file_name;
} file_metadata;

void create_arc(char *arc_name, int argc, char *argv[])
{
    FILE *fout = fopen(arc_name, "wb");
    char n = argc - 4;
    fputc(n, fout);

    for (int i = 4; i < argc; i++)
    {
        FILE *fin = fopen(argv[i], "rb");

        char name_length = strlen(argv[i]);
        fputc(name_length, fout);

        int file_size;
        fseek(fin, 0, SEEK_END);
        file_size = ftell(fin);
        fseek(fin, 0, SEEK_SET);
        fwrite(&file_size, sizeof(int), 1, fout);

        fwrite(argv[i], 1, name_length, fout);

        char *data = (char *)malloc(file_size);
        fread(data, 1, file_size, fin);
        fwrite(data, 1, file_size, fout);

        free(data);
        fclose(fin);
    }
    fclose(fout);
}

file_metadata get_file_metadata(FILE *fin)
{
    char name_length;
    fread(&name_length, 1, 1, fin);

    int file_size;
    fread(&file_size, sizeof(int), 1, fin);

    char *file_name = (char *)malloc(name_length + 1);
    file_name[name_length] = '\0';

    fread(file_name, 1, name_length, fin);

    file_metadata data;
    data.file_size = file_size;
    data.file_name = file_name;

    return data;
}

void get_list(char *arc_name)
{
    FILE *arc = fopen(arc_name, "rb");
    char n;
    fread(&n, 1, 1, arc);

    for (int i = 0; i < n; i++)
    {
        file_metadata metadata = get_file_metadata(arc);

        fseek(arc, metadata.file_size, SEEK_CUR);
        printf("%s ", metadata.file_name);
        free(metadata.file_name);
    }
    fclose(arc);
}

void extract_arc(char *arc_name)
{
    FILE *arc = fopen(arc_name, "rb");
    char n;
    fread(&n, 1, 1, arc);

    for (int i = 0; i < n; i++)
    {
        file_metadata metadata = get_file_metadata(arc);

        FILE *out = fopen(metadata.file_name, "wb");
        char buffer[buffer_size];
        int mod = metadata.file_size;
        while (mod > buffer_size)
        {
            fread(&buffer, 1, buffer_size, arc);
            fwrite(&buffer, 1, buffer_size, out);
            mod -= buffer_size;
        }
        fread(&buffer, 1, mod, arc);
        fwrite(&buffer, 1, mod, out);

        free(metadata.file_name);
        fclose(out);
    }
    fclose(arc);
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        printf("Error, not enough arguments\n");
        return -1;
    }

    char *arc_name;

    for (int i = 1; i < 4; i += 2)
    {
        if (strcmp(argv[i], "--file") == 0)
        {
            arc_name = argv[i + 1];
        }
        else if (strcmp(argv[i], "--create") == 0)
        {
            create_arc(arc_name, argc, argv);
        }
        else if (strcmp(argv[i], "--extract") == 0)
        {
            extract_arc(arc_name);
        }
        else if (strcmp(argv[i], "--list") == 0)
        {
            get_list(arc_name);
        }
        else
        {
            printf("Error, wrong format\n");
            return -1;
        }
    }

    return 0;
}