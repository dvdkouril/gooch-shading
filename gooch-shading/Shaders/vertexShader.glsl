#version 400

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vn;

out vec4 pos;
out vec3 n;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void){
    vec4 v = vec4(vp, 1.0);
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    pos = view * model * v;
    n = normalize(normalMatrix * vn);
    gl_Position = projection * view * model * v;
}
