#version 400
out vec4 frag_color;

in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    // frag_color = vec4(0.5, 0.0, 0.5, 1.0);
    frag_color = texture(ourTexture, TexCoord);
}
