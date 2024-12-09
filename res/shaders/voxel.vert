#version 400

layout (location = 0) in vec3 position_vert;
layout (location = 1) in vec2 texture_coordinate_vert;
layout (location = 2) in vec3 normal_vert;

out vec3 position_frag;
out vec2 texture_coordinate_frag;
out vec3 normal_frag;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position_vert, 1.0);

    position_frag = vec3(model * vec4(position_vert, 1.0));
    texture_coordinate_frag = texture_coordinate_vert;
    normal_frag = normal_vert;
}
