// Copyright 2023 Lucas Klassmann
// License: Apache License 2.0
#include "game_math.h"

///////////////////////////////////////////////////////////////////////////////
///// VECTOR
///////////////////////////////////////////////////////////////////////////////
const Vector VectorUp = {0.0, 1.0};
const Vector VectorLeft = {-1.0, 0.0};
const Vector VectorRight = {1.0, 0.0};
const Vector VectorDown = {0.0, -1.0};
const Vector VectorZero = {0.0, 0.0};

Vector vector_new(double x, double y) {
    Vector v = {x, y};
    return v;
}

Vector vector_add(Vector a, Vector b) {
    Vector v = {a.x + b.x, a.y + b.y};
    return v;
}

Vector vector_add_scalar(Vector a, double b) {
    Vector v = {a.x + b, a.y + b};
    return v;
}

Vector vector_sub(Vector a, Vector b) {
    Vector v = {a.x - b.x, a.y - b.y};
    return v;
}

Vector vector_sub_scalar(Vector a, double b) {
    Vector v = {a.x - b, a.y - b};
    return v;
}

Vector vector_mul_scalar(Vector a, double b) {
    Vector v = {a.x * b, a.y * b};
    return v;
}

Vector vector_div_scalar(Vector a, double b) {
    if (fabs(b) > DBL_EPSILON) {
        Vector v = {a.x / b, a.y / b};
        return v;
    }
    return a;
}

Vector vector_lerp(Vector a, Vector b, double t) {
    Vector v = vector_add(a, vector_mul_scalar(vector_sub(b, a), t));
    return v;
}

double vector_magnitude(Vector v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

double vector_distance(Vector a, Vector b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx - dy * dy);
}


///////////////////////////////////////////////////////////////////////////////
///// RECT
///////////////////////////////////////////////////////////////////////////////

Rect rect_new(double x, double y, double w, double h) {
    Rect r = {x, y, w, h};
    return r;
}

bool rect_overlaps(Rect a, Rect b, Vector *side) {
    // checks the rectangles are overlapping and which side
    double a_left = a.x;
    double a_right = a.x + a.w;
    double a_up = a.y;
    double a_down = a.y + a.h;

    double b_left = b.x;
    double b_right = b.x + b.w;
    double b_up = b.y;
    double b_down = b.y + b.h;

    if (a_right >= b_left && a_left <= b_right) {
        if (a_down >= b_up && a_up <= b_down) {
            if (a_right > b_left && a_left < b_right) {
                if (a_down > b_up && a_up < b_down) {
                    *side = VectorZero;
                } else if (a_down <= b_up) {
                    *side = VectorUp;
                } else {
                    *side = VectorDown;
                }
            } else if (a_right <= b_left) {
                *side = VectorLeft;
            } else {
                *side = VectorRight;
            }
            return true;
        }
    }
    return false;
}

Vector rect_center(Rect r) {
    return vector_new(r.x + (r.w / 2), r.y + (r.h / 2));
}

///////////////////////////////////////////////////////////////////////////////
///// LUA API
///////////////////////////////////////////////////////////////////////////////

int api_vector_new(lua_State *L) {
    double a = luaL_checknumber(L, 1);
    double b = luaL_checknumber(L, 2);
    Vector v = vector_new(a, b);

    Vector *ptr = lua_newuserdata(L, sizeof(Vector));
    *ptr = v;
    luaL_getmetatable(L, "Vector");
    lua_setmetatable(L, -2);
    return 1;
}

int api_vector_add(lua_State *L) {
    Vector v;

    if (lua_type(L, 1) == LUA_TNUMBER && lua_type(L, 2) == LUA_TUSERDATA) {
        lua_Number scalar = luaL_checknumber(L, 1);
        Vector *a = luaL_checkudata(L, 2, "Vector");
        v = vector_add_scalar(*a, scalar);
    } else if (lua_type(L, 1) == LUA_TUSERDATA && lua_type(L, 2) == LUA_TNUMBER) {
        Vector *a = luaL_checkudata(L, 1, "Vector");
        lua_Number scalar = luaL_checknumber(L, 2);
        v = vector_add_scalar(*a, scalar);
    } else if (lua_type(L, 1) == LUA_TUSERDATA && lua_type(L, 2) == LUA_TUSERDATA) {
        Vector *v1 = luaL_checkudata(L, 1, "Vector");
        Vector *v2 = luaL_checkudata(L, 2, "Vector");
        v = vector_add(*v1, *v2);
    } else {
        v = vector_new(0.0, 0.0);
    }
    Vector *ptr = lua_newuserdata(L, sizeof(Vector));
    *ptr = v;
    luaL_getmetatable(L, "Vector");
    lua_setmetatable(L, -2);
    return 1;
}

int api_vector_sub(lua_State *L) {
    Vector v;

    if (lua_type(L, 1) == LUA_TNUMBER && lua_type(L, 2) == LUA_TUSERDATA) {
        lua_Number scalar = luaL_checknumber(L, 1);
        Vector *a = luaL_checkudata(L, 2, "Vector");
        v = vector_sub_scalar(*a, scalar);
    } else if (lua_type(L, 1) == LUA_TUSERDATA && lua_type(L, 2) == LUA_TNUMBER) {
        Vector *a = luaL_checkudata(L, 1, "Vector");
        lua_Number scalar = luaL_checknumber(L, 2);
        v = vector_sub_scalar(*a, scalar);
    } else if (lua_type(L, 1) == LUA_TUSERDATA && lua_type(L, 2) == LUA_TUSERDATA) {
        Vector *v1 = luaL_checkudata(L, 1, "Vector");
        Vector *v2 = luaL_checkudata(L, 2, "Vector");
        v = vector_sub(*v1, *v2);
    } else {
        v = vector_new(0.0, 0.0);
    }
    Vector *ptr = lua_newuserdata(L, sizeof(Vector));
    *ptr = v;
    luaL_getmetatable(L, "Vector");
    lua_setmetatable(L, -2);
    return 1;
}

int api_vector_mul(lua_State *L) {
    Vector v;

    if (lua_type(L, 1) == LUA_TNUMBER && lua_type(L, 2) == LUA_TUSERDATA) {
        lua_Number scalar = luaL_checknumber(L, 1);
        Vector *a = luaL_checkudata(L, 2, "Vector");
        v = vector_mul_scalar(*a, scalar);
    } else if (lua_type(L, 1) == LUA_TUSERDATA && lua_type(L, 2) == LUA_TNUMBER) {
        Vector *a = luaL_checkudata(L, 1, "Vector");
        lua_Number scalar = luaL_checknumber(L, 2);
        v = vector_mul_scalar(*a, scalar);
    } else {
        v = vector_new(0.0, 0.0);
    }
    Vector *ptr = lua_newuserdata(L, sizeof(Vector));
    *ptr = v;
    luaL_getmetatable(L, "Vector");
    lua_setmetatable(L, -2);
    return 1;
}

int api_vector_div(lua_State *L) {
    Vector v;

    if (lua_type(L, 1) == LUA_TNUMBER && lua_type(L, 2) == LUA_TUSERDATA) {
        lua_Number scalar = luaL_checknumber(L, 1);
        Vector *a = luaL_checkudata(L, 2, "Vector");
        v = vector_div_scalar(*a, scalar);
    } else if (lua_type(L, 1) == LUA_TUSERDATA && lua_type(L, 2) == LUA_TNUMBER) {
        Vector *a = luaL_checkudata(L, 1, "Vector");
        lua_Number scalar = luaL_checknumber(L, 2);
        v = vector_div_scalar(*a, scalar);
    } else {
        v = vector_new(0.0, 0.0);
    }
    Vector *ptr = lua_newuserdata(L, sizeof(Vector));
    *ptr = v;
    luaL_getmetatable(L, "Vector");
    lua_setmetatable(L, -2);
    return 1;
}

int api_vector_lerp(lua_State *L) {
    Vector *a = luaL_checkudata(L, 1, "Vector");
    Vector *b = luaL_checkudata(L, 2, "Vector");
    lua_Number t = luaL_checknumber(L, 3);

    Vector v = vector_lerp(*a, *b, t);
    Vector *ptr = lua_newuserdata(L, sizeof(Vector));
    *ptr = v;
    luaL_getmetatable(L, "Vector");
    lua_setmetatable(L, -2);
    return 1;
}


int api_vector_magnitude(lua_State *L) {
    Vector *a = luaL_checkudata(L, 1, "Vector");
    lua_Number m = vector_magnitude(*a);
    lua_pushnumber(L, m);
    return 1;
}

int api_vector_distance(lua_State *L) {
    Vector *a = luaL_checkudata(L, 1, "Vector");
    Vector *b = luaL_checkudata(L, 2, "Vector");

    lua_Number d = vector_distance(*a, *b);
    lua_pushnumber(L, d);
    return 1;
}

int api_vector_access_x(lua_State *L) {
    Vector *v = luaL_checkudata(L, 1, "Vector");
    lua_pushnumber(L, v->x);
    return 1;
}

int api_vector_access_y(lua_State *L) {
    Vector *v = luaL_checkudata(L, 1, "Vector");
    lua_pushnumber(L, v->y);
    return 1;
}

int api_vector_tostring(lua_State *L) {
    Vector *v = luaL_checkudata(L, 1, "Vector");
    lua_pushfstring(L, "Vector<x: %f, y: %f>", v->x, v->y);
    return 1;
}

int api_rect_new(lua_State *L) {
    double x = luaL_checknumber(L, 1);
    double y = luaL_checknumber(L, 2);
    double w = luaL_checknumber(L, 3);
    double h = luaL_checknumber(L, 4);
    Rect r = rect_new(x, y, w, h);

    Rect *ptr = lua_newuserdata(L, sizeof(Rect));
    *ptr = r;
    luaL_getmetatable(L, "Rect");
    lua_setmetatable(L, -2);
    return 1;
}

int api_rect_tostring(lua_State *L) {
    Rect *r = luaL_checkudata(L, 1, "Rect");
    lua_pushfstring(L, "Rect<x: %f, y:%f, w:%f, h: %f>", r->x, r->y, r->w, r->h);
    return 1;
}

int api_rect_overlaps(lua_State *L) {
    Rect *a = luaL_checkudata(L, 1, "Rect");
    Rect *b = luaL_checkudata(L, 2, "Rect");

    Vector side;
    bool overlaps = rect_overlaps(*a, *b, &side);

    // Boolean if overlaps
    lua_pushboolean(L, overlaps);

    // Side
    Vector *ptr = lua_newuserdata(L, sizeof(Vector));
    *ptr = side;
    luaL_getmetatable(L, "Vector");
    lua_setmetatable(L, -2);

    return 2;
}

int api_rect_access_x(lua_State *L) {
    Rect *r = luaL_checkudata(L, 1, "Rect");
    lua_pushnumber(L, r->x);
    return 1;
}

int api_rect_access_y(lua_State *L) {
    Rect *r = luaL_checkudata(L, 1, "Rect");
    lua_pushnumber(L, r->y);
    return 1;
}

int api_rect_access_w(lua_State *L) {
    Rect *r = luaL_checkudata(L, 1, "Rect");
    lua_pushnumber(L, r->w);
    return 1;
}

int api_rect_access_h(lua_State *L) {
    Rect *r = luaL_checkudata(L, 1, "Rect");
    lua_pushnumber(L, r->h);
    return 1;
}

int api_rect_access_position(lua_State *L) {
    int pos = lua_gettop(L);
    if (pos > 1) {
        // SET
        Rect *r = luaL_checkudata(L, 1, "Rect");
        Vector *v = luaL_checkudata(L, 2, "Vector");
        r->position = *v;
        return 0;
    }

    // GET
    Rect *r = luaL_checkudata(L, 1, "Rect");
    Vector *ptr = lua_newuserdata(L, sizeof(Vector));
    *ptr = r->position;
    luaL_getmetatable(L, "Vector");
    lua_setmetatable(L, -2);
    return 1;
}


int api_rect_access_dimension(lua_State *L) {
    Rect *r = luaL_checkudata(L, 1, "Rect");
    Vector *ptr = lua_newuserdata(L, sizeof(Vector));
    *ptr = r->dimension;
    luaL_getmetatable(L, "Vector");
    lua_setmetatable(L, -2);
    return 1;
}

int api_rect_access_center(lua_State *L) {
    Rect *r = luaL_checkudata(L, 1, "Rect");
    Vector center = rect_center(*r);
    Vector *ptr = lua_newuserdata(L, sizeof(Vector));
    *ptr = center;
    luaL_getmetatable(L, "Vector");
    lua_setmetatable(L, -2);
    return 1;
}


static const struct luaL_Reg vector_methods[] = {
        {"__add",      api_vector_add},
        {"__sub",      api_vector_sub},
        {"__mul",      api_vector_mul},
        {"__div",      api_vector_div},
        {"lerp",       api_vector_lerp},
        {"x",          api_vector_access_x},
        {"y",          api_vector_access_y},
        {"__tostring", api_vector_tostring},
        {NULL, NULL}
};


static const struct luaL_Reg rect_methods[] = {
        {"overlaps",   api_rect_overlaps},
        {"x",          api_rect_access_x},
        {"y",          api_rect_access_y},
        {"w",          api_rect_access_w},
        {"h",          api_rect_access_h},
        {"position",   api_rect_access_position},
        {"dimension",  api_rect_access_dimension},
        {"center",     api_rect_access_center},
        {"__tostring", api_rect_tostring},
        {NULL, NULL}
};

int module_vector(lua_State *L) {
    int pos = lua_gettop(L);

    // VECTOR
    luaL_newmetatable(L, "Vector");
    luaL_setfuncs(L, vector_methods, 0);

    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    lua_newtable(L);
    pos = lua_gettop(L);
    lua_pushcfunction(L, api_vector_new);
    lua_setfield(L, pos, "new");
    lua_pushcfunction(L, api_vector_lerp);
    lua_setfield(L, pos, "lerp");
    lua_pushcfunction(L, api_vector_magnitude);
    lua_setfield(L, pos, "magnitude");
    lua_pushcfunction(L, api_vector_distance);
    lua_setfield(L, pos, "distance");
    return 1;
}

int module_rect(lua_State *L) {
    int pos = lua_gettop(L);
    luaL_newmetatable(L, "Rect");
    luaL_setfuncs(L, rect_methods, 0);

    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    lua_newtable(L);
    pos = lua_gettop(L);
    lua_pushcfunction(L, api_rect_new);
    lua_setfield(L, pos, "new");

    return 1;
}


void api_math_open(lua_State *L) {
    luaL_requiref(L, "core.vector", module_vector, 0);
    lua_pop(L, 1);

    luaL_requiref(L, "core.rect", module_rect, 0);
    lua_pop(L, 1);
}
