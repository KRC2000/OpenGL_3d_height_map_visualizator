#version 330 core
out vec4 FragColor;

uniform bool light;

in vec3 cameraPos;
in vec3 normal;
in vec3 fragPos;
in vec4 vertexColor;

void main()
{
	vec3 toCameraDir = normalize(cameraPos - fragPos); 
	float diff = max(dot(normal, toCameraDir), 0.0);

	if (light) FragColor =  vertexColor * vec4(1f, 1f, 1f, 0.f)* diff;
	else FragColor =  vertexColor;
}