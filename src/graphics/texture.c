#include "texture.h"

#include "stb_image.h"

GLenum to_gl_filter(texture_filter filter) {
    switch (filter) {
    case TEXTURE_FILTER_LINEAR:
        return GL_LINEAR;
    case TEXTURE_FILTER_NEAREST:
        return GL_NEAREST;
    }
}

void texture_init(texture *texture, texture_filter filter) {
    glGenTextures(1, &texture->gl_id);
    texture->filter = filter;
}

void texture_bind(texture *texture) {
    glBindTexture(GL_TEXTURE_2D, texture->gl_id);

    GLenum error = glGetError();
    if (error != 0) {
        printf("texture_bind: %d\n", error);
    }

    GLenum gl_filter = GL_NEAREST;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_filter);
}

void texture_load(texture *texture, char *path) {
    int width;
    int height;
    int channels;

    unsigned char *stb_texture = stbi_load(path, &width, &height, &channels, 0);

    GLenum gl_format;

    if (channels == 4) {
        gl_format = GL_RGBA;
    } else {
        gl_format = GL_RGB;
    }

    texture_bind(texture);

    glTexImage2D(GL_TEXTURE_2D, 0, gl_format, width, height, 0, gl_format,
                 GL_UNSIGNED_BYTE, stb_texture);

    stbi_image_free(stb_texture);
}
