#include <stdio.h>
#include <string.h>
#include <malloc.h>

union FSize 
{
    unsigned char Bytes[4];
    unsigned int SizeOfFile;
} FSize;

void Create(int argc, char* argv[], char* ArchName) 
{
    FILE* Arch = fopen(ArchName, "wb");
    FILE* F;

    for (int i = 5; i < argc; i++) 
    {
        F = fopen(argv[i], "rb");

        fseek(F, 0, SEEK_END);

        unsigned int FileSize = ftell(F);

        fseek(F, 0, SEEK_SET);

        unsigned char NameLen = strlen(argv[i]) + 1;

        fputc(NameLen, Arch);

        FSize.SizeOfFile = FileSize;

        for (int j = 3; j >= 0; j--)
            fputc(FSize.Bytes[j], Arch);

        fwrite(argv[i], sizeof(unsigned char), NameLen, Arch);

        unsigned char* Buff = calloc(FileSize, sizeof(unsigned char));

        fread(Buff, sizeof(unsigned char), FileSize, F);
        fwrite(Buff, sizeof(unsigned char), FileSize, Arch);

        free(Buff);
        fclose(F);
    }

    printf("%s created\n", ArchName);

    fclose(Arch);
}

void Extract(char* ArchName) {
    FILE* Arch = fopen(ArchName, "rb");
    int NameLen;

    while ((NameLen = fgetc(Arch)) != -1) 
    {
        char FileLen[4];
        fread(FileLen, sizeof(unsigned char), 4, Arch);

        for (int j = 3; j >= 0; j--)
            FSize.Bytes[j] = FileLen[3-j];

        unsigned char* FName = calloc(NameLen, sizeof(unsigned char));
        for (unsigned char i = 0; i < NameLen; i++) {
            FName[i] = fgetc(Arch);
        }

        FILE* F = fopen(FName, "wb");

        unsigned char* Buff = calloc(sizeof(unsigned char), FSize.SizeOfFile);

        fread(Buff, sizeof(unsigned char), FSize.SizeOfFile, Arch);
        fwrite(Buff, sizeof(unsigned char), FSize.SizeOfFile, F);

        printf("%s exctracted\n", FName);

        free(FName);
        free(Buff);
        fclose(F);
    }

    fclose(Arch);
}

void List(char* ArchName) 
{
    FILE* Arch = fopen(ArchName, "rb");
    unsigned int NameLen;

    while ((NameLen = getc(Arch)) != EOF) 
    {
        char FileLen[4];
        fread(FileLen, sizeof(unsigned char), 4, Arch);
        
        for (int j = 3; j >= 0; j--)
            FSize.Bytes[j] = FileLen[3 - j];

        unsigned char* FName = calloc(NameLen, sizeof(unsigned char));

        fread(FName, sizeof(unsigned char), NameLen, Arch);

        fseek(Arch, FSize.SizeOfFile, SEEK_CUR);

        printf("%s\n", FName);
        free(FName);
    }
    fclose(Arch);
}

int main(int argc, char* argv[]) 
{
    char* ArchName = argv[3];

    if (strcmp(argv[4], "--create") == 0)
        Create(argc, argv, ArchName);
    else if (strcmp(argv[4], "--extract") == 0)
        Extract(ArchName);
    else if (strcmp(argv[4], "--list") == 0)
        List(ArchName);

    return 0;
}