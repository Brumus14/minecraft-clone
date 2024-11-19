#include "bo.h"

#include <stdio.h>
#include "gl_util.h"

GLenum to_gl_usage(bo_usage usage) {
    switch (usage) {
    case BO_USAGE_STREAM_DRAW:
        return GL_STREAM_DRAW;
    case BO_USAGE_STREAM_READ:
        return GL_STREAM_READ;
    case BO_USAGE_STREAM_COPY:
        return GL_STREAM_COPY;
    case BO_USAGE_STATIC_DRAW:
        return GL_STATIC_DRAW;
    case BO_USAGE_STATIC_READ:
        return GL_STATIC_READ;
    case BO_USAGE_STATIC_COPY:
        return GL_STATIC_COPY;
    case BO_USAGE_DYNAMIC_DRAW:
        return GL_DYNAMIC_DRAW;
    case BO_USAGE_DYNAMIC_READ:
        return GL_DYNAMIC_READ;
    case BO_USAGE_DYNAMIC_COPY:
        return GL_DYNAMIC_COPY;
    default:
        fprintf(stderr, "unknown buffer object usage: %d\n", usage);
        return GL_STATIC_DRAW;
    }
}

GLenum to_gl_type(bo_type type) {
    switch (type) {
    case BO_TYPE_VERTEX:
        return GL_ARRAY_BUFFER;
    case BO_TYPE_INDEX:
        return GL_ELEMENT_ARRAY_BUFFER;
    default:
        fprintf(stderr, "unknown buffer object type: %d\n", type);
        return GL_ARRAY_BUFFER;
    }
}

void bo_init(bo *bo, bo_type type) {
    if (bo == NULL) {
        fprintf(stderr, "bo_init: buffer object is null\n");
        return;
    }

    bo->type = type;
    GL_CALL(glGenBuffers(1, &bo->gl_id));
}

void bo_bind(bo *bo) {
    if (bo == NULL) {
        fprintf(stderr, "bo_bind: buffer object is null\n");
        return;
    }

    GLenum gl_type = to_gl_type(bo->type);
    GL_CALL(glBindBuffer(gl_type, bo->gl_id));
}

void bo_upload(bo *bo, int data_size, void *data, bo_usage usage) {
    if (bo == NULL) {
        fprintf(stderr, "bo_upload: buffer object is null\n");
        return;
    }

    bo_bind(bo);

    GLenum gl_type = to_gl_type(bo->type);
    GL_CALL(glBufferData(gl_type, data_size, data, to_gl_usage(usage)));
}

void bo_delete(bo *bo) {
    GL_CALL(glDeleteBuffers(1, &bo->gl_id));
}
