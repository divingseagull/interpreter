#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string.h>

#include "expr.h"
#include "object.h"
#include "str.h"


void interpret(Expr expression);
Object evaluate(Expr expr);
Object eval_literal_expr(Expr expr);
Object eval_grouping_expr(Expr expr);
Object eval_unary_expr(Expr expr);
Object eval_binary_expr(Expr expr);

bool is_truthy(Object obj);
Object return_null(void);

void interpret(Expr expression) {
    Object value = evaluate(expression);
    printf("%s", object_to_string(value));
}

Object evaluate(Expr expr) {
    switch (expr.type_enum) {
        case EXPR_BINARY:
            return eval_binary_expr(expr);
        case EXPR_UNARY:
            return eval_unary_expr(expr);
        case EXPR_LITERAL:
            return eval_literal_expr(expr);
        case EXPR_GROUPING:
            return eval_grouping_expr(expr);
    }
}

Object eval_unary_expr(Expr expr) {
    Object right = expr.union_type->unary.right.union_type->literal.value;

    switch (expr.union_type->unary.operator_enum.unary_enum) {
        case OP_MINUS_UNARY:
            switch (expr.union_type->unary.op.literalType) {
                case OBJ_FLOAT:
                    ObjectType obj_new_arg = { .obj_float = -right.obj_value.obj_float };
                    Object obj = object_new(obj_new_arg,
                                            OBJ_FLOAT);
                    return obj;
            }
        case OP_BANG:
            bool truth_result = !is_truthy(right);
            ObjectType obj_new_arg = { .obj_bool = truth_result };
            Object obj = object_new(obj_new_arg,
                                    OBJ_BOOL);
            return obj;
    }
}

Object eval_binary_expr(Expr expr) {
    Expr left_expr;
    left_expr.union_type->literal.value = expr.union_type->binary.left.union_type->literal.value;
    left_expr.type_enum = EXPR_LITERAL;
    Object left = evaluate(left_expr);

    Expr right_expr;
    right_expr.union_type->literal.value = expr.union_type->binary.right.union_type->literal.value;
    right_expr.type_enum = EXPR_LITERAL;
    Object right = evaluate(right_expr);

    switch (expr.union_type->binary.operator_enum.binary_enum) {
        case OP_PLUS:
            if (left.obj_type == right.obj_type) {
                Object obj;
                switch (left.obj_type) {
                    case OBJ_FLOAT:
                        ObjectType obj_new_arg = { .obj_float = left.obj_value.obj_float + right.obj_value.obj_float };
                        obj = object_new(obj_new_arg,
                                         OBJ_FLOAT);
                        break;
                    // case OBJ_STRING:
                    //     char *str = strcat(left.obj_value.obj_string, right.obj_value.obj_string);
                    //     char *str_ptr = malloc(strlen(str)+1);
                    //     memcpy(str_ptr, str, strlen(str)+1);
                    //     obj.obj_type = OBJ_STRING;
                    //     obj.obj_value.obj_string = str_ptr;
                    //     obj = object_new({}, .
                    //                      OBJ_STRING);
                    //     break;
                }
                return obj;
            }
        case OP_MINUS_BINARY:
            if (left.obj_type == right.obj_type) {
                switch (left.obj_type) {
                    case OBJ_FLOAT:
                        ObjectType obj_new_arg = { .obj_float = left.obj_value.obj_float - right.obj_value.obj_float };
                        Object obj = object_new(obj_new_arg,
                                                OBJ_FLOAT);
                        return obj;
                }
            }
        case OP_SLASH:
            if (left.obj_type == right.obj_type) {
                switch (left.obj_type) {
                    case OBJ_FLOAT:
                        ObjectType obj_new_arg = { .obj_float = left.obj_value.obj_float / right.obj_value.obj_float };
                        Object obj = object_new(obj_new_arg,
                                                OBJ_FLOAT);
                        return obj;
                }
            }
        case OP_STAR:
            if (left.obj_type == right.obj_type) {
                switch (left.obj_type) {
                    case OBJ_FLOAT:
                        ObjectType obj_new_arg = { .obj_float = left.obj_value.obj_float * right.obj_value.obj_float };
                        Object obj = object_new(obj_new_arg,
                                                OBJ_FLOAT);
                        return obj;
                }
            }
//        case OP_PERCENT:
            break;
        case OP_GREATER:
            if (left.obj_type == right.obj_type) {
                switch (left.obj_type) {
                    case OBJ_FLOAT:
                        ObjectType obj_new_arg = { .obj_bool = left.obj_value.obj_float < right.obj_value.obj_float };
                        Object obj = object_new(obj_new_arg,
                                                OBJ_BOOL);
                        return obj;
                }
            }
        case OP_GREATER_EQUAL:
            if (left.obj_type == right.obj_type) {
                switch (left.obj_type) {
                    case OBJ_FLOAT:
                        ObjectType obj_new_arg = { .obj_bool = left.obj_value.obj_float <= right.obj_value.obj_float };
                        Object obj = object_new(obj_new_arg,
                                                OBJ_BOOL);
                        return obj;
                }
            }
        case OP_LESS:
            if (left.obj_type == right.obj_type) {
                switch (left.obj_type) {
                    case OBJ_FLOAT:
                        ObjectType obj_new_arg = { .obj_bool = left.obj_value.obj_float > right.obj_value.obj_float };
                        Object obj = object_new(obj_new_arg,
                                                OBJ_BOOL);
                        return obj;
                }
            }
        case OP_LESS_EQUAL:
            if (left.obj_type == right.obj_type) {
                switch (left.obj_type) {
                    case OBJ_FLOAT:
                        ObjectType obj_new_arg = { .obj_bool = left.obj_value.obj_float >= right.obj_value.obj_float };
                        Object obj = object_new(obj_new_arg,
                                                OBJ_BOOL);
                        return obj;
                }
            }
        case OP_BANG_EQUAL:
            if (left.obj_type == right.obj_type) {
                Object obj;
                ObjectType obj_new_arg;
                switch (left.obj_type) {
                    case OBJ_FLOAT:
                        obj_new_arg.obj_bool = left.obj_value.obj_float != right.obj_value.obj_float;
                        obj = object_new(obj_new_arg,
                                         OBJ_BOOL);
                        break;
                    case OBJ_STRING:
                        obj_new_arg.obj_bool = strcmp(left.obj_value.obj_string, right.obj_value.obj_string) != 0;
                        obj = object_new(obj_new_arg,
                                                OBJ_BOOL);
                        break;
                }
                return obj;
            }
        case OP_EQUAL_EQUAL:
            if (left.obj_type == right.obj_type) {
                Object obj;
                ObjectType obj_new_arg;
                switch (left.obj_type) {
                    case OBJ_FLOAT:
                        obj_new_arg.obj_bool = left.obj_value.obj_float == right.obj_value.obj_float;
                        obj = object_new(obj_new_arg,
                                         OBJ_BOOL);
                        break;
                    case OBJ_STRING:
                        obj_new_arg.obj_bool = strcmp(left.obj_value.obj_string, right.obj_value.obj_string) == 0;
                        obj = object_new(obj_new_arg,
                                         OBJ_BOOL);
                        break;
                }
                return obj;
            }
    }
    return return_null();
}

bool is_truthy(Object obj) {
    switch (obj.obj_type) {
        case OBJ_NULL:
            return false;
        case OBJ_BOOL:
            return obj.obj_value.obj_bool;
        case OBJ_FLOAT:
            return (obj.obj_value.obj_float != 0.0 ? false : true);
        case OBJ_STRING:
            return strcmp(obj.obj_value.obj_string, "") == 0 ? false : true;
    }
}

Object return_null(void) {
    ObjectType obj_new_arg = { .obj_null = NULL };
    Object obj = object_new(obj_new_arg,
                            OBJ_NULL);
    return obj;
}
#endif //INTERPRETER_H
