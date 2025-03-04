#include "meshObject.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#define TINYOBJLOADER_IMPLEMENTATION // Define this in only *one* .cc
#include "tiny_obj_loader.h"


// Start meshObject
// Initialize static member
int meshObject::nextId = 1;
std::map<int, meshObject*> meshObject::meshObjectMap;

// TODO: P1aTask3 - Modify the constructor of Object to accept the path to an obj file
meshObject::meshObject(const std::string& objFilePath, const Lights& lightsManager, const glm::vec3& diffusem, const glm::vec3& specularm, const float& shininess) 
: id(nextId++), lightsManager(lightsManager), diffusem(diffusem), specularm(specularm), shininess(shininess) { // Assign current value of nextId to id and increment it
    
    // Add this object to the map
    meshObjectMap[id] = this;

    // Initialize the model matrix
    modelMatrix = glm::mat4(1.0f);




    // Generate and bind VAO, VBO, and EBO
    // Generate and bind VAO, VBO, and EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    // TODO: P1aTask3 - Read from obj file instead.
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, objFilePath.c_str());

    if (!success) {
        std::cerr << "Failed to load OBJ file: " << err << std::endl;
        return;
    }

    // Store vertices and indices in vectors
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            int vertexIdx = index.vertex_index * 3;
            int normalIdx = index.normal_index * 3;

            // Get vertex positions
            float vx = attrib.vertices[vertexIdx];
            float vy = attrib.vertices[vertexIdx + 1];
            float vz = attrib.vertices[vertexIdx + 2];

            float nx = attrib.normals[normalIdx];
            float ny = attrib.normals[normalIdx + 1];
            float nz = attrib.normals[normalIdx + 2];

            // Add to vertices list
            vertices.push_back(vx);
            vertices.push_back(vy);
            vertices.push_back(vz);

            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz);

            indices.push_back(indices.size());  // Add sequential indices
        }
    }


    numIndices = (GLsizei)indices.size();
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // Vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0); // positions

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1); //normals

    glBindVertexArray(0);

    // Load and compile shaders
    shaderProgram = LoadShaders("meshVertexShader.glsl", "meshFragmentShader.glsl");
    pickingShaderProgram = LoadShaders("pickingVertexShader.glsl", "pickingFragmentShader.glsl");
    objColorLocation = glGetUniformLocation(shaderProgram, "objColor"); // Get location of uniform from shader
    attenuationCoefficientLocation = glGetUniformLocation(shaderProgram, "attenuationCoefficient");
    diffusemLocation = glGetUniformLocation(shaderProgram, "diffusem");
    finalMatrixLocation = glGetUniformLocation(shaderProgram, "finalMatrix");
    viewPositionLocation = glGetUniformLocation(shaderProgram, "viewPosition");
    specularm1Location = glGetUniformLocation(shaderProgram, "specularm");
    shininess1Location = glGetUniformLocation(shaderProgram, "shininess");

    // Light 1 uniforms
    position1Location = glGetUniformLocation(shaderProgram, "position1");
    intensityl1Location = glGetUniformLocation(shaderProgram, "intensityl1");
    diffusel1Location = glGetUniformLocation(shaderProgram, "diffusel1");
    specularl1Location = glGetUniformLocation(shaderProgram, "specularl1");

    // Light 2 uniform
    position2Location = glGetUniformLocation(shaderProgram, "position2");
    intensityl2Location = glGetUniformLocation(shaderProgram, "intensityl2");
    diffusel2Location = glGetUniformLocation(shaderProgram, "diffusel2");
    specularl2Location = glGetUniformLocation(shaderProgram, "specularl2");

}


meshObject::~meshObject() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    
    // Remove this object from the map
    meshObjectMap.erase(id);
}


void meshObject::changeColor(const glm::vec4& color) {
    // Set the color of the uniform specified in the fragment shader
    glUseProgram(shaderProgram);
    glUniform4fv(objColorLocation, 1, glm::value_ptr(color)); // Set the uniform
}
//TODO: P1bTask5 - Modify to accept lighiting info as arguement.
void meshObject::draw(const glm::vec3& cameraPosition, const glm::mat4& view, const glm::mat4& projection, const glm::vec4& color, const glm::mat4& parentTransform) {
    glUseProgram(shaderProgram);
    
    // Compute the MVP matrix
    glm::mat4 final_model = parentTransform * modelMatrix;
    glm::mat4 MVP = projection * view * final_model;
    GLuint matrixID = glGetUniformLocation(shaderProgram, "MVP");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, glm::value_ptr(MVP));

    

 
    // Object uniforms
    glUniform4fv(objColorLocation, 1, glm::value_ptr(color)); // Set the uniform
    glUniform1f(attenuationCoefficientLocation,  1.0f);
    glUniform3fv(diffusemLocation, 1, glm::value_ptr(diffusem));
    glUniformMatrix4fv(finalMatrixLocation, 1, GL_FALSE, glm::value_ptr(final_model));
    glUniform3fv(viewPositionLocation, 1, glm::value_ptr(cameraPosition));
    glUniform3fv(specularm1Location, 1, glm::value_ptr(specularm));
    glUniform1f(shininess1Location, shininess);



    // Abandon hopes for a scalable light structure thanks to uniforms being really hard
    //glm::vec3 light1Pos = glm::vec3(lightsManager.lights[0]->m[3]);
    //glm::vec3 light1Intensity = glm::vec3(lightsManager.lights[0]->intensity);
    //glm::vec3 light1Diffuse = glm::vec3(lightsManager.lights[0]->diffuse);
    //glm::vec3 light1Specular = glm::vec3(lightsManager.lights[0]->specular);



    //Individual lights
    glUniform3fv(position1Location, 1, glm::value_ptr(glm::vec3(lightsManager.lights[0]->m[3])));
    glUniform3fv(intensityl1Location, 1, glm::value_ptr(glm::vec3(lightsManager.lights[0]->intensity)));
    glUniform3fv(diffusel1Location, 1, glm::value_ptr(glm::vec3(lightsManager.lights[0]->diffuse)));
    glUniform3fv(specularl1Location, 1, glm::value_ptr(glm::vec3(lightsManager.lights[0]->specular)));

    // Light 2
    glUniform3fv(position2Location, 1, glm::value_ptr(glm::vec3(lightsManager.lights[1]->m[3])));
    glUniform3fv(intensityl2Location, 1, glm::value_ptr(glm::vec3(lightsManager.lights[1]->intensity)));
    glUniform3fv(diffusel2Location, 1, glm::value_ptr(glm::vec3(lightsManager.lights[1]->diffuse)));
    glUniform3fv(specularl2Location, 1, glm::value_ptr(glm::vec3(lightsManager.lights[1]->specular)));

    

    // Draw the object
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void meshObject::translate(const glm::vec3& translation) {
    // Apply translation to the model matrix
    modelMatrix = glm::translate(modelMatrix, translation);
}

void meshObject::rotate(float angle, const glm::vec3& axis) {
    // Apply rotation to the model matrix
    modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), axis);
}

void meshObject::drawPicking(const glm::mat4& view, const glm::mat4& projection) {
    glUseProgram(pickingShaderProgram); // Use the picking shader

    //TODO: P1bBonus2 - Send the value of the id variable to the shader as a uniform. Use glUniform1f

    glm::mat4 MVP = projection * view * modelMatrix;
    GLuint matrixID = glGetUniformLocation(pickingShaderProgram, "MVP");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, glm::value_ptr(MVP));

    // Draw the object
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glUseProgram(0); // Unbind the shader program
}



meshObject* meshObject::getMeshObjectById(int id) {
    // Look up the object by ID in the map
    auto it = meshObjectMap.find(id);
    if (it != meshObjectMap.end()) {
        return it->second;
    }
    return nullptr; // Return nullptr if ID not found
}
