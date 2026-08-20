#ifndef NOISE_H
#define NOISE_H

#include "FastNoiseLite.h"

enum noise_2d_type {
    NOISE_2D_TYPE_PERLIN,
};

struct noise_2d {
    fnl_state state;
};

enum noise_3d_type {
    NOISE_3D_TYPE_PERLIN,
};

struct noise_3d {
    fnl_state state;
};

void noise_2d_init(struct noise_2d *noise, enum noise_2d_type type, int seed,
                   float frequency, int octaves, float lacunarity, float gain);
float noise_2d_get(struct noise_2d *noise, float x, float y);
float noise_2d_generate(struct noise_2d *noise);
float noise_2d_generate_upscaled(struct noise_2d *noise);

void noise_3d_init(struct noise_3d *noise, enum noise_3d_type type, int seed,
                   float frequency, int octaves, float lacunarity, float gain);
float noise_3d_get(struct noise_3d *noise, float x, float y, float z);
float noise_3d_generate(struct noise_3d *noise);
float noise_3d_generate_upscaled(struct noise_3d *noise);

#endif
