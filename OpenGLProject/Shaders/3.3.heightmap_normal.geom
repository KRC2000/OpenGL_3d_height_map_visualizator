#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in vec4 vertexColor_pass[];
out vec4 vertexColor;

in vec4 posWithoutProjection[];
in mat4 matrix[];

uniform float normalLength = 1;

vec3 GetTriangleNormal();
void main()
{
	//vec3 triangleNormal = GetTriangleNormal();
	for (int i = 0; i < gl_in.length(); i++)
	{
		gl_Position = matrix[i] * gl_in[i].gl_Position; 
		vertexColor = vertexColor_pass[i];
		EmitVertex();
		//transpose(inverse(
		gl_Position = matrix[i] * (gl_in[i].gl_Position + vec4(GetTriangleNormal(), 0) * (-normalLength * 0.05)); 
		vertexColor = vertexColor_pass[i];
		EmitVertex();

		EndPrimitive();
	}

}


vec3 GetTriangleNormal()
{
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(a, b));
} 