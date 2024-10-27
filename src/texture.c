#include "texture.h"

#include "stb_image.h"

void texture_init(texture *texture) {
    glGenTextures(1, &texture->gl_id);
}

void texture_bind(texture *texture) {
    glBindTexture(GL_TEXTURE_2D, texture->gl_id);
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

    glTexImage2D(GL_TEXTURE_2D, 0, gl_format, width, height, 0, gl_format,
                 GL_UNSIGNED_BYTE, stb_texture);
}
