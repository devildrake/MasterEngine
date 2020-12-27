#version 440

in vec3 FragPos;  
uniform vec3 lightEmitterColor;

out vec4 outColor;
void main() {
outColor = vec4(lightEmitterColor,1.0);
}