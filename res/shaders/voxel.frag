#version 400
in vec2 TexCoord;

out vec4 frag_color;

uniform sampler2D ourTexture;

void main()
{
    // frag_color = vec4(texture(ourTexture, TexCoord).rgb, 0.5);
    frag_color = texture(ourTexture, TexCoord + vec2(0.01, 0.01));
}
