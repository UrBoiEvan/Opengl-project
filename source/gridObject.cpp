#include "gridObject.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>


gridObject::gridObject() { // Assign current value of nextId to id and increment it
    
    // Initialize the model matrix
    modelMatrix = glm::mat4(1.0f);

    // Generate and bind VAO, VBO, and EBO
    // Generate and bind VAO, VBO, and EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // TODO: P1aTask1 - Modify vertices and indices to draw grid and axis lines. Note: multiple vertices at the origin will be needed due to different color of axis lines.
    
    // Tetrahedron vertices with positions and colors
    std::vector<GLfloat> vertices = {
        // Positions          // Colors
         0.0f, 0.0f,  0.0f,   1.0f, 0.0f, 0.0f,  // Origin R      //index 0
         5.0f, 0.0f,  0.0f,   1.0f, 0.0f, 0.0f,  // X-Axis                1
         0.0f, 0.0f,  0.0f,   0.0f, 1.0f, 0.0f,  // Origin G              2
         0.0f, 5.0f,  0.0f,   0.0f, 1.0f, 0.0f,  // Y-Axis                3
         0.0f, 0.0f,  0.0f,   0.0f, 0.0f, 1.0f,  // Origin B              4
         0.0f, 0.0f,  5.0f,   0.0f, 0.0f, 1.0f,  // Z-Axis                5

        
     
         // Draw xz plane grid                     Line Segment     Index
         5.0f, 0.0f,  5.0f,    1.0f, 1.0f, 1.0f, // Grid line 1  | 6
         -5.0f, 0.0f,  5.0f,   1.0f, 1.0f, 1.0f,        
         5.0f, 0.0f,  4.0f,    1.0f, 1.0f, 1.0f, // Grid line 2  | 8
        -5.0f, 0.0f,  4.0f,    1.0f, 1.0f, 1.0f,
        5.0f, 0.0f,  3.0f,     1.0f, 1.0f, 1.0f, // Grid line 3  | 10
        -5.0f, 0.0f,  3.0f,    1.0f, 1.0f, 1.0f,
        5.0f, 0.0f,  2.0f,     1.0f, 1.0f, 1.0f, // Grid line 4  | 12
        -5.0f, 0.0f,  2.0f,    1.0f, 1.0f, 1.0f,
        5.0f, 0.0f,  1.0f,     1.0f, 1.0f, 1.0f, // Grid line 5  | 14
        -5.0f, 0.0f,  1.0f,    1.0f, 1.0f, 1.0f,
        0.0f, 0.0f,  0.0f,     1.0f, 1.0f, 1.0f, // Grid line 6  | 16  Starts at x=0 to prevent z-fighting
        -5.0f, 0.0f,  0.0f,    1.0f, 1.0f, 1.0f,
        5.0f, 0.0f,  -1.0f,    1.0f, 1.0f, 1.0f, // Grid line 7  | 18
        -5.0f, 0.0f,  -1.0f,   1.0f, 1.0f, 1.0f,
        5.0f, 0.0f,  -2.0f,    1.0f, 1.0f, 1.0f, // Grid line 8  | 20
        -5.0f, 0.0f,  -2.0f,   1.0f, 1.0f, 1.0f,
        5.0f, 0.0f,  -3.0f,    1.0f, 1.0f, 1.0f, // Grid line 9  | 22
        -5.0f, 0.0f,  -3.0f,   1.0f, 1.0f, 1.0f,
        5.0f, 0.0f,  -4.0f,    1.0f, 1.0f, 1.0f, // Grid line 10 | 24
        -5.0f, 0.0f,  -4.0f,   1.0f, 1.0f, 1.0f,
        5.0f, 0.0f,  -5.0f,    1.0f, 1.0f, 1.0f, // Grid line 11 | 26
        -5.0f, 0.0f,  -5.0f,   1.0f, 1.0f, 1.0f,

        5.0f, 0.0f,  5.0f,    1.0f, 1.0f, 1.0f, // Grid line 12 | 28
        5.0f, 0.0f,  -5.0f,   1.0f, 1.0f, 1.0f,
        4.0f, 0.0f,  5.0f,    1.0f, 1.0f, 1.0f, // Grid line 13 | 30
        4.0f, 0.0f,  -5.0f,   1.0f, 1.0f, 1.0f,
        3.0f, 0.0f,  5.0f,    1.0f, 1.0f, 1.0f, // Grid line 14 | 32
        3.0f, 0.0f,  -5.0f,   1.0f, 1.0f, 1.0f,
        2.0f, 0.0f,  5.0f,    1.0f, 1.0f, 1.0f, // Grid line 15 | 34
        2.0f, 0.0f,  -5.0f,   1.0f, 1.0f, 1.0f,
        1.0f, 0.0f,  5.0f,    1.0f, 1.0f, 1.0f, // Grid line 16 | 36
        1.0f, 0.0f,  -5.0f,   1.0f, 1.0f, 1.0f,
        0.0f, 0.0f,  0.0f,    1.0f, 1.0f, 1.0f, // Grid line 17 | 38  Starts at z=0 to prevent z-fighting
        0.0f, 0.0f,  -5.0f,   1.0f, 1.0f, 1.0f,
        -1.0f, 0.0f,  5.0f,   1.0f, 1.0f, 1.0f, // Grid line 18 | 40
        -1.0f, 0.0f,  -5.0f,  1.0f, 1.0f, 1.0f,
        -2.0f, 0.0f,  5.0f,   1.0f, 1.0f, 1.0f, // Grid line 19 | 42
        -2.0f, 0.0f,  -5.0f,  1.0f, 1.0f, 1.0f,
        -3.0f, 0.0f,  5.0f,   1.0f, 1.0f, 1.0f, // Grid line 20 | 44
        -3.0f, 0.0f,  -5.0f,  1.0f, 1.0f, 1.0f,
        -4.0f, 0.0f,  5.0f,   1.0f, 1.0f, 1.0f, // Grid line 21 | 46
        -4.0f, 0.0f,  -5.0f,  1.0f, 1.0f, 1.0f,
        -5.0f, 0.0f,  5.0f,   1.0f, 1.0f, 1.0f, // Grid line 22 | 48    
        -5.0f, 0.0f,  -5.0f,  1.0f, 1.0f, 1.0f,

         
    };

    // Tetrahedron indices
    std::vector<GLuint> indices = {
        0,  1,
        2,  3,
        4,  5,
        6, 7, 8, 9, 10, 11 ,12 ,13 ,14 ,15 ,16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
        28, 29, 30, 31, 32, 33 ,34 ,35 ,36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49
    };
    
    numIndices = (GLsizei)indices.size();
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // Vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Load and compile shaders
    shaderProgram = LoadShaders("gridVertexShader.glsl", "gridFragmentShader.glsl");
}

gridObject::~gridObject() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    
}

void gridObject::draw(const glm::mat4& view, const glm::mat4& projection) {
    glUseProgram(shaderProgram);
    
    // Compute the MVP matrix and send to shader as a uniform
    glm::mat4 MVP = projection * view * modelMatrix;
    GLuint MVPLoc = glGetUniformLocation(shaderProgram, "MVP");
    glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(MVP));

    // Bind the VAO and draw the grid using glDrawElements(GL_LINES, numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, numIndices, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}
