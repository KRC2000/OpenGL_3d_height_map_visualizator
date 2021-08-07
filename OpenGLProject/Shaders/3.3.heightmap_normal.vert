#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model_mat4 = mat4(1);
uniform mat4 view_mat4 = mat4(1);
uniform mat4 projection_mat4 = mat4(1);

uniform sampler2D hMap;
uniform float heightMultiplier = 1;
uniform bool textured = true;

out vec4 vertexColor_pass;

out vec4 posWithoutProjection;
out mat4 matrix;

void main()
{
    vec4 color = texture(hMap, vec2((aPos.x+1)/2, (aPos.z+1)/2));

    matrix = projection_mat4 *view_mat4 * model_mat4;
    gl_Position = vec4(aPos.x, (color.x - color.z)/heightMultiplier, aPos.z, 1.0);

    posWithoutProjection =  vec4(aPos.x, (color.x - color.z)/heightMultiplier, aPos.z, 1.0);

    if (textured) vertexColor_pass = color;
    else vertexColor_pass = vec4(0);
} 