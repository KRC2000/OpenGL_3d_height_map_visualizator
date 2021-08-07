#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec4 vertexColor_pass[];
out vec4 vertexColor;


out vec3 normal;
out vec3 fragPos;

vec3 GetTriangleNormal();


void main()
{
	vec3 triangleNormal = GetTriangleNormal();
	for (int i = 0; i < gl_in.length(); i++)
	{
		gl_Position = gl_in[i].gl_Position; 
		vertexColor = vertexColor_pass[i];
		fragPos = vec3(gl_in[i].gl_Position);
		normal = triangleNormal;
		EmitVertex();
	}
	EndPrimitive();
}


vec3 GetTriangleNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
//	vec3 a = vec3(fragPos_pass[0]) - vec3(fragPos_pass[1]);
//	vec3 b = vec3(fragPos_pass[2]) - vec3(fragPos_pass[1]);
	return normalize(cross(a, b));
} 