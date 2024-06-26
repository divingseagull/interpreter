//#ifndef INTERPRETER_CMAKE_BOX_H
//#define INTERPRETER_CMAKE_BOX_H
//
//#include <stdlib.h>
//#include <cvector.h>
//
//#define defineBox(type) typedef struct type##Box {                    \
//    type *data_ptr;                                             \
//} type##Box;                                                    \
//                                                                \
//cvector_vector_type(Box) box_list = NULL;                       \
//                                                                \
//type##Box box_new(type *source_data_ptr, size_t source_data_size) {   \
//    void *box_data_ptr = calloc(1, source_data_size);           \
//    memcpy(box_data_ptr, source_data_ptr, source_data_size);    \
//    type##Box new_box = {                                             \
//            .data_ptr = box_data_ptr,                       \
//    };                                                          \
//    cvector_push_back(box_list, new_box);                       \
//    return new_box;                                             \
//}
//
//#endif //INTERPRETER_CMAKE_BOX_H
