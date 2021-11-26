/* Orlov Aleksandr, 12-5, M3107, 22.11.2021 */

#define MAX_ARR_SIZE 1024

struct mp3file
{
    FILE* file;
    char* str;
    int size;
};

FILE* mp3open(struct mp3file*, char*);
void mp3update(struct mp3file*);
void mp3close(struct mp3file*);
int isFrameHeader(char*);
int findFrame(struct mp3file*, char*);
int printFrames(struct mp3file*);
int getFrame(struct mp3file*, char*);
void setFrame(struct mp3file*, char*, char*);