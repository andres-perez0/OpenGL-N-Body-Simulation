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
const unsigned int SCR_WIDTH  = 1200;
const unsigned int SCR_HEIGHT = 1200;

#define G_CONSTANT 6.674e-11
#define E_M_DIST 384400*1000
// Function prototypes
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
float graviationForce(float m1, float m2, float r);
float accValue(float G_Force, float m);

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

    // void updateKinematics(float deltaTime, float acc) {
    //     this->velocity[0] += acc * deltaTime;
    //     this->velocity[1] += acc * deltaTime;

    //     this->position[0] += this->velocity[0] * deltaTime;
    //     this->position[1] += this->velocity[1] * deltaTime;
    // }

    void accelerate(float x, float y) {
        this->velocity[0] += x;
        this->velocity[1] += y;
    }

    void updatePos() {
        this->position[0] += this->velocity[0];
        this->position[1] += this->velocity[1];
    }

    void checkBoundaries() {
        float reverseVelocity=-0.9;
        if ((this->position[1] - this->radius) < 0 ) {
            this->velocity[1] *= reverseVelocity;
            this->position[1] = this->radius;
        }

        if ((this->position[1] + this->radius) > SCR_HEIGHT) {
            this->velocity[1] *= reverseVelocity;
            this->position[1] = SCR_HEIGHT - this->radius;
        }

        if ((this->position[0] - this->radius) < 0 ) {
            this->velocity[0] *= reverseVelocity;
            this->position[0] = this->radius;
        }

        if ((this->position[0] + this->radius) > SCR_WIDTH) {
            this->velocity[0] *= reverseVelocity;
            this->position[0] = SCR_WIDTH - this->radius;
        }
    }

    float centerFromDistance(float x_1, float y_1, float x_2, float y_2) {
        return std::sqrt((x_2-x_1)*(x_2-x_1) + (y_2-y_1)*(y_2-y_1));
    }

    void checkCollision(obj& obj2) {
        float minCollisionDistance = this->radius + obj2.radius;
        float currentCollisionDistance = centerFromDistance(this->position[0], this->position[1], obj2.position[0], obj2.position[1]);

        if (currentCollisionDistance <= minCollisionDistance) {
            this->velocity[1] *= -0.9;
            obj2.velocity[1] *= -0.9;

            this->velocity[0] *= -0.9;
            obj2.velocity[0] *= -0.9;
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
    float centerY = SCR_HEIGHT/2.0f;

    std::vector<float> acceleration = {0.0f, 0.0f};
    float radius = 50.0f;

    float moonRadius =  1737.4*1000; // km -> m
    float moonMass = 7.346e+22; // * 10^22 kg
    float earthMass = 5.9722e+24;// *10^24
    float earthRadius = 6378*1000; // km -> m 

    std::vector<obj> objects = {
        obj({centerX-200, centerY}, {0.0, 1.0}, {0.0, 0.0}, 20, moonMass*1.5),
        obj({centerX+200, centerY}, {0.0 , -1.0}, {0.0, 0.0}, 20, moonMass*1.5),
        obj({centerX, centerY+200}, {1.0, 0.0}, {0.0, 0.0}, 20, moonMass*1.5),
        obj({centerX, centerY-200}, {-1.0 , 0.0}, {0.0, 0.0}, 20, moonMass*1.5),
        // obj({centerX, centerY+200}, {-1 , 0.0}, {0.0, 0.0}, 20, moonMass)
    };
    
    float lastTime=glfwGetTime();
    float currentTime, deltaTime;
    int i, j;

    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        for (auto& obj : objects) {
            for (auto& obj2 : objects) {
                if (&obj == &obj2) {continue;} // Ensure that they we calculating opposite to one another 

                // We find the differences between the objects
                float dy = obj2.position[1] - obj.position[1];
                float dx = obj2.position[0] - obj.position[0];
                float r = std::sqrt(dy*dy + dx*dx);

                // direction vector that holds the ratios for sin(theta) and cos(theta)
                std::vector<float> direction = {dx / r, dy / r};

                r *= 100000;

                float ForceGravity = (G_CONSTANT * obj.mass * obj2.mass) / (r*r); 
                // x component = Force from gravity times the cos(theta) equivalent ratio) divided by the mass
                acceleration[0] = (ForceGravity * direction[0]) / obj.mass;
                acceleration[1] = (ForceGravity * direction[1]) / obj.mass;

                obj.accelerate(acceleration[0], acceleration[1]);    
            }
            obj.updatePos();
        }

        // Check for position between all unique pairs 
        for (i = 0; i < objects.size(); i++) {
            for (j = i+1; j < objects.size(); j++) {
                objects[i].checkCollision(objects[j]);
            }
            objects[i].checkBoundaries();
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

float graviationForce(float m1, float m2, float r) {
    return (G_CONSTANT * m1 * m2) / (r*r);
}

float accValue(float G_Force, float m) {
    return G_Force/m;
}