#define uc unsigned char

typedef struct {
    uc idv3[3];
    uc version[2];
    bool flag;
    uc size[4];
} TagHeader;

typedef struct {
    uc frame_name[4];
    uc size[4];
    uc flag[2];
    bool unicode;
} TagFrame;

void get_frames(char *filename, char *frame_name);
void get_frame(char *filename, char *frame_name);
void set_frame(char *filename, char *frame_name, char *frame_value);
