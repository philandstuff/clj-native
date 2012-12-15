#include <stdlib.h>
#ifndef _WIN32
  #include <stdbool.h>
  #define EXPORT
#else
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
  #define bool BOOL
  #define false FALSE
  #define true TRUE
  #define EXPORT __declspec(dllexport)
#endif

// simple test
EXPORT int mul(int x, int y) {
    return x * y;
}

// boolean tests
EXPORT bool and2(bool x, bool y) {
    return x & y;
}
EXPORT void and3(bool x, bool y, bool *z) {
    *z = x & y;
}

// boolean test + allocate a buffer
// get closer to https://github.com/supercollider/supercollider/blob/master/server/scsynth/SC_World.cpp World_CopySndBuf

typedef struct {
    int n;
    int *buf;
} NBuf;

EXPORT void and3_buf(bool x, bool y, bool *z, int n, NBuf *pb) {
    int *p;
    int i;
    // boolean test
    *z = x & y;
    // allocate buffer
    pb->n = n;
    pb->buf = (int *)malloc(n*sizeof(int));
    // initialize buffer
    p = pb->buf;
    for(i = 0; i < n; i++) {
        *p++ = i;
    }
}

typedef struct {
    int x;
    int y;
    const char * name;
} Point;

EXPORT Point *static_point(int x, int y) {
    static Point point;
    point.x = x;
    point.y = y;
    point.name = "foo";
    return &point;
}

typedef struct {
    int length;
    int items[1];
} Hack;

EXPORT Hack *struct_hack(void) {
    /* This leaks memory; is there a way to test this without leaking? */
    Hack *ret = malloc (sizeof(Hack) + 4*sizeof(int));
    ret->length = 5;
    ret->items[0] = 0;
    ret->items[1] = 1;
    ret->items[2] = 2;
    ret->items[3] = 3;
    ret->items[4] = 4;
    return ret;
}
