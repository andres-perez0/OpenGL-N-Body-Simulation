#include "Vector2D.h"
#include <iostream>

// GLAD must be included before GLFW
// #include <glad/glad.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// From the video 
#include <cmath>
#include <vector>

GLFWwindow *StartGLFW();

// Window dimensions
const unsigned int SCR_WIDTH=1000;
const unsigned int SCR_HEIGHT=800;

// Function prototypes
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

struct obj {
    public: 
    float mass;
    float radius;
    float res;
    std::vector<float> position;
    std::vector<float> velocity;
    std::vector<float> acceleration; 

    obj (std::vector<float> pos, std::vector<float> vel, std::vector<float> acc, float r, float m) {
        this->position      = pos;
        this->velocity      = vel;
        this->acceleration  = acc;
        this->radius        = r;
        this->mass          = m;
        this->res           = 300.0f;
    }

    void updateKinematics(float deltaTime) {
        this->velocity[0] += this->acceleration[0]  * deltaTime;
        this->velocity[1] += this->acceleration[1]  * deltaTime;

        this->position[0] += 2* this->velocity[0]      * deltaTime;
        this->position[1] += 2* this->velocity[1]      * deltaTime;
    }

    void checkBoundaries() {
        if ((this->position[1] - this->radius) < 0 || (this->position[1] + this->radius) > SCR_HEIGHT) {
            this->velocity[1] *= -0.9;
        }
        if ((this->position[0] - this->radius) < 0 || (this->position[0] + this->radius) > SCR_WIDTH) {
            this->velocity[0] *= -0.9;
        }
    }

    void checkCollision(obj& obj2) {
        if (this->position[1] + this->radius <= obj2.position[1] + obj2.radius) {
            this->velocity[1] *= -0.9;
            obj2.velocity[1] *= -0.9;
        }
    }

    void drawCircle() {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2d(this->position[0],this->position[1]);

        for (int i = 0; i < this->res; i++) {
            float angle = 2.0f * 3.141592653589 * (static_cast<float>(i)/this->res);
            float x = this->position[0] + cos(angle) * this->radius;
            float y = this->position[1] + sin(angle) * this->radius;
            glVertex2d(x,y);
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
    float centerY = SCR_HEIGHT/4.0f;
    std::vector<float> position = {centerX, centerY};
    std::vector<float> velocity = {1.0f, 0.0f};
    std::vector<float> acceleration = {0, -9.81f};
    float radius = 50.0f;
    int res =100;

    std::vector<obj> objects = {
        obj({centerX, 450.0f}, {0.0f, 0.0f}, acceleration, 30.0f, 20),
        obj({centerX, 200.0f}, {0.0f, 0.0f}, acceleration, 40.0f, 50)
    };
    
    float lastTime=glfwGetTime();
    float currentTime, deltaTime;
    int i, j;

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Update the kinematics first and then check boundaries
        for (auto& obj : objects) {
            obj.updateKinematics(deltaTime);
            obj.checkBoundaries();
        }

        // Check for position between all unique pairs 
        for (i = 0; i < objects.size(); i++) {
            for (j = i+1; j < objects.size(); j++) {
                objects[i].checkCollision(objects[j]);
            }
        }

        // Draw after everything is calculated
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto& obj : objects) {
            obj.drawCircle();
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

