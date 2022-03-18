#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define ID3_MAX_SIZE 128
struct ID3TAG
{
    char TAG[3];
    char title[30];
    char artist[30];
    char album[30];
    char year[4];
    char comment[30];
    char genre;
};
char* getTag(char* string, const short len)
{
    char* result = (char*) malloc(sizeof(char) * (len + 1));
    memset(result, 0, len + 1);
    strncat(result, string, len);
    return result;
}
int genre_int(char* str)
{
    char len = 4, result = 0;
    for (int i = 0; i < len; i++)
    {
        if (str[i] > 47 && str[i] < 58)
        {
            result *= 10;
            result += (int)str[i] - 48;
        }
        else return result;
    }
    return result;
}
int main(int argc, char* argv[]) {
    FILE *mp3_file = NULL;
    char *mp3_name;
    char *tag;
    char *val;
    struct ID3TAG *Frames = NULL;
    if (!strncmp(argv[1], "--filepath=", 11)) {
        mp3_name = strpbrk(argv[1], "=") + 1;
        if ((mp3_file = fopen(mp3_name, "rb+")) == NULL) {
            printf("Unable to open file: %s \n", mp3_name);
            return 0;
        }
    }
    char *temp = (char *) malloc(sizeof(char) * ID3_MAX_SIZE);
    memset(temp, 0, ID3_MAX_SIZE);
    fseek(mp3_file, -128, SEEK_END);
    fread(temp, 1, ID3_MAX_SIZE, mp3_file);
    Frames = (struct ID3TAG *) (temp);
    if (!strcmp("--show", argv[2])) {
        printf("Song title: %s\n", getTag(Frames->title, 30));
        printf("Artist: %s\n", getTag(Frames->artist, 30));
        printf("Album: %s\n", getTag(Frames->album, 30));
        printf("Year: %s\n", getTag(Frames->year, 4));
        printf("Comment: %s\n", getTag(Frames->comment, 30));
        printf("Genre: %d\n", Frames->genre);
    } else if (!strncmp("--get=", argv[2], 6)) {
        tag = strpbrk(argv[2], "=") + 1;
        if (!strcmp(tag, "title"))
            printf("Song title: %s\n", getTag(Frames->title, 30));
        else if (!strcmp(tag, "artist"))
            printf("Artist: %s\n", getTag(Frames->artist, 30));
        else if (!strcmp("album", tag))
            printf("Album: %s\n", getTag(Frames->album, 30));
        else if (!strcmp("year", tag))
            printf("Year: %s\n", getTag(Frames->year, 4));
        else if (!strcmp("comment", tag))
            printf("Comment: %s\n", getTag(Frames->comment, 30));
        else if (!strcmp("genre", tag))
            printf("Genre: %d\n", Frames->genre);
        else {
            printf("Unknown tag\n");
            return 0;
        }
    } else if (!strncmp("--set=", argv[2], 6)) {
        tag = strpbrk(argv[2], "=") + 1;
        if (!strncmp("--value=", argv[3], 8)) {
            val = strpbrk(argv[3], "=") + 1;
            if (!strcmp(tag, "title"))
                memcpy(Frames->title, val, 30);
            else if (!strcmp(tag, "artist"))
                memcpy(Frames->artist, val, 30);
            else if (!strcmp(tag, "album"))
                memcpy(Frames->album, val, 30);
            else if (!strcmp(tag, "year"))
                memcpy(Frames->year, val, 4);
            else if (!strcmp(tag, "comment"))
                memcpy(Frames->comment, val, 30);
            else if (!strcmp(tag, "genre"))
                Frames->genre = (char) genre_int(val);
        } else {
            printf("Incorrect value\n");
            return 0;
        }
    } else {
        printf("Unknown expression: %s\n", argv[2]);
        return 0;
    }
    fflush(mp3_file);
    if (!strncmp("--set=", argv[2], 6)) {
        temp = (char *) Frames;
        fseek(mp3_file, -128, SEEK_END);
        fwrite(temp, ID3_MAX_SIZE, 1, mp3_file);
        fclose(mp3_file);
    }
    return 0;
}