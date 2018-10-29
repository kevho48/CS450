#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

struct PointLight {
    vec3 pos;
    vec3 color;
};

uniform PointLight light;
uniform mat4       modelMat;
uniform mat4       viewMat;
uniform mat4       normalMat;
uniform mat4       projMat;

out vec3 view_Half;
out vec3 view_Normal;
out vec3 view_LightDir;
out vec3 world_Pos;

void main()
{
    view_Normal = vec3(normalMat * vec4(normal, 1.0));

    vec3 pos = vec3(viewMat * modelMat * vec4(position, 1.0));
    vec3 light_pos = vec3(viewMat * modelMat * vec4(light.pos, 1.0));

    view_LightDir = normalize(light_pos - pos);

    vec3 viewDir = normalize(-1.0 * pos);

    view_Half = normalize(normalize(view_LightDir) + normalize(viewDir));

    gl_Position = projMat * vec4(pos, 1.0);
}