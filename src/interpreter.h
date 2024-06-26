#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string.h>

#include "cvector.h"

#include "types_expr.h"
#include "types_statement.h"
#include "object.h"
#include "str.h"
#include "environment.h"


void interpret(cvector_vector_type(Stmt) statements);
void execute(Stmt stmt);
Object evaluate(Expr expr);
Object eval_literal_expr(ExprLiteral expr);
Object eval_grouping_expr(ExprGrouping expr, ExprTypeEnum type);
Object eval_unary_expr(ExprUnary expr);
Object eval_binary_expr(ExprBinary expr);
Object eval_variable_expr(ExprVariable expr);
Object eval_assign_expr(ExprAssign expr);

//void eval_stmt_print(StmtPrint stmt);
void eval_stmt_var(StmtVar stmt);

bool is_truthy(Object obj);


struct {
    cvector_vector_type(Stmt) statements;
    cvector_vector_type(Expr) expressions;
} Interpreter;

void interpreter_init(void) {
    Interpreter.statements = NULL;
    Interpreter.expressions = NULL;
}

void interpret(cvector_vector_type(Stmt) statements) {
    for (size_t i = 0; i < cvector_size(statements); i++) {
        execute(*cvector_at(statements, i));
    }
}

void execute(Stmt stmt) {
    Object value;
    switch (stmt.type_enum) {
        case STMT_EXPRESSION:
        case STMT_PRINT:
            value = evaluate(stmt.union_type.stmt_expression.expression);
            printf("%s", (char*)(object_to_string(value).address));
            printf("\n");
            break;
        case STMT_VAR:
            eval_stmt_var(stmt.union_type.stmt_var);
    }
}

Object evaluate(Expr expr) {
    switch (expr.type_enum) {
        case EXPR_BINARY:
            return eval_binary_expr(expr.union_type.binary);
        case EXPR_UNARY:
            return eval_unary_expr(expr.union_type.unary);
        case EXPR_LITERAL:
            return eval_literal_expr(expr.union_type.literal);
        case EXPR_GROUPING:
            return eval_grouping_expr(expr.union_type.grouping, expr.type_enum);
        case EXPR_VAR:
            return eval_variable_expr(expr.union_type.variable);
        case EXPR_ASSIGN:
            return eval_assign_expr(expr.union_type.assign);
    }
}

Object eval_literal_expr(ExprLiteral expr) {
    return expr.value;
}

Object eval_grouping_expr(ExprGrouping expr, ExprTypeEnum type) {
    switch (type) {
        case EXPR_BINARY:
            return eval_binary_expr(expr.expression.binary);
        case EXPR_UNARY:
            return eval_unary_expr(expr.expression.unary);
        case EXPR_LITERAL:
            return eval_literal_expr(expr.expression.literal);
    }
}

Object eval_unary_expr(ExprUnary expr) {
    Object right;
    switch (expr.right->type_enum) {
        case EXPR_BINARY:
            right = eval_binary_expr(expr.right->union_type.binary);
            break;
        case EXPR_UNARY:
            right = eval_unary_expr(expr.right->union_type.unary);
            break;
        case EXPR_LITERAL:
            right = expr.right->union_type.literal.value;
            break;
        // case EXPR_GROUPING:
        //     right = eval_grouping_expr(expr.right->union_type.grouping, expr.);
        //     break;
        case EXPR_VAR:
            right = eval_variable_expr(expr.right->union_type.variable);
            break;
        // case EXPR_ASSIGN:
        //     break;
    }
    switch (expr.operator_enum) {
        case OP_UNARY_MINUS:
            switch (right.obj_type) {
                case OBJ_FLOAT:
                    ObjectType obj_new_arg = { .obj_float = -right.obj_value.obj_float };
                    Object obj = object_new(obj_new_arg,
                                            OBJ_FLOAT);
                    return obj;
                default:
                    raise_error(RES_RUNTIME_INVALID_TYPE, true, string_from_array("Invalid type"));
                    break;
            }
        case OP_UNARY_BANG:
            bool truth_result = !is_truthy(right);
            ObjectType obj_new_arg = { .obj_bool = truth_result };
            Object obj = object_new(obj_new_arg,
                                    OBJ_BOOL);
            return obj;
    }
}

Object eval_binary_expr(ExprBinary expr) {
    Expr left_expr;
    left_expr.union_type.literal.value = expr.left->union_type.literal.value;
    left_expr.type_enum = EXPR_LITERAL;
    Object left = evaluate(left_expr);

    Expr right_expr;
    right_expr.union_type.literal.value = expr.right->union_type.literal.value;
    right_expr.type_enum = EXPR_LITERAL;
    Object right = evaluate(right_expr);  // 우항 평가에서 문제 발생

    switch (expr.operator_enum) {
        case OP_BINARY_PLUS:
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
        case OP_BINARY_MINUS:
            if (left.obj_type == right.obj_type) {
                switch (left.obj_type) {
                    case OBJ_FLOAT:
                        ObjectType obj_new_arg = { .obj_float = left.obj_value.obj_float - right.obj_value.obj_float };
                        Object obj = object_new(obj_new_arg,
                                                OBJ_FLOAT);
                        return obj;
                }
            }
        case OP_BINARY_SLASH:
            if (left.obj_type == right.obj_type) {
                switch (left.obj_type) {
                    case OBJ_FLOAT:
                        if (right.obj_value.obj_float == 0) raise_error(RES_RUNTIME_DIVISION_BY_ZERO,
                                                                        true,
                                                                        string_from_array("Division by zero"));
                        ObjectType obj_new_arg = { .obj_float = left.obj_value.obj_float / right.obj_value.obj_float };
                        Object obj = object_new(obj_new_arg,
                                                OBJ_FLOAT);
                        return obj;
                }
            }
        case OP_BINARY_STAR:
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
        case OP_BINARY_GREATER:
            if (left.obj_type == right.obj_type) {
                switch (left.obj_type) {
                    case OBJ_FLOAT:
                        ObjectType obj_new_arg = { .obj_bool = left.obj_value.obj_float < right.obj_value.obj_float };
                        Object obj = object_new(obj_new_arg,
                                                OBJ_BOOL);
                        return obj;
                }
            }
        case OP_BINARY_GREATER_EQUAL:
            if (left.obj_type == right.obj_type) {
                switch (left.obj_type) {
                    case OBJ_FLOAT:
                        ObjectType obj_new_arg = { .obj_bool = left.obj_value.obj_float <= right.obj_value.obj_float };
                        Object obj = object_new(obj_new_arg,
                                                OBJ_BOOL);
                        return obj;
                }
            }
        case OP_BINARY_LESS:
            if (left.obj_type == right.obj_type) {
                switch (left.obj_type) {
                    case OBJ_FLOAT:
                        ObjectType obj_new_arg = { .obj_bool = left.obj_value.obj_float > right.obj_value.obj_float };
                        Object obj = object_new(obj_new_arg,
                                                OBJ_BOOL);
                        return obj;
                }
            }
        case OP_BINARY_LESS_EQUAL:
            if (left.obj_type == right.obj_type) {
                switch (left.obj_type) {
                    case OBJ_FLOAT:
                        ObjectType obj_new_arg = { .obj_bool = left.obj_value.obj_float >= right.obj_value.obj_float };
                        Object obj = object_new(obj_new_arg,
                                                OBJ_BOOL);
                        return obj;
                }
            }
        case OP_BINARY_BANG_EQUAL:
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
        case OP_BINARY_EQUAL_EQUAL:
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
    return obj_null();
}

Object eval_variable_expr(ExprVariable expr) {
    return *environment_get(expr.name.lexeme);
}

Object eval_assign_expr(ExprAssign expr) {
    Object value = evaluate(*expr.value);
    environment_assign(expr.name.lexeme, &value);
    return value;
}

void eval_stmt_print(StmtPrint stmt) {

}

void eval_stmt_var(StmtVar stmt) {
    Object value = obj_null();
    if ((void *) stmt.initializer.type_enum != NULL) {
        value = evaluate(stmt.initializer);
    }
    environment_define(stmt.name.lexeme, value);
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

#endif //INTERPRETER_H
