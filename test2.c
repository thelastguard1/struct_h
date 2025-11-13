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
    printf("C=%d S=%d I=%d LL=%lld\n", d->c, d->s, d->i, d->ll);
    return 0;
}

int main() {
    void* t = newStr();

    addChar(t, 10);
    addShort(t, 20);
    addInt(t, 30);
    addLong(t, 40);

    printf("Bytes iniciales:\n");
    dumpBytes(t);

    printf("Lectura directa:\n");
    getMan(getPointer(t));

    // hardcoded offsets
    size_t off_c = 0;
    size_t off_s = _Alignof(short);      
    size_t off_i = 4;                    
    size_t off_ll = 8;                   

    printf("\nValores por getX:\n");
    printf("C=%d\n", getChar(t, off_c));
    printf("S=%d\n", getShort(t, off_s));
    printf("I=%d\n", getInt(t, off_i));
    printf("LL=%lld\n", getLong(t, off_ll));

    printf("\nModificando valores con setX...\n");
    setChar(t, off_c, 99);
    setShort(t, off_s, 777);
    setInt(t, off_i, 888);
    setLong(t, off_ll, 9999);

    printf("\nBytes despues de modificar:\n");
    dumpBytes(t);

    printf("\nLectura despues de setX:\n");
    getMan(getPointer(t));

    freeStr(t);
}
