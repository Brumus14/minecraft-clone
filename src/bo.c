#include "bo.h"

GLenum to_gl_usage(bo_usage usage) {
    switch (usage) {
    case STREAM_DRAW:
        return GL_STREAM_DRAW;
    case STREAM_READ:
        return GL_STREAM_READ;
    case STREAM_COPY:
        return GL_STREAM_COPY;
    case STATIC_DRAW:
        return GL_STATIC_DRAW;
    case STATIC_READ:
        return GL_STATIC_READ;
    case STATIC_COPY:
        return GL_STATIC_COPY;
    case DYNAMIC_DRAW:
        return GL_DYNAMIC_DRAW;
    case DYNAMIC_READ:
        return GL_DYNAMIC_READ;
    case DYNAMIC_COPY:
        return GL_DYNAMIC_COPY;
    }
}

GLenum to_gl_type(bo_type type) {
    switch (type) {
    case BUFFER_TYPE_VERTEX:
        return GL_ARRAY_BUFFER;
    case BUFFER_TYPE_INDEX:
        return GL_ELEMENT_ARRAY_BUFFER;
    }
}

void init_bo(bo *bo, bo_type type) {
    bo->type = type;

    glGenBuffers(1, &bo->bo_id);
}

void bind_bo(bo *bo) {
    GLenum gl_type = to_gl_type(bo->type);
    glBindBuffer(gl_type, bo->bo_id);
}

void upload_bo(bo *bo, size_t data_size, void *data, bo_usage usage) {
    GLenum gl_type = to_gl_type(bo->type);
    glBufferData(gl_type, data_size, data, to_gl_usage(usage));
}
