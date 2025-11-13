#ifndef STRUCT_H
#define STRUCT_H
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    unsigned char* data;
    size_t size;
    size_t capacity;
    size_t cursor;
} DynStruct;
/* --- Internas --- */
static void ensure_capacity(DynStruct* ds, size_t extra);

static void do_align(DynStruct* ds, size_t align);

static void append(DynStruct* ds, const void* data, size_t sz, size_t align);

/* --- Constructor / Destructor --- */
void* newStr();

void freeStr(void* ptr);

void clearStr(void* ptr);

void* getPointer(void* ptr);

size_t getSize(void* ptr);

void setOffset(void* ptr, size_t pos);

size_t getOffset(void* ptr);
/* --- Funciones ADD (escritura secuencial) --- */
#define ADD_TYPED(funcName, type) \
void funcName(void* s, type v);

ADD_TYPED(addChar, char)
ADD_TYPED(addShort, short)
ADD_TYPED(addInt, int)
ADD_TYPED(addLong, long long)

ADD_TYPED(adduChar, unsigned char)
ADD_TYPED(adduShort, unsigned short)
ADD_TYPED(adduInt, unsigned int)
ADD_TYPED(adduLong, unsigned long long)

ADD_TYPED(addFloat, float)
ADD_TYPED(addDouble, double)
ADD_TYPED(addPtr, void*)

#undef ADD_TYPED

void addStr(void* s, void* val, size_t sz);

/* --- Lectura (getX) y escritura directa (setX) por offset --- */
#define READFUNC(type, name);\
type get##name(void* s, size_t offset);\
void set##name(void* s, size_t offset, type v);


READFUNC(char, Char)
READFUNC(short, Short)
READFUNC(int, Int)
READFUNC(long long, Long)
READFUNC(unsigned char, UChar)
READFUNC(unsigned short, UShort)
READFUNC(unsigned int, UInt)
READFUNC(unsigned long long, ULong)
READFUNC(float, Float)
READFUNC(double, Double)
READFUNC(void*, Ptr)

#undef READFUNC

/* --- Utilidad de depuración --- */
void dumpBytes(void* s);

void* fromStruct(const void* src, size_t size);

#ifdef STRUCT_H_IMPLEMENTATION
/* --- Internas --- */
static void ensure_capacity(DynStruct* ds, size_t extra) {
    if (ds->cursor + extra > ds->capacity) {
        ds->capacity = (ds->cursor + extra) * 2;
        ds->data = (unsigned char*)realloc(ds->data, ds->capacity);
    }
}

static void do_align(DynStruct* ds, size_t align) {
    size_t pad = (align - (ds->cursor % align)) % align;
    if (pad) {
        ensure_capacity(ds, pad);
        memset(ds->data + ds->cursor, 0, pad);
        ds->cursor += pad;
        if (ds->cursor > ds->size) ds->size = ds->cursor;
    }
}

static void append(DynStruct* ds, const void* data, size_t sz, size_t align) {
    do_align(ds, align);
    ensure_capacity(ds, sz);
    memcpy(ds->data + ds->cursor, data, sz);
    ds->cursor += sz;
    if (ds->cursor > ds->size)
        ds->size = ds->cursor;
}

/* --- Constructor / Destructor --- */
void* newStr() {
    DynStruct* ds = (DynStruct*)malloc(sizeof(DynStruct));
    ds->size = 0;
    ds->cursor = 0;
    ds->capacity = 64;
    ds->data = (unsigned char*)malloc(ds->capacity);
    return ds;
}

void freeStr(void* ptr) {
    DynStruct* ds = (DynStruct*)ptr;
    free(ds->data);
    free(ds);
}

void clearStr(void* ptr) {
    DynStruct* ds = (DynStruct*)ptr;
    ds->size = 0;
    ds->cursor = 0;
}

void* getPointer(void* ptr) {
    return ((DynStruct*)ptr)->data;
}

size_t getSize(void* ptr) {
    return ((DynStruct*)ptr)->size;
}

void setOffset(void* ptr, size_t pos) {
    DynStruct* ds = (DynStruct*)ptr;
    if (pos > ds->size) pos = ds->size;
    ds->cursor = pos;
}

size_t getOffset(void* ptr) {
    return ((DynStruct*)ptr)->cursor;
}

/* --- Funciones ADD (escritura secuencial) --- */
#define ADD_TYPED(funcName, type) \
void funcName(void* s, type v) { \
    append((DynStruct*)s, &v, sizeof(v), _Alignof(type)); \
}

ADD_TYPED(addChar, char)
ADD_TYPED(addShort, short)
ADD_TYPED(addInt, int)
ADD_TYPED(addLong, long long)

ADD_TYPED(adduChar, unsigned char)
ADD_TYPED(adduShort, unsigned short)
ADD_TYPED(adduInt, unsigned int)
ADD_TYPED(adduLong, unsigned long long)

ADD_TYPED(addFloat, float)
ADD_TYPED(addDouble, double)
ADD_TYPED(addPtr, void*)

#undef ADD_TYPED

void addStr(void* s, void* val, size_t sz) {
    append((DynStruct*)s, val, sz, 1);
}

/* --- Lectura (getX) y escritura directa (setX) por offset --- */
#define READFUNC(type, name) \
type get##name(void* s, size_t offset) { \
    DynStruct* ds = (DynStruct*)s; \
    if (offset + sizeof(type) > ds->size) return (type)0; \
    type v; memcpy(&v, ds->data + offset, sizeof(v)); return v; \
} \
void set##name(void* s, size_t offset, type v) { \
    DynStruct* ds = (DynStruct*)s; \
    if (offset + sizeof(type) > ds->size) return; \
    memcpy(ds->data + offset, &v, sizeof(v)); \
}

READFUNC(char, Char)
READFUNC(short, Short)
READFUNC(int, Int)
READFUNC(long long, Long)
READFUNC(unsigned char, UChar)
READFUNC(unsigned short, UShort)
READFUNC(unsigned int, UInt)
READFUNC(unsigned long long, ULong)
READFUNC(float, Float)
READFUNC(double, Double)
READFUNC(void*, Ptr)

#undef READFUNC

/* --- Utilidad de depuración --- */
void dumpBytes(void* s) {
    DynStruct* ds = (DynStruct*)s;
    for (size_t i = 0; i < ds->size; ++i)
        printf("%02X ", ds->data[i]);
    printf("\n");
}

void* fromStruct(const void* src, size_t size) {
    DynStruct* ds = (DynStruct*)malloc(sizeof(DynStruct));
    ds->capacity = size;
    ds->size = size;
    ds->cursor = 0;
    ds->data = (unsigned char*)malloc(size);
    memcpy(ds->data, src, size);
    return ds;
}

#endif
#endif