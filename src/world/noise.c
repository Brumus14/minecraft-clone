#include "noise.h"

fnl_noise_type noise_2d_type_to_fnl(enum noise_2d_type type) {
    switch (type) {
    case NOISE_2D_TYPE_PERLIN:
        return FNL_NOISE_PERLIN;
    }
}

fnl_noise_type noise_3d_type_to_fnl(enum noise_3d_type type) {
    switch (type) {
    case NOISE_3D_TYPE_PERLIN:
        return FNL_NOISE_PERLIN;
    }
}

void noise_2d_init(struct noise_2d *noise, enum noise_2d_type type, int seed,
                   float frequency, int octaves, float lacunarity, float gain) {
    noise->state = fnlCreateState();
    noise->state.noise_type = noise_2d_type_to_fnl(type);
    noise->state.seed = seed;
    noise->state.frequency = frequency;
    noise->state.octaves = octaves;
    noise->state.lacunarity = lacunarity;
    noise->state.gain = gain;
}

float noise_2d_get(struct noise_2d *noise, float x, float y) {
    return fnlGetNoise2D(&noise->state, x, y);
}

float noise_2d_generate(struct noise_2d *noise) {
}

float noise_2d_generate_upscaled(struct noise_2d *noise) {
}

void noise_3d_init(struct noise_3d *noise, enum noise_3d_type type, int seed,
                   float frequency, int octaves, float lacunarity, float gain) {
    noise->state = fnlCreateState();
    noise->state.noise_type = noise_3d_type_to_fnl(type);
    noise->state.seed = seed;
    noise->state.frequency = frequency;
    noise->state.octaves = octaves;
    noise->state.lacunarity = lacunarity;
    noise->state.gain = gain;
}

float noise_3d_get(struct noise_3d *noise, float x, float y, float z) {
    return fnlGetNoise3D(&noise->state, x, y, z);
}

float noise_3d_generate(struct noise_3d *noise) {
}

float noise_3d_generate_upscaled(struct noise_3d *noise) {
}
