#version 400

in vec3 position_frag;
in vec2 texture_coordinate_frag;
in vec3 normal_frag;

out vec4 frag_color_out;

uniform vec3 camera_position;
uniform sampler2D texture_sampler;

void main()
{
    // frag_color_out = texture(texture_sampler, texture_coordinate_frag); // + vec2(0.01, 0.01)

    float brightness;

    // check if z component of normal being generated correctly
    if (normal_frag.y == -1) {
        brightness = 1.0 / 6;
    }

    if (normal_frag.z == -1) {
        brightness = 2.0 / 6;
    }

    if (normal_frag.x == -1) {
        brightness = 3.0 / 6;
    }

    if (normal_frag.x == 1) {
        brightness = 4.0 / 6;
    }

    if (normal_frag.z == 1) {
        brightness = 5.0 / 6;
    }

    if (normal_frag.y == 1) {
        brightness = 6.0 / 6;
    }

    vec4 texture_colour = texture(texture_sampler, texture_coordinate_frag);

    // refactor this
    vec3 norm = normalize(normal_frag);
    vec3 light_direction = normalize(camera_position - position_frag);
    float diff = max(dot(norm, light_direction), 0.0) * 0.5 + 0.5;
    vec3 diffuse = diff * vec3(1,1,1);

    vec3 view_direction = light_direction;
    vec3 reflect_direction = reflect(-light_direction, norm);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), 32);
    vec3 specular = 0.0 * spec * vec3(1,1,1);

    vec3 result = (diffuse + specular) * texture_colour.xyz;

    frag_color_out = vec4(result, 1.0);

    // frag_color_out = vec4(texture_colour.xyz * brightness, 1.0);
}
