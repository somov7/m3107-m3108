/* Orlov Aleksandr, 12-5, M3107,  29.10.2021 */

#define MAX_REQUEST_SIZE 5000
#define TIME_STRING_SIZE 40
#define FILENAME_SIZE 261

typedef struct {
    long timestamp;
    struct Timestamp* next;
} Timestamp;

void parse(FILE* f, long* errors, long window);