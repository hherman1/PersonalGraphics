#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;

// Values that stay constant for the whole mesh.
uniform mat4 model;
uniform mat4 light_view;
uniform mat4 light_projection;


void main(){
 gl_Position =  light_projection * light_view * model * vec4(vertexPosition_modelspace,1);
}