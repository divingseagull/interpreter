#ifndef INTERPRETER_CMAKE_ENVIRONMENT_H
#define INTERPRETER_CMAKE_ENVIRONMENT_H

#include "hashmap.h"

#include "str.h"
#include "object.h"
#include "types_token.h"

#define ENVIRONMENT_INITIAL_SIZE 5

struct {
    struct hashmap_s values;
} Environment;

void environment_init(void) {
    hashmap_create(ENVIRONMENT_INITIAL_SIZE, &Environment.values);
}

void environment_define(String name, Object value) {
    hashmap_put(&Environment.values, name.address, name.len, value.address);
}

Object *environment_get(String name) {  // 리턴된 데이터가 온전하지 않음, 다른 타입으로 캐스팅하거나 다른 함수를 사용하는것을 고려해보기
    Object *result = NULL;
    result = hashmap_get(&Environment.values, name.address, name.len);
    return result;
}

void environment_assign(String name, Object *ptr) {
    if (hashmap_get(&Environment.values, name.address, name.len)) {
        hashmap_put(&Environment.values, name.address, name.len, ptr);
    }
}

#endif //INTERPRETER_CMAKE_ENVIRONMENT_H
