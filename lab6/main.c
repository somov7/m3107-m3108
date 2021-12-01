#include "arc.h"
#include "list.h"

int main(int argc, char **argv) {
    inCompress(argv, argc, "test.arc");
    char *buf = malloc(2);
    int a;
    uint64_t b;
    FILE *temp = fopen("test.arc", "rb");
    fread(&a, 4, 1, temp);
    printf("%d\n", a);

    fread(buf, 1, 2, temp);

    fread(&b, 8, 1, temp);
    printf("%lld", b);
    return 0;
}
