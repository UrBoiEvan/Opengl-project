#ifndef meshObject_hpp
#define meshObject_hpp

#include "common_types.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <common/shader.hpp>
#include <map>
#include <string>



class meshObject {
public:
    meshObject(const std::string& objFilePath, const Lights& lightsPointer, const glm::vec3& diffusem, const glm::vec3& specularm, const float& shininess);
    ~meshObject();

    void draw(const glm::vec3& cameraPosition, const glm::mat4& view, const glm::mat4& projection, const glm::vec4& color, const glm::mat4& parentTransform = glm::mat4(1.0f));
    void drawPicking(const glm::mat4& view, const glm::mat4& projection);
    void translate(const glm::vec3& translation); // Translate the object
    void rotate(float angle, const glm::vec3& axis); // Rotate the object

    int getId() const { return id; } // Getter for the ID
    
    static meshObject* getMeshObjectById(int id); // Retrieve object by ID

    glm::mat4 getModelMatrix() const { return modelMatrix; } // Getter for the model matrix
    GLuint getshaderProgram() const { return shaderProgram; } // Getter for the shader program
    void changeColor(const glm::vec4& color); // Set the color of the object
    
    // TODO: P1bTask4 - Create a list of children.

private:
// VAO - HOW the vertex data (positions, normals, texture coordinates) are laid out
// VBO - WHERE the vertex data is stored
// EBO - HOW the vertices are connected (stores the indices of the vertices to tell opengl which order to draw them)
    GLuint VAO, VBO, EBO;
    GLuint shaderProgram; // used under normal cricumstances
    GLuint pickingShaderProgram; // used when the object is "picked", different shaders for different purposes
    GLint objColorLocation; // Get location of uniform
    GLint attenuationCoefficientLocation;
    GLint diffuselLocation;
    GLint diffusemLocation;
    GLint finalMatrixLocation;
    GLint viewPositionLocation;
    GLint specularm1Location;
    GLint shininess1Location;

    // Light 1 uniforms
    GLint position1Location;
    GLint intensityl1Location;
    GLint diffusel1Location;
    GLint specularl1Location;

    // Light 2 uniforms
    GLint position2Location;
    GLint intensityl2Location;
    GLint diffusel2Location;
    GLint specularl2Location;



    glm::mat4 modelMatrix; // [ M = T • R • S ] Model matrix for the object
    glm::vec4 color;
    glm::vec3 diffusem;
    glm::vec3 specularm;
    float shininess;

    const Lights& lightsManager;

    static int nextId; // Static counter for unique IDs
    int id;            // ID for this specific object
    
    GLsizei numIndices;
    
    static std::map<int, meshObject*> meshObjectMap; // Static map of ID to Object
};

#endif
