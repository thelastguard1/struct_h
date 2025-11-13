#define STRUCT_H_IMPLEMENTATION
#include "struct.h"
#include <stdio.h>

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
    man m = {10, 20, 30, 40};
    void* t = fromStruct(&m, sizeof(man));

    printf("Contenido copiado desde struct C:\n");
    getMan(getPointer(t));

    setInt(t, 999, 2); // HARDCODED OFFSET :)

    printf("\nDespues de modificar:\n");
    getMan(getPointer(t));

    freeStr(t);
    return 0;
}
