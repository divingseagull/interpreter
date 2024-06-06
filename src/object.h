#ifndef INTERPRETER_OBJECT_H
#define INTERPRETER_OBJECT_H

#include "str.h"

typedef union {
    double obj_float;
//    int obj_int;
    char *obj_string;
    bool obj_bool;
    void *obj_null;
} ObjectType;

typedef enum {
    OBJ_NULL,
    OBJ_STRING,
    OBJ_FLOAT,
    // OBJ_INT,
    OBJ_BOOL,
} ObjectTypeEnum;

typedef struct {
    ObjectType obj_value;
    ObjectTypeEnum obj_type;
    void *address;
} Object;

inline extern Object object_new(ObjectType obj_value, ObjectTypeEnum obj_type) {
    Object obj = {
        .obj_value = obj_value,
        .obj_type = obj_type,
    };
    void *ptr = malloc(sizeof(obj));
    obj.address = ptr;
    return obj;
}

inline extern String object_to_string(Object obj) {
    switch (obj.obj_type) {
        case OBJ_NULL:
            return str_from("NULL");
        case OBJ_STRING:
            return str_from(obj.obj_value.obj_string);
        case OBJ_FLOAT:
            return str_from_float(obj.obj_value.obj_float);
        case OBJ_BOOL:
            return str_from(obj.obj_value.obj_bool ? "TRUE" : "FALSE");
    }
}

#endif // INTERPRETER_OBJECT_H
