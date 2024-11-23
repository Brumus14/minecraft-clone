#version 400
in vec2 TexCoord;

out vec4 frag_color;

uniform sampler2D ourTexture;

void main()
{
    frag_color = texture(ourTexture, TexCoord);
}
