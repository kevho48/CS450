#version 330 core

// Just using position and coordinates
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texcoords;

out vec2 UV; 

void main()
{
	// Already passing in normalized device coordinates
	// So we don't have to do anything to them
	gl_Position = vec4(position, 1.0);	
	UV = texcoords;
}
