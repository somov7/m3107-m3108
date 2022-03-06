#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char tag[3];
    char title[30];
    char track[30];
    char artist[30];
    char album[30];
    char year[4];
    char comment[30];
    unsigned char genre;
} mp3Info;

int main (int argc, char *argv[])
{
  if ( argc < 2 )
  {
    printf("Arguments error");
    exit(0);
  }

  char *filepath, *get, *set;
  int show = 0;
  int argIndex = 1;

  for ( ; argIndex < argc; argIndex++ )
  {
    if (strcmp(argv[argIndex], "--show") == 0)
    {
      show = 1;
    } else
    {
      char *name = strtok(argv[argIndex], "=");
      char *value = strtok(NULL, "=");

      if (strcmp(name, "--filepath") == 0)
      {
        filepath = value;
      } else if (strcmp(name, "--get") == 0)
      {
        get = value; 
      } else if (strcmp(name, "--set") == 0)
      {
        set = value;
      }
    }
  }
  
  FILE *file = fopen(filepath, "rb");

  if (file == NULL)
  {
    printf("Cannot open file: %s\n");
    exit(0);
  } else
  {
    mp3Info tags;
    if (fseek(file, sizeof(mp3Info), SEEK_END) == -1)
    {
      fprintf(stderr, "File is not mp3");
      return EXIT_FAILURE;
    }
    if (fread(&tags, sizeof(mp3Info), 1, file) != 1)
    {
      printf("Could not read the tag\n");
      exit (0);
    }
    if (show == 1)
    {
      if (memcmp(tags.tag, "TAG", 3) == 0)
      {
        printf("Title: %.30s\n", tags.title);
        printf("Artist: %.30s\n", tags.artist);
        printf("Album: %.30s\n", tags.album);
        printf("Year: %.4s\n", tags.year);
        if (tags.comment[28] == '\0')
        {
          printf("Comment: %.28s\n", tags.comment);
          printf("Track: %d\n", tags.comment[29]);
        } else
        {
          printf("Comment: %.30s\n", tags.comment);
        }
        printf("Genre: %d\n", tags.genre);
      } else
      {
        fprintf(stderr, "Cannot find Tags\n");
        return EXIT_FAILURE;
      }
    } else if (strcmp(get, "track") == 0)
    {
      printf("Track: %.30s\n", tags.track);
    } else if (strcmp(get, "title") == 0)
    {
      printf("Title: %.30s\n", tags.title);
    } else if (strcmp(get, "artist") == 0)
    {
      printf("Title: %.30s\n", tags.artist);
    } else if (strcmp(get, "album") == 0)
    {
      printf("Title: %.30s\n", tags.album);
    } else if (strcmp(get, "year") == 0)
    {
      printf("Year: %.4s\n", tags.year);
    } else if (strcmp(get, "comment") == 0)
    {
      if (tags.comment[28] == '\0')
      {
        printf("Comment: %.28s\n", tags.comment);
        printf("Track: %d\n", tags.comment[29]);
      } else
      {
        printf("Comment: %.30s\n", tags.comment);
      }
    } else if (strcmp(get, "genre") == 0)
    {
      printf("Genre: %d\n", tags.genre);
    }
    fclose(file);
    return 0;
  }
}

