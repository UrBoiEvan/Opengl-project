#ifndef sceneGraph_hpp
#define sceneGraph_hpp

#include "common_types.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>



struct meshNode : SceneNode {
    meshObject* obj;
    glm::vec4 color; // represents ambient(m)

    meshNode(meshObject* obj, glm::vec4 color);
    void visit(const glm::vec3& cameraPosition, const glm::mat4& view, const glm::mat4& proj, const glm::mat4& parentTransform);
};



#endif