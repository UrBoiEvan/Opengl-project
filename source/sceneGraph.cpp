#include "sceneGraph.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

// Set m to identify matrix in constructor
SceneNode::SceneNode() : m(glm::mat4(1.0f)), child(nullptr), sibling(nullptr) {};

SceneNode::~SceneNode() {
    if (child) delete child;
    if (sibling) delete sibling;
}

void SceneNode::visit(const glm::mat4& view, const glm::mat4& proj, const glm::mat4& parentTransform) {
    std::cout << "SceneNode::visit" << std::endl;
}

void SceneNode::translate(const glm::vec3& translation) {
    m = glm::translate(m, translation);
}
void SceneNode::rotate(float angle, const glm::vec3& axis) {
    m = glm::rotate(m, glm::radians(angle), axis);
}
void SceneNode::traverseDraw(const glm::mat4& view, const glm::mat4& proj, const glm::mat4& parentTransform) {
    //std::cout << "traverseDraw" << std::endl;
    glm::mat4 globalTransform = parentTransform * m;
    visit(view, proj, globalTransform);
    if (child) child->traverseDraw(view, proj, globalTransform);
    if (sibling) sibling->traverseDraw(view, proj, parentTransform);
}

meshNode::meshNode(meshObject* obj, glm::vec4 color) 
    : SceneNode(), obj(obj), color(color) {}

void meshNode::visit(const glm::mat4& view, const glm::mat4& proj, const glm::mat4& parentTransform) {
    // passes parentTransform into model draw, which uses model matrix * transform
    // m = identity at first from sceneNode() constructor
    std::cout << "meshNode::visit" << std::endl;
    obj->draw(view, proj, color, parentTransform);
}


