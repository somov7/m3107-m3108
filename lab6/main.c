#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(push, 1)
typedef struct {
    uint32_t fileSize;
    uint16_t nameSize;
} LoopHeader;
#pragma pack(pop)

typedef enum {
    GET_LIST,
    EXTRACT,
    CREATE,
    NOPE
} Commands;

typedef enum {
    FILE_NOT_FOUND = 1,
    FILENAME_NOT_FOUND,
    COMMAND_NOT_FOUND,
    WRONG_FILE_FORMAT,
    FILE_ALREADY_CREATE,
    WRONG_ARGUMENTS_COUNT,
} ErrorTypes;

void error(ErrorTypes error) {
    switch (error) {
        case FILE_NOT_FOUND:
            printf("Error: File not found\n");
            break;
        case FILENAME_NOT_FOUND:
            printf("Error: Argument of '--file' not found\n");
            break;
        case COMMAND_NOT_FOUND:
            printf("Error: Command not found\n");
            break;
        case WRONG_FILE_FORMAT:
            printf("Error: File should be with .arc format\n");
            break;
        case FILE_ALREADY_CREATE:
            printf("Error: File with this name already exists\n");
            break;
        case WRONG_ARGUMENTS_COUNT:
            printf("Error: Wrong arguments count\n");
            break;
    }
    printf("Correct usage: --file <filepath> and one of options:\n"
           "--create <files path> for create archive\n"
           "--extract\n"
           "--list");
    exit(error);
}

void checkArgumentsCount(int argsCount) {
    if (argsCount < 4)
        error(WRONG_ARGUMENTS_COUNT);
}

void checkFileFormat(FILE *file) {
    char type[3];
    fread(type, 3, 1, file);
    if (!(type[0] == 'A' && type[1] == 'R' && type[2] == 'C'))
        error(WRONG_FILE_FORMAT);
}

void checkFileName(char *filename) {
    uint64_t nameSize = strlen(filename);
    filename += nameSize - 3;
    if (strcmp(filename, "arc") != 0)
        error(WRONG_FILE_FORMAT);
}

char *getFileName(int argumentsCount, char **arguments) {
    char *filename = NULL;
    for (int i = 1; i < argumentsCount; ++i) {
        if (!strcmp(arguments[i], "--file") == 0 && i < argumentsCount - 1) {
            filename = arguments[i];
            break;
        }
    }
    if (filename == NULL)
        error(FILENAME_NOT_FOUND);
    return filename;
}

Commands determineCommand(char *string) {
    Commands command = NOPE;
    if (!strcmp(string, "--list"))
        command = GET_LIST;
    else if (!strcmp(string, "--extract"))
        command = EXTRACT;
    else if (!strcmp(string, "--create"))
        command = CREATE;
    return command;
}

Commands getCommand(int argumentsCount, char **arguments) {
    char commands[3][10] = {"--list", "--extract", "--create"};
    Commands command = NOPE;
    for (int i = 1; i < argumentsCount; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (!strcmp(arguments[i], commands[j])) {
                command = determineCommand(commands[j]);
                break;
            }
        }
    }
    if (command == NOPE)
        error(COMMAND_NOT_FOUND);
    return command;
}

int getFirstFileIndex(int argumentsCount, char **arguments) {
    int firstIndex = -1;
    for (int i = 1; i < argumentsCount; ++i) {
        if (!strcmp(arguments[i], "--create") && i < argumentsCount - 1) {
            firstIndex = i + 1;
            break;
        }
    }
    return firstIndex;
}

void createArchive(FILE *file, int firstFileIndex, int argumentsCount, char **arguments) {
    fwrite("ARC", 3, 1, file);
    if (firstFileIndex < 0)
        return;
    for (int i = firstFileIndex; i < argumentsCount; ++i) {
        FILE *input = fopen(arguments[i], "rb");
        if (input == NULL) {
            printf("File '%s' not found in current directory", arguments[i]);
            continue;
        }
        fseek(input, 0, SEEK_END);
        uint32_t fileSize = ftell(input);
        fseek(input, 0, SEEK_SET);
        if (fileSize > UINT32_MAX) {
            printf("File '%s' too big to add to the archive", arguments[i]);
            continue;
        }

        uint8_t *fileData = calloc(1, fileSize);
        fread(fileData, fileSize, 1, input);

        uint16_t nameSize = strlen(arguments[i]) + 1;
        char *fileName = calloc(1, nameSize);
        strcpy(fileName, arguments[i]);
        fileName[nameSize - 1] = '\0';

        LoopHeader loopHeader;
        loopHeader.fileSize = fileSize;
        loopHeader.nameSize = nameSize;

        fwrite(&loopHeader, sizeof(loopHeader), 1, file);
        fwrite(fileName, nameSize, 1, file);
        fwrite(fileData, fileSize, 1, file);

        free(fileName);
        free(fileData);
        fclose(input);
    }
}

void getFilesList(FILE *file) {
    printf("Files in archive:\n");
    int count = 0;
    LoopHeader loopHeader;
    fread(&loopHeader, sizeof(loopHeader), 1, file);
    while (!feof(file)) {
        char *filename = calloc(1, loopHeader.nameSize);
        fread(filename, loopHeader.nameSize, 1, file);
        printf("%s\n", filename);

        uint8_t *fileData = calloc(1, loopHeader.fileSize);
        fread(fileData, loopHeader.fileSize, 1, file);

        free(filename);
        free(fileData);

        fread(&loopHeader, sizeof(loopHeader), 1, file);
        count++;
    }

    if (count == 0)
        printf("none\n");
}

void extractFiles(FILE *file) {
    LoopHeader loopHeader;
    fread(&loopHeader, sizeof(loopHeader), 1, file);
    while (!feof(file)) {
        char *filename = calloc(1, loopHeader.nameSize);
        fread(filename, loopHeader.nameSize, 1, file);

        FILE *output = fopen(filename, "wb");
        uint8_t *fileData = calloc(1, loopHeader.fileSize);
        fread(fileData, loopHeader.fileSize, 1, file);

        if (output == NULL) {
            printf("Can't create file '%s'", filename);
            free(fileData);
            continue;
        }

        fwrite(fileData, loopHeader.fileSize, 1, output);

        free(filename);
        free(fileData);

        fread(&loopHeader, sizeof(loopHeader), 1, file);
    }
}

void solve(int argumentsCount, char **arguments) {
    char *filename = getFileName(argumentsCount, arguments);
    Commands command = getCommand(argumentsCount, arguments);
    switch (command) {
        case GET_LIST: {
            FILE *file = fopen(filename, "rb");
            if (file == NULL)
                error(FILE_NOT_FOUND);
            checkFileFormat(file);
            getFilesList(file);
            break;
        } case EXTRACT: {
            FILE *file = fopen(filename, "rb");
            if (file == NULL)
                error(FILE_NOT_FOUND);
            checkFileFormat(file);
            extractFiles(file);
            break;
        } case CREATE: {
            FILE *file = fopen(filename, "rb");
            if (file != NULL)
                error(FILE_ALREADY_CREATE);
            checkFileName(filename);
            file = fopen(filename, "wb");
            int firstFileIndex = getFirstFileIndex(argumentsCount, arguments);
            createArchive(file, firstFileIndex, argumentsCount, arguments);
            break;
        }
    }
}

int main(int argc, char **argv) {
    checkArgumentsCount(argc);
    solve(argc, argv);
    return 0;
}