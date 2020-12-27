#version 440

uniform mat4 matVP;
uniform mat4 matGeo;

layout (location = 0) in vec3 pos;


out vec3 FragPos;  

void main() {

    FragPos = vec3(matGeo * vec4(pos, 1.0));
    gl_Position = matVP * matGeo * vec4(pos, 1);
    


}