//#include <stdlib.h>
//
//#include "str.h"
//
//Str *str_new(void) {
//    Str _str = {
//            0,
//            0,
//            ""
//    };
//    void *ptr = malloc(sizeof(_str));
//    return ptr;
//}
//
//Str *str_realloc(Str self, size_t size) {
//    void *ptr = realloc(&self, size);
//    return ptr;
//}
//
//void str_free(Str *self) {
//    free(self);
//}
//
//size_t str_len(Str self) {
//    return self._len;
//}
