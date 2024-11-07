#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D texture1;

uniform int activaTransparencia;
uniform vec4 colorAlpha;

void main()
{
    vec4 texColor = texture(texture1, TexCoords);
    FragColor = texColor * colorAlpha;
    if (FragColor.a < 0.1 && activaTransparencia == 1)
        discard;
}
