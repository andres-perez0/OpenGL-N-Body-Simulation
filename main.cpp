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

void drawCircle(float x_0, float y_0, float r, float res);

struct obj {
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
        this->res           = 150.0f;
    }

    void updateKinematics(float deltaTime) {
        this->velocity[0] += this->acceleration[0]  * deltaTime;
        this->velocity[1] += this->acceleration[1]  * deltaTime;
        this->position[0] += this->velocity[0]      * deltaTime;
        this->position[1] += this->velocity[1]      * deltaTime;
        
        
    }

    void checkBoundaries() {
        if (this->position[1] < 0 || this->position[1] > SCR_HEIGHT) {
            this->velocity[1] *= -0.9;
        }
        if (this->position[0] < 0 || this->position[0] > SCR_WIDTH) {
            this->velocity[0] *= -0.9;
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

    // Set a viewport so OpenGL knows how to map your normalized coordinates to window coordinates
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Setup an orthographic projection for 2D rendering
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, SCR_WIDTH, 0.0, SCR_HEIGHT, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float centerX = SCR_WIDTH/2.0f;
    float centerY = SCR_HEIGHT/2.0f;
    std::vector<float> position = {centerX, centerY};
    std::vector<float> velocity = {1.0f, 0.0f};
    std::vector<float> acceleration = {9.81f, -9.81f};
    float radius = 50.0f;
    int res =100;

    std::vector<obj> objects = {
        obj(position, velocity, acceleration, radius, 100),
        obj({400.0f, 300.0f}, velocity, acceleration, 30.0f, 100)
    };
    
    float lastTime=glfwGetTime();
    float currentTime, deltaTime;

    while (!glfwWindowShouldClose(window)) {
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto& obj : objects) {
            obj.checkBoundaries();
            obj.updateKinematics(deltaTime);
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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    
    // Crucial step: Set the window's context as the current one for all OpenGL operations
    glfwMakeContextCurrent(window);

    return window;
}       


// // Function prototypes
// void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// void processInput(GLFWwindow* window);

// // Window dimensions
// const unsigned int SCR_WIDTH=800;
// const unsigned int SCR_HEIGHT=600;

// int main() {
//     // initialize GLFW
//     glfwInit();
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//     glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
//     // glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_COMPAT_PROFILE);

// #ifdef __APPLE__
//     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
// #endif

//     // Create a GLFW window
//     GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
//     if (window == NULL) {
//         std::cout << "Failed to create GLFW window" << std::endl;
//         glfwTerminate();
//         return -1;
//     }
//     glfwMakeContextCurrent(window);
//     glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
//     // Initialize GLAD
//     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//         std::cout << "Failed to initialize GLAD" << std::endl;
//         return -1;
//     }

//     // Main render loop 
//     while (!glfwWindowShouldClose(window)) {
//         // process input
//         processInput(window);

//         // render 
//         glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//         glClear(GL_COLOR_BUFFER_BIT);

//         // swap buffers and poll for events
//         glfwSwapBuffers(window);
//         glfwPollEvents();
//     }

//     glfwTerminate();
//     return 0;
// }

// // process all input: query GLFW whether relevant keys are pressed/released
// void processInput(GLFWwindow *window) {
//     if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
//         glfwSetWindowShouldClose(window, true);
//     }
// }

// // GLFW callback function to resize the viewport when the window size changes 
// void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
//     glViewport(0, 0, width, height);
// }