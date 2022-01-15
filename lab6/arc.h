#ifndef ARV_ARC_H
#define ARV_ARC_H
#include <stdio.h>

#pragma pack(push, 1)
//��������� ����� ��� ������� �������� ���������� � ��
typedef struct
{
    char expansion[3]; //����������
    size_t fileSize;
    size_t fileCounter;
} fileHeader;

#pragma pack(pop)

// ��������� ������
typedef struct
{
    fileHeader header;
    FILE *file;
} Arch;

Arch open(const char *archName);
Arch init(char *archName);
long create(Arch *arch, char* fileName);
char **list(Arch *arch);
int extract(Arch *arch);
void close(Arch *arch);
#endif //ARV_ARC_H
