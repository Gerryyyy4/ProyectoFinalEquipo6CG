#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

const float speed = 0.1; 
out vec2 TexCoords;
const float amplitude = 0.05;
const float frequency = 5.0;
const float PI = 3.14159;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main()
{
    float distance = length(aPos);
    float effect = amplitude*sin(-PI*distance*frequency+time);
    vec3 modifiedPos = aPos;
    modifiedPos.y += effect;  
    modifiedPos.x += effect * 0.1; 
    modifiedPos.z += effect * 0.1;
    
    gl_Position = projection * view * model * vec4(modifiedPos, 1);
    TexCoords = vec2(aTexCoords.x, -aTexCoords.y + time * speed);
}

