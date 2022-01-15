#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#ifndef ID3V2_H_INCLUDED
#define ID3V2_H_INCLUDED

//ID3v2 file tag header structure
struct ID3v2_header
{
    char identifier[3];
    char major[1];
    char minor[1];
    char flags[1];
    char size[4];
};
//ID3v2 file frame structure
struct ID3v2_frame_header
{
    char identifier[4];
    char size[4];
    char flags[2];
};

#endif // ID3V2_H_INCLUDED
