//#include <stdlib.h>
//
//#ifndef INTERPRETER_STR_H
//#define INTERPRETER_STR_H
//
//typedef struct {
//    size_t _len;
//    size_t _capacity;
//    char *data;  // 포인터인거 반드시 기억하기, 동적 할당된 배열의 시작 주소를 가리킴
//} Str;
//
//
//// 필수적으로 구현
//Str *str_new(void);
//Str *str_realloc(Str self, size_t size);
//void str_free(Str *self);  // 할당 해제. 해제하지 않을 시 메모리에 계속 상주하므로 메모리 누수 발생함.
//size_t str_len(Str self);
//Str str_from(char *_from);  // char[]에서 Str 생성
//Str str_concat_str(Str self, Str str2);  // Str과 Str 결합
//Str str_concat_char_array(Str self, char *str2);  // Str과 char[] 결합
//Str str_slice(Str self, size_t start);  // 파이썬의 슬라이싱과 같은 그것
//Str str_slice_w_end(Str self, size_t start, size_t end);
//
//// 필요할 떄 구현
//Str str_slice_w_step(Str self, size_t start, size_t end, size_t step);
//Str str_format(Str self, ...);  // printf처럼 포매팅된 문자열을 돌려줌
//Str str_split(Str self, Str delimiter);  // 파이썬의 str.split()과 같음, 인자가 Str 타입
//Str str_split_char_array(Str self, char *delimiter);  // 인자가 char[]
//Str str_strip(Str self);  // Str의 좌우의 공백 제거
//Str str_lstrip(Str self);  // Str의 좌측의 공백 제거
//Str str_rstrip(Str self);  // Str의 우측의 공백 제거
//
//#endif //INTERPRETER_STR_H
