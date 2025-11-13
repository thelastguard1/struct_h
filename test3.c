#define STRUCT_H_IMPLEMENTATION
#include "struct.h"

typedef struct {
    char c;
    short s;
    int i;
    long long ll;
} man;

int getMan(void* m){
    man* d = (man*)m;
    printf("%d\n", d->c);
    printf("%d\n", d->s);
    printf("%d\n", d->i);
    printf("%lld\n", d->ll);
    return 0;
}

int main(void) {
    void *t = newStr();

    addChar(t, 10);
    size_t off_s = getOffset(t);
    addShort(t, 20);
    size_t off_i = getOffset(t);
    addInt(t, 30);
    size_t off_ll = getOffset(t);
    addLong(t, 40);

    printf("Offsets:\n");
    printf("short s = %zu\n", off_s);
    printf("int i = %zu\n", off_i);
    printf("long long ll = %zu\n", off_ll);

    printf("\n--- Before ---\n");
    getMan(t);

    setInt(t, 999, off_i);

    printf("\n--- After ---\n");
    getMan(t);

    freeStr(t);
    return 0;
}
