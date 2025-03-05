#include "meshObject.hpp"
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

void SceneNode::visit(const glm::vec3& cameraPosition, const glm::mat4& view, const glm::mat4& proj, const glm::mat4& parentTransform) {
}

void SceneNode::translate(const glm::vec3& translation) {
    m = glm::translate(m, translation);
}
void SceneNode::rotate(float angle, const glm::vec3& axis) {
    m = glm::rotate(m, glm::radians(angle), axis);
}
void SceneNode::traverseDraw(const glm::vec3& cameraPosition, const glm::mat4& view, const glm::mat4& proj, const glm::mat4& parentTransform) {
    //std::cout << "traverseDraw" << std::endl;
    glm::mat4 globalTransform = parentTransform * m;
    visit(cameraPosition, view, proj, globalTransform);
    if (child) child->traverseDraw(cameraPosition, view, proj, globalTransform);
    if (sibling) sibling->traverseDraw(cameraPosition, view, proj, parentTransform);
}

meshNode::meshNode(meshObject* obj, glm::vec4 color) 
    : SceneNode(), obj(obj), color(color) {
}

void meshNode::visit(const glm::vec3& cameraPosition, const glm::mat4& view, const glm::mat4& proj, const glm::mat4& parentTransform) {
    // passes parentTransform(m) into model draw, which uses model matrix * transform(m)
    // m = parentTranform = identity at first funciton call from sceneNode() constructor
    obj->draw(cameraPosition, view, proj, color, parentTransform);
}

lightNode::lightNode(glm::vec3 intensity, glm::vec3 diffuse, glm::vec3 specular) 
    : intensity(intensity), diffuse(diffuse), specular(specular) {}

Lights::Lights(glm::vec3 ambient, float attenuationCoefficient) 
    : ambient(ambient), attenuationCoefficient(attenuationCoefficient) {}

void Lights::addLight(lightNode* light) {
    lights.push_back(light);
}