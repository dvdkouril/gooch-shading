#version 400

in vec3 n;
in vec4 pos;

out vec4 frag_color; // first "out" variable will be taken as fragment shader output

uniform mat4 model;

void main(void){
   vec3 lightPos = vec3(5.0, 5.0, 5.0);
   vec3 l = normalize(lightPos - vec3(pos));
   vec3 v = normalize(-vec3(pos));
   vec3 h = normalize(v + l);
   vec4 diffuse = vec4(0.5, 0.5, 0.5, 1.0) * (0.5, 0.5, 0.5, 1.0) * max(0.0, dot(n,l));
   vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0) * (0.1, 0.1, 0.1, 1.0);
    
   frag_color = ambient + diffuse;
}
