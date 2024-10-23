#ifndef VBO_H
#define VBO_H

#include <stdio.h>
#include <glad/glad.h>

typedef enum bo_type { BUFFER_TYPE_VERTEX, BUFFER_TYPE_INDEX } bo_type;

typedef struct bo {
    GLuint bo_id;
    bo_type type;
} bo;

typedef enum bo_usage {
    STREAM_DRAW,
    STREAM_READ,
    STREAM_COPY,
    STATIC_DRAW,
    STATIC_READ,
    STATIC_COPY,
    DYNAMIC_DRAW,
    DYNAMIC_READ,
    DYNAMIC_COPY
} bo_usage;

GLenum to_gl_usage(bo_usage usage);
GLenum to_gl_type(bo_type type);
void init_bo(bo *bo, bo_type type);
void bind_bo(bo *bo);
void upload_bo(bo *bo, size_t data_size, void *data, bo_usage usage);

#endif
