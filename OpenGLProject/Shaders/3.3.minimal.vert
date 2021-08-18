#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 global_mat4 = mat4(1);
uniform mat4 view_mat4 = mat4(1);
uniform mat4 projection_mat4 = mat4(1);
void main()
{
    gl_Position = projection_mat4 * view_mat4 * global_mat4 * vec4(aPos, 1.0);
} 