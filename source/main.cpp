// main.cpp
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "common_types.hpp"
#include "meshObject.hpp"
#include "gridObject.hpp"
#include "sceneGraph.hpp"
#include <vector>



// Function prototypes
int initWindow(void);
static void mouseCallback(GLFWwindow*, int, int, int);
int getPickedId();


// Global variables
glm::vec4 robotColor = glm::vec4(0.6757f, 0.8125f, 0.8f, 1.0f);
glm::vec4 robotColorBright = glm::vec4(0.8757f, 0.4125f, 0.87734f, 1.0f);


const GLuint windowWidth = 1024, windowHeight = 768;
GLFWwindow* window;

// The object that is currently selected for P1bTask4
int currSelected = 0; // 0 nothing selected, 1:Camera 'c', 2:BaseTranslate 't', 3:BaseRotate 'r'

float cameraAngleX = glm::radians(0.0f);  // X angle (up/down)
float cameraAngleY = glm::radians(0.0f);  // Y angle (left/right)
float radius = 10.0f;  



// draws a node, and all of its children and siblings recursively



int main() {
    // ATTN: REFER TO https://learnopengl.com/Getting-started/Creating-a-window
    // AND https://learnopengl.com/Getting-started/Hello-Window to familiarize yourself with the initialization of a window in OpenGL
    
    if (initWindow() != 0) return -1;

    // Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    //            Purpose: Used for perspective projection in the camera.
    //                     Convert from view space (camera) to clip space.
    //                     This is done later in the pipeline, after the view matrix is applied.
    glm::mat4 projectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    // Or, for Project 2, use an ortho camera :
    // gProjectionMatrix = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.0f, 100.0f); // In world coordinates

    
    //TODO: P1aTask3 - Create each robot arm piece by creating instances class meshObject.
    gridObject grid;

    // ----- Graphics Pipeline Step 1  Object coordinates ---------------------
    // Mesh Objects | stores local coordinates and model matrix   [OBJ Coords] • [Model Matrix] = [World Coords]
    glm::vec3 diffusem = glm::vec3(0.5,0.5,0.5);//glm::vec3(robotColor); // Diffuse material
    float attenuationCoefficient = 0.1;
    glm::vec3 ambient = glm::vec3(0.5,0.5,0.5);
    glm::vec3 specularm = glm::vec3(0.5,0.5,0.5);
    float shininess = 64.0f;

    // Lights structure to store every light
    // Set lights in the scene before creating the mesh objects
    Lights lightsManager = Lights(ambient, attenuationCoefficient);

    //                            Intensity, Diffuse, Specular
    lightNode light1 = lightNode(glm::vec3(0,0,0), glm::vec3(0.0,0.7,0.6), glm::vec3(0.5,0.5,0.5));
    light1.translate(glm::vec3(5,5,0));

    lightNode light2 = lightNode(glm::vec3(0,0,0), glm::vec3(0.6,0.7,0.0), glm::vec3(0.5,0.5,0.5));
    light2.translate(glm::vec3(-5,5,0));

    lightsManager.addLight(&light1);
    lightsManager.addLight(&light2);

    meshObject baseMesh = meshObject("MeshObjects/Base.obj", lightsManager, diffusem, specularm, shininess); // In meshobject constructor, 
    meshObject Arm1Mesh = meshObject("MeshObjects/Arm1.obj", lightsManager, diffusem, specularm, shininess); // model matrix is initialized to identity matrix
    meshObject Arm2Mesh = meshObject("MeshObjects/Arm2.obj", lightsManager, diffusem, specularm, shininess);
    meshObject JointMesh = meshObject("MeshObjects/Joint.obj", lightsManager, diffusem, specularm, shininess);

    // ----- Graphics Pipeline Step 2, defining World coordinates ---------------------
    // Translate the mesh object to world space by modifying the EXISTING model matrix [OBJ Coords] • [New Model Matrix] = [World Coords]
    // baseMesh.translate(glm::vec3(0,0,0));
    //Arm1Mesh.translate(glm::vec3(0,2,0));          // Model Matrix: T • R • S
    //JointMesh.translate(glm::vec3(-0.5,4.35,0));   // These change the T part of the model matrix internally
    //Arm2Mesh.translate(glm::vec3(-1.48,3.616,0));

    //Arm1Mesh.rotate(12, glm::vec3(0,0,1));   // These change the R part of the model matrix internally
    Arm2Mesh.rotate(-47, glm::vec3(0,0,1));



    // Define the Nodes with the object, and the draw function
    meshNode* Robot = new meshNode(&baseMesh, robotColor);
    meshNode* Arm1 = new meshNode(&Arm1Mesh, robotColor);
    meshNode* Arm2 = new meshNode(&Arm2Mesh, robotColor);
    meshNode* Joint = new meshNode(&JointMesh, robotColor);



    //lightNode* Light1 = new lightNode(glm::vec3(1.0,1.0,1.0), glm::vec3)

    // Set the hierarchy
    Robot->child = Arm1;
    Arm1->child = Joint;
    Joint->sibling = Arm2;

    // Set Node->m to position the robot arm pieces hierarchically
    Arm1->translate(glm::vec3(0,1.4425,0));
    Joint->translate(glm::vec3(0,3,0));
    Arm2->translate(glm::vec3(0,3,0));

    // TODO: P1bTask4 - Create a hierarchical structure and adjust the relative translations.

    //TODO: P1aTask2 - Create variables to keep track of camera angles.
    
    //TODO: P1bTask5 - Create variables to store lighting info.

    // Change values of selectedColor in fragment shader 



    

    
    // Variables for timing
    //deltaTime = Frame 2 time - Frame 1 Time

    double lastFrameTime = glfwGetTime(); // For frame-to-frame timing for camera
    double lastFPSTime = lastFrameTime;     // For fprint reporting
    int nbFrames = 0;

    
    do {
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastFrameTime);
        float translation_unit = 2.5f * deltaTime; // Used for smooth translation
        lastFrameTime = currentTime;  // update for next frame
    
        nbFrames++;
        if (currentTime - lastFPSTime >= 1.0) {
            printf("%f ms/frame\n", 1000.0 / double(nbFrames));
            printf("\ncurrSelected: %d\n", currSelected);
            nbFrames = 0;
            lastFPSTime = currentTime; // update reporting timer
        }

      
        // Set currSelected based on key pressed
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
            currSelected = 1;
        }
        else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
            currSelected = 2;
        }
        else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            currSelected = 3;
        }
        else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            currSelected = 4;
        }
        else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
            currSelected = 5;
        }
        else {
            currSelected = 0;
        } // nothing selected

        if (currSelected == 1){ // Camera selected
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                cameraAngleY -= 1.0f * deltaTime; // rotate left
            }
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                cameraAngleY += 1.0f * deltaTime; // rotate right
            }
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                cameraAngleX += 1.0f * deltaTime; // rotate up
            }
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                cameraAngleX -= 1.0f * deltaTime; // rotate down
            }
        }
        
        else if (currSelected == 2) { // Robot base Translate selected

            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                Robot->translate(glm::vec3(-translation_unit, 0, 0)); // move left
            }
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                Robot->translate(glm::vec3(translation_unit, 0, 0)); // move right
            }
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                Robot->translate(glm::vec3(0, 0, -translation_unit)); // move in
            }
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                Robot->translate(glm::vec3(0, 0, translation_unit)); // move out
            }
        }
        else if (currSelected == 3) { // Robot base Rotate about Y selected
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                Robot->rotate(-(40 * deltaTime), glm::vec3(0,1,0)); // rotate clockwise
            }
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                Robot->rotate(40 * deltaTime, glm::vec3(0,1,0)); // rotate counter
            }
        }
        
        else if (currSelected == 4) { // Arm1 Selected

            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                Arm1->rotate(-(40 * deltaTime), glm::vec3(0,0,1)); // rotate clockwise
            }
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                Arm1->rotate(40 * deltaTime, glm::vec3(0,0,1)); // rotate counter
            }
        }
        else if (currSelected == 5) { // Arm2 Selected
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                Arm2->rotate(-(40 * deltaTime), glm::vec3(0,0,1)); // rotate clockwise
            }
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                Arm2->rotate(40 * deltaTime, glm::vec3(0,0,1)); // rotate counter
            }
        }
        
        //
        glm::vec3 cameraPosition;
        cameraPosition.x = radius * cos(cameraAngleX) * sin(cameraAngleY);
        cameraPosition.y = radius * sin(cameraAngleX);
        cameraPosition.z = radius * cos(cameraAngleX) * cos(cameraAngleY);


        // [World Coords] • [View Matrix] = [View Coords]
        // View matrix is constructed as follows:
        glm::mat4 viewMatrix = glm::lookAt( //lookAt(eye, center, up)
            cameraPosition,   // Camera position
            glm::vec3(0.0f),  // Look at the origin
            glm::vec3(0, 1, 0)  // Head is looking up at the origin (set to 0,-1,0 to look upside-down)
        );
        
        // Draw picking for P1bBonus2
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)){
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            //TODO: P1bBonus2 draw all robort arm pieces using drawPicking function
            //base.drawPicking(viewMatrix, projectionMatrix); // collision detection with the mouse

            currSelected = getPickedId();
            
            std::cout << "Picked id: " << currSelected << std::endl;
            
            //TODO: P1bBonus2 - meshObject::getMeshObjectById can be used to get the picked object.
        }
        
        //TODO: P1bTask4 - On key press set currSelected to the id of the robot piece to select.
        
        //TODO: P1bTask4 - On key press, based on currSelected, make appropriate transformation.
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // DRAWING the SCENE
        
        //TODO: P1aTask3 - Draw all robot arm pieces.
        // ----- Graphics Pipeline Step 3, 4: apply view matrix and then projection matrix to world coords ---------------------
        grid.draw(viewMatrix, projectionMatrix);
        //baseMesh.draw(viewMatrix, projectionMatrix, robotColor);
        
        // Draw robot based on the hierarchy
        Robot->traverseDraw(cameraPosition, viewMatrix, projectionMatrix);
        
        //TODO: P1bTask4 - Draw the robot arm pieces using the hierachy instead. Call the draw function on the root node. The remeaining pieces will be drawn using recursive calls.

        //TODO: P1bTask5 - Pass the lighting info to the draw function.
        
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
    glfwWindowShouldClose(window) == 0);

    glfwTerminate();
    return 0;
}

// Initialize GLFW and create a window
int initWindow() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // FOR MAC

    window = glfwCreateWindow(windowWidth, windowHeight, "Harden,Evan(27541192)", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    
    // Set up inputs
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_FALSE);
    glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);
    glfwSetMouseButtonCallback(window, mouseCallback);
    
    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);
    
    return 0;
}

static void mouseCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        std::cout << "Left mouse button pressed" << std::endl;
    }
}

int getPickedId(){
    glFlush();
    // --- Wait until all the pending drawing commands are really done.
    // Ultra-mega-over slow !
    // There are usually a long time between glDrawElements() and
    // all the fragments completely rasterized.
    glFinish();
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    unsigned char data[4];

    //TODO: P1bBonus2 - Use glfwGetCursorPos to get the x and y value of the cursor.
    
    //TODO: P1bBonus2 - Use glfwGetFramebufferSize and glfwGetWindowSize to get the frame buffer size and window size. On high resolution displays, these sizes might be different.
    
    
    //TODO: P1bBonus2 - Use glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data) to read the pixel data.
    // Note that y position has to be flipped as glfwGetCursorPos gives the cursor position relative to top left of the screen. The read location must also be multiplied by (buffer size / windowSize) for some displays.
    
    int pickedId = data[0];
    return pickedId;
}
