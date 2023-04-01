#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 light_pos;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 Normal;
layout(location = 3) out vec4 LightPosition;
layout(location = 4) out vec3 Position;

void main() 
{
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);

    Position = (ubo.view * ubo.model * vec4(inPosition, 1.0)).xyz;

    fragColor = vec3(1, 0, 0);
    
    LightPosition = ubo.view * vec4(ubo.light_pos, 0.0f);

    mat3 normal_matrix = mat3(vec3((ubo.view * ubo.model)[0]), vec3((ubo.view * ubo.model)[1]), vec3((ubo.view * ubo.model)[2]));
    Normal = normalize( normal_matrix * inNormal);

    fragTexCoord = inTexCoord;
}
