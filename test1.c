#define STRUCT_H_IMPLEMENTATION
#include "struct.h"
#include <stdio.h>

typedef struct {
    char c;
    short s;
    int i;
    long long ll;
} man;

int getMan(void* m) {
    man* d = (man*)m;
    printf("%d\n", d->c);
    printf("%d\n", d->s);
    printf("%d\n", d->i);
    printf("%lld\n", d->ll);
    return 0;
}

int main() {
    void *t = newStr();
    addChar(t, 10);
    addShort(t, 20);
    addInt(t, 30);
    addLong(t, 40);

    dumpBytes(t);
    getMan(getPointer(t));

    freeStr(t);
}
