#ifndef STRUCTS_H
#define STRUCTS_H

enum argTypes{LINES, BYTES, WORDS};

struct Option {
    char* longName;
    char* shortName;
    enum argTypes argType;
};

struct Args {
    char* filePath;
    enum argTypes argType;
};

#endif
