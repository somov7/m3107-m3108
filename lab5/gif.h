//
// Created by Антон Чемодуров on 03.12.2021.
//

#ifndef LAB5_GIF_H
#define LAB5_GIF_H

#include <stdint.h>

typedef char* ColorTable;

#pragma pack(push, 1)
typedef struct {
    char header[6]; // сигнатура файла
    uint16_t lsWidth; // ширина и высота локального экрана
    uint16_t lsHeight;
    char GlobalCT:1; // флаг глобальной палитры
    char color:3; // бит на цвет для (мнохромного 0)
    char gCTSortFlag:1; // флаг сортировки палитры
    char GlobalCTSize:3; // размер палитры (для мнохромного 3)
    char BGColor; // номер цвета фона
    char r; // ?
}gifLSD; // Local Screen Descriptor

typedef struct {
    char GraghSep; // '!'
    char gcLabel; // 0xf9
    char byteSize; // 0x04
    char _reserved:3; // 0
    char dispMethod:3;
    char end:2;
    uint16_t delay;
    char tr;
    char nullSep; // 0
}gifGCE; // Graphics Control Extension

typedef struct {
    char ImageSep; // ','
    uint16_t lsLeft; // координаты изображения на локальном экране
    uint16_t lsTop;
    uint16_t ImWidth; // ширина высота изображения
    uint16_t ImHeight;
    char localCT:1; // флаг локальной таблицы цветов
    char interface:1; // чересстрочная или обычная развертка картинки
    char lCTSortFlag:1;
    char _reserved:2; // 0
    char lCTSize:3; // размер локальной таблицы цветов если !localCT, то 0
}gifImD; // Image Descriptor
/**
 * 2c 00 00 00 00 04 00 04 00 00 03 08 08 0a d2 42 90 94 59 12 00
 */
#pragma pack(pop)

typedef struct {
    char minCodeSize;
    char *data;
}gifImagData;


typedef struct {
    gifLSD *lsd;
    ColorTable colorTable;
    gifGCE *gce;
    gifImagData *data[];
}GIF;

#endif //LAB5_GIF_H
