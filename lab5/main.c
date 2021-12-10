#include <stdio.h>
#include "bmp.h"
#include "game.h"
#include "glut.h"

unsigned char *Frame;
double Zoom = 1;
int isPause = 1, W, H;

void PutPixel(int X, int Y, int G, int B, int R)
{
    if (X < 0 || Y < 0 || X >= W || Y >= H)
        return;
    Frame[(Y * W + X) * 3 + 0] = B;
    Frame[(Y * W + X) * 3 + 1] = G;
    Frame[(Y * W + X) * 3 + 2] = R;
}

int GetCell(byte* F, int x, int y)
{
    x = (x + W) % W;
    y = (y + H) % H;
    return F[y * W + x];
}

void SetCell(byte* F, int x, int y, int v)
{
    x = (x + W) % W;
    y = (y + H) % H;
    F[y * W + x] = v;
}

void FieldDraw(byte* F)
{
    int x, y;

    for (y = 0; y < H; y++)
        for (x = 0; x < W; x++)
            if (GetCell(F, x, y))
                PutPixel(x, y, 255, 50, 100);
            else
                PutPixel(x, y, 60, 30, 100);
}

void FieldClear(byte* F)
{
    int x, y;

    for (y = 0; y < H; y++)
        for (x = 0; x < W; x++)
            SetCell(F, x, y, 0);
}

int GetNeighbours(byte* F, int x, int y)
{
    return GetCell(F, x + 1, y)
        + GetCell(F, x + 1, y + 1)
        + GetCell(F, x + 1, y - 1)
        + GetCell(F, x, y + 1)
        + GetCell(F, x, y - 1)
        + GetCell(F, x - 1, y - 1)
        + GetCell(F, x - 1, y)
        + GetCell(F, x - 1, y + 1);
}

void NewGeneration(byte* F1, byte* F2)
{
    int x, y, v, n;

    for (y = 0; y < H; y++)
        for (x = 0; x < W; x++)
        {
            n = GetNeighbours(F1, x, y);
            if (GetCell(F1, x, y))
                if (n < 2 || n > 3)
                    v = 0;
                else
                    v = 1;
            else
                if (n == 3)
                    v = 1;
                else
                    v = 0;
            SetCell(F2, x, y, v);
        }
}

void Keyboard(unsigned char key, int X, int Y)
{
    if (key == 27)
        exit(0);
    else
        PutPixel(X, Y, 0, 0, 500);
}

void Display(void)
{
    byte* tmp;

    glClearColor(0.5, 0.5, 0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    if (isPause)
    {
        FieldDraw(F1);
        NewGeneration(F1, F2);
        tmp = F1;
        F1 = F2;
        F2 = tmp;
    }

    glRasterPos2d(-1, 1);
    glPixelZoom(Zoom, -Zoom);
    glDrawPixels(W, H, GL_BGR_EXT, GL_UNSIGNED_BYTE, Frame);

    glFinish();
    glutPostRedisplay();
    glutSwapBuffers();
}

int main(int argc, char* argv[]) {
    char filename[MAX_STRING_SIZE], dirname[MAX_STRING_SIZE];
    char* offset;
    int iterations = 3, frequency = 1, opengl = 1;
    int **pixels, **pixels2;
    FILE *f;
    bitMapFile bmp;

    if (argc == 2 && !strcmp(argv[1], "--help")) {
        printf("This app supports two modes:\n1) Just generating several generations\n2) OpenGl mode with saving to bmp\n");
        printf("If you want to activate OpenGl mode your .gmp picture should have NxN size. Also n %% 100 == 0\n");
        printf("App supports 5 commands last three of them are optional:\n--input - source image filename\n");
        printf("--output - output directory path\n--max_iter - max number of iterations\n--dump_freq - frequency of creating generations\n");
        printf("--opengl - OpenGl status (0 to turn off, 1 to turn on)\n");
        printf("Example of command: Lab5.exe --input vitsan.bmp --output D:/ --max_iter 3 --dump_freq 1 --opengl 1\n");
        return 0;
    }

    if (argc < 5) {
        printf("Error: invalid parameters\n");
        return 1;
    }

    for (int i = 1; i < argc; i += 2) {
        if (!strcmp("--input", argv[i]))
            strcpy(filename, argv[i + 1]);
        else if (!strcmp("--output", argv[i]))
            strcpy(dirname, argv[i + 1]);
        else if (!strcmp("--max_iter", argv[i])) {
            iterations = atoi(argv[i + 1]);
            if (iterations < 0) {
                printf("Error: incorrect iterations\n");
                return 1;
            }
        }
        else if (!strcmp("--dump_freq", argv[i])) {
            frequency = atoi(argv[i + 1]);
            if (frequency < 0) {
                printf("Error: incorrect frequency\n");
                return 1;
            }
        }
        else if (!strcmp("--opengl", argv[i])) {
            opengl = atoi(argv[i + 1]);
            if (opengl != 0 && opengl != 1) {
                printf("Error: incorrect opengl status\n");
                return 1;
            }
        }
    } 

    /* Trying to open the file */
    f = fopen(filename, "rb");
    if (f == NULL) {
        printf("Error: can't open the file\n");
        return 1;
    }
    
    /* Reading first 54 bytes of some information */
    fread(&bmp, 1, sizeof(bitMapFile), f);

    offset = (char *) calloc(sizeof(char), bmp.bfOffs);
    fseek(f, 0, SEEK_SET);
    fread(offset, 1, bmp.bfOffs, f); //after that pixels go

    /* Initializing variables for OpenGl */
    W = bmp.biWidth;
    H = bmp.biHeight;
    
    pixels = bmpToPixels(f, bmp);
    pixels2 = pixels;
    Frame = (unsigned char *)malloc(W * H * 3 * sizeof(unsigned char));

    if (opengl) {
        if (W % 100 != 0 || H % 100 != 0 || W != H) {
            printf("Error: incorrect parameters for OpenGl mode\n");
            return 1;
        }

        for (int i = 0; i < iterations; i++)
            if (i % frequency == 0) {
                pixels = newGeneration(pixels, bmp);
                arrToBmp(pixels, bmp, i, dirname, offset);
            }
            

        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
        
        F1 = malloc(W * H);
        F2 = malloc(W * H);
        fieldInit(F1, pixels2, bmp);
        glutInitWindowPosition(0, 0);
        glutInitWindowSize(H, W);
        glutCreateWindow("Vitsan onelove");
        glutDisplayFunc(Display);

        glutKeyboardFunc(Keyboard);

        glutMainLoop(); 
    }
    else 
        for (int i = 0; i < iterations; i++)
            if (i % frequency == 0) {
                pixels = newGeneration(pixels, bmp);
                arrToBmp(pixels, bmp, i, dirname, offset);
            }
    return 0;
}