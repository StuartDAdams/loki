#version 330

#moka_compilation_flags
// #define NORMAL_MAP

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec2 aTexCoords;

out vec3 in_frag_pos;
out vec3 in_normal;
out vec2 in_texture_coord;

#ifdef NORMAL_MAP 
    out mat3 tbn_matrix;
#endif

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    in_frag_pos = vec3(model * vec4(aPos, 1.0));
    in_normal = mat3(transpose(inverse(model))) * aNormal;  
    in_texture_coord = aTexCoords;

    #ifdef NORMAL_MAP
        vec3 bitangent = cross(aNormal, aTangent);
        vec3 T = normalize(vec3(model * vec4(aTangent,  0.0)));
        vec3 B = normalize(vec3(model * vec4(bitangent, 0.0)));
        vec3 N = normalize(vec3(model * vec4(aNormal,   0.0)));
        tbn_matrix = mat3(T, B, N);
    #endif

    gl_Position = projection * view * vec4(in_frag_pos, 1.0);
}