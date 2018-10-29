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
    // transform the normal
    view_Normal = vec3(normalMat * vec4(normal, 1.0));


    // transform the vertex and light position to view space
    // view space -> camera at origin -> so model transform
    // then view transform
    vec3 pos = vec3(viewMat * modelMat * vec4(position, 1.0));
    vec3 light_pos = vec3(viewMat * modelMat * vec4(light.pos, 1.0));

    // get light direction as normalized vector from vertex 
    // position to the light position
    view_LightDir = normalize(light_pos - pos);

    // get the view direction as normalized negative
    // vertex position
    vec3 viewDir = normalize(-1.0*pos);

    // get the half vector as the normalized sum of
    // normalized light dir and view dir
    view_Half = normalize(normalize(view_LightDir) + normalize(viewDir));

    // transform the vertex position using projMat
    // we only need to apply the projection
    // matrix because its already in view coordinates
    gl_Position = projMat * vec4(pos, 1.0);
}