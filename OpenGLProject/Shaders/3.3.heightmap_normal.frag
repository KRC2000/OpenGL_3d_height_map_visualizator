#version 330 core
out vec4 FragColor;

// in vec3 cameraPos;
// in vec3 normal;
// in vec4 vertexColor;
// in vec3 fragPos;

void main()
{
	// vec3 toCameraDir = normalize(cameraPos - fragPos); 
	// float diff = max(dot(normal, toCameraDir), 0.0);
	//FragColor = vertexColor + vec4(0.1f, 0.1f, 0.1f, 1.f)* diff;
	//FragColor = vertexColor + vec4(0.1f, 0.1f, 0.1f, 0.f)* diff;
	FragColor = vec4(0.1f, 0.1f, 0.1f, 1.f);

}