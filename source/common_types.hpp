#ifndef common_types_hpp
#define common_types_hpp

#include <glm/glm.hpp>
#include <vector>

// Only declare the base SceneNode structure here
struct SceneNode {
    glm::mat4 m;
    SceneNode* child;
    SceneNode* sibling;
    
    SceneNode();
    virtual ~SceneNode();
    
    virtual void visit(const glm::vec3& cameraPosition, const glm::mat4& view, const glm::mat4& proj, const glm::mat4& parentTransform = glm::mat4(1.0f));
    void translate(const glm::vec3& translation);
    void rotate(float angle, const glm::vec3& axis);
    void traverseDraw(const glm::vec3& cameraPosition, const glm::mat4& view, const glm::mat4& proj, const glm::mat4& parentTransform = glm::mat4(1.0f));
};

// Light structures
struct lightNode : SceneNode {
    glm::vec3 intensity;
    glm::vec3 diffuse;
    glm::vec3 specular;
    
    lightNode(glm::vec3 intensity, glm::vec3 diffuse, glm::vec3 specular);
};

struct Lights {
    glm::vec3 ambient;
    float attenuationCoefficient;
    std::vector<lightNode*> lights;
    
    Lights(glm::vec3 ambient, float attenuationCoefficient);
    void addLight(lightNode* light);
};

#endif