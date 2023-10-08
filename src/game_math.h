// Copyright 2023 Lucas Klassmann
// License: Apache License 2.0
#ifndef GAME_MATH_H
#define GAME_MATH_H

#include "core.h"


typedef struct {
    union {
        struct {
            double x;
            double y;
        };
        struct {    // alias
            double w;
            double h;
        };
    };
} Vector;

typedef struct {
    union {
        struct {
            double x;
            double y;
        };
        Vector a;
        Vector position;
    };

    union {
        struct {
            double w;
            double h;
        };
        Vector b;
        Vector dimension;
    };
} Rect;

Vector vector_new(double x, double y);

Vector vector_add(Vector a, Vector b);

Vector vector_add_scalar(Vector a, double b);

Vector vector_sub(Vector a, Vector b);

Vector vector_sub_scalar(Vector a, double b);

Vector vector_mul_scalar(Vector a, double b);

Vector vector_div_scalar(Vector a, double b);

Vector vector_lerp(Vector a, Vector b, double t);

double vector_magnitude(Vector v);

double vector_distance(Vector a, Vector b);

// LUA API
void api_math_open(lua_State *L);

#endif // GAME_MATH_H
