#include "Vector2D.h"
#include <memory>
#include <iostream>

// GLAD must be included before GLFW
// #include <glad/glad.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // Required for glm::value_ptr

// From the video 
#include <cmath>
#include <vector>

GLFWwindow *StartGLFW();

// Window dimensions
const unsigned int SCR_WIDTH  = 1200;
const unsigned int SCR_HEIGHT = 1200;

#define G_CONSTANT 6.674e-11
#define E_M_DIST 384400*1000
// Function prototypes
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
float graviationForce(float m1, float m2, float r);
float accValue(float G_Force, float m);

struct body {
    glm::vec2 position;
    glm::vec3 color;
    float radius; 
    int res;
    
    body(glm::vec2 pos, float rad, glm::vec3 col) {  
        this->position = pos;
        this->radius = rad;
        this->color = col;
        res = 300;
    };
    
    void render() {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2d(position.x, position.y);
        for (int i = 0; i < this->res; i++) {
            float angle = 2.0f * 3.141592653589f * (static_cast<float>(i)/this->res);
            float x = position.x + cos(angle) * this->radius;  
            float y = position.y + sin(angle) * this->radius;  
            glVertex2d(x, y);
        }
        glEnd();
    }
};

int main() {
    GLFWwindow* window = StartGLFW();

    if (window == nullptr) {
        std::cout << "window failed to initialize" << std::endl;
        return -1;
    }

    int initialWidth, initialHeight;
    glfwGetFramebufferSize(window, &initialWidth, &initialHeight);
    framebuffer_size_callback(window, initialWidth, initialHeight);
    float centerX = SCR_WIDTH/2.0f;
    float centerY = SCR_HEIGHT/2.0f;

    float lastTime=glfwGetTime();
    float currentTime, deltaTime;
    int i, j;

    std::vector<std::unique_ptr<body>> objects;
    objects.emplace_back(new body(glm::vec2 (3.0f * SCR_WIDTH/4.0f, SCR_HEIGHT/2.0f), 10.0f, glm::vec3(1.0f, 1.0f, 1.0f)));
    objects.emplace_back(new body(glm::vec2 (SCR_WIDTH/2.0f, SCR_HEIGHT/2.0f), 10.0f, glm::vec3(1.0f, 1.0f, 1.0f)));
    objects.emplace_back(new body(glm::vec2 (SCR_WIDTH/4.0f, SCR_HEIGHT/2.0f), 10.0f, glm::vec3(1.0f, 1.0f, 1.0f)));

    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (auto& obj: objects) {
            obj->render();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

GLFWwindow* StartGLFW() {
    if (!glfwInit()) {
        std::cerr << "failed to initialize glfw, panic!" << std::endl;
        return nullptr;
    }

    // Request a compatibility profile to ensure glBegin/glEnd work
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Gravity Simulator", NULL, NULL);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    // Crucial step: Set the window's context as the current one for all OpenGL operations
    glfwMakeContextCurrent(window);  // A state machine that holds information regarding the rending objects
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return window;
}      

// GLFW callback function to resize the viewport when the window size changes 
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // When the window is resized, the size of the framebuffer also changes. You need to react to this change
    // You do this by reshaping the viewport and updating the project matrix
    glViewport(0, 0, width, height);

    // Setup an orthographic projection for 2D rendering
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, 0.0, height, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// process all input: query GLFW whether relevant keys are pressed/released
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

float graviationForce(float m1, float m2, float r) {
    return (G_CONSTANT * m1 * m2) / (r*r);
}

float accValue(float G_Force, float m) {
    return G_Force/m;
}
