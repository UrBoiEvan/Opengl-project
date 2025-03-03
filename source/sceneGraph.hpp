#ifndef sceneGraph_hpp
#define sceneGraph_hpp

#include "meshObject.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

// Create object hierarchy
struct SceneNode {
    // Data
    glm::mat4 m; // starting matrix 
    virtual void visit(const glm::mat4& view, const glm::mat4& proj, const glm::mat4& parentTransform = glm::mat4(1.0f));
    SceneNode* child;
    SceneNode* sibling;

    // Constructor
    SceneNode();
    ~SceneNode();

    // Methods
    void translate(const glm::vec3& translation);
    void rotate(float angle, const glm::vec3& axis);
    void traverseDraw(const glm::mat4& view, const glm::mat4& proj, const glm::mat4& parentTransform  = glm::mat4(1.0f));
};

struct meshNode : SceneNode {
    meshObject* obj;
    glm::vec4 color;
    meshNode(meshObject* obj, glm::vec4 color);
    void visit(const glm::mat4& view, const glm::mat4& proj, const glm::mat4& parentTransform);
};

#endif