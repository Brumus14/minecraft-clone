#ifndef VBO_H
#define VBO_H

#include "glad/glad.h"

typedef enum bo_type { BO_TYPE_VERTEX, BO_TYPE_INDEX } bo_type;

typedef struct bo {
    GLuint gl_id;
    bo_type type;
} bo;

typedef enum bo_usage {
    BO_USAGE_STREAM_DRAW,
    BO_USAGE_STREAM_READ,
    BO_USAGE_STREAM_COPY,
    BO_USAGE_STATIC_DRAW,
    BO_USAGE_STATIC_READ,
    BO_USAGE_STATIC_COPY,
    BO_USAGE_DYNAMIC_DRAW,
    BO_USAGE_DYNAMIC_READ,
    BO_USAGE_DYNAMIC_COPY
} bo_usage;

GLenum to_gl_usage(bo_usage usage);
GLenum to_gl_type(bo_type type);
void bo_init(bo *bo, bo_type type);
void bo_bind(bo *bo);
void bo_upload(bo *bo, int data_size, void *data, bo_usage usage);
void bo_destroy(bo *bo);
int bo_get_size(bo *bo);

#endif
