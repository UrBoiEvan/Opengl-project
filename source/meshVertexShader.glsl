#version 330 core

// Input vertex attributes (from VBO)
layout(location = 0) in vec3 position; // Vertex position
layout(location = 1) in vec3 normal;
//TODO: P1bTask5 - Input Normals for lighting

out vec3 fragPos;
out vec3 fragNormal;

// Uniforms
uniform mat4 MVP; // Combined Model-View-Projection matrix
uniform mat4 finalMatrix;


void main() {
    // Transform the vertex position
    gl_Position = MVP * vec4(position, 1.0);

    fragPos = vec3(finalMatrix * vec4(position, 1.0));
    
    mat3 normalMatrix = mat3(transpose(inverse(finalMatrix)));
    fragNormal = normalMatrix * normal;

}
