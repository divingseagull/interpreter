#ifndef INTERPRETER_OBJECT_H
#define INTERPRETER_OBJECT_H

#include <stdbool.h>
#include <stdlib.h>

#include "str.h"
#include "cvector.h"

cvector_vector_type(void*) obj_list = NULL;


typedef union ObjectType {
    double obj_float;
//    int obj_int;
    char *obj_string;
    bool obj_bool;
    void *obj_null;
} ObjectType;

typedef enum ObjectTypeEnum {
    OBJ_NULL,
    OBJ_STRING,
    OBJ_FLOAT,
    // OBJ_INT,
    OBJ_BOOL,
} ObjectTypeEnum;

typedef struct Object {
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
    cvector_push_back(obj_list, &obj);
    return obj;
}

inline extern String object_to_string(Object obj) {
    switch (obj.obj_type) {
        case OBJ_NULL:
            return string_from_array("NULL");
        case OBJ_STRING:
            return string_from_array(obj.obj_value.obj_string);
        case OBJ_FLOAT:
            return string_from_float(obj.obj_value.obj_float);
        case OBJ_BOOL:
            return string_from_array(obj.obj_value.obj_bool ? "TOKEN_TRUE" : "TOKEN_FALSE");
    }
}

Object obj_null(void) {
    ObjectType obj_new_arg = { .obj_null = NULL };
    Object obj = object_new(obj_new_arg,
                            OBJ_NULL);
    return obj;
}
#endif // INTERPRETER_OBJECT_H
