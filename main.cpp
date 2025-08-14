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
#define MOON_MASS 7.346e+22 // * 10^22 kg
#define MOON_RADIUS 1737.4*1000; // km -> m
#define EARTH_MASS 5.9722e+24;// *10^24
#define EARTH_RADIUS 6378*1000; // km -> m 


// Function prototypes
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
float graviationForce(float m1, float m2, float r);
float accValue(float G_Force, float m);

struct body {
    std::vector<glm::vec2> trails;    

    glm::vec2 currPosition;
    glm::vec2 position;
    glm::vec2 prevPosition;

    glm::vec3 velocity;
    glm::vec3 acceleration;
    
    glm::vec3 color;
    
    float mass;
    float radius; 
    int res;
    bool timeState;
    
    body(glm::vec2 pos, float rad, glm::vec3 col) {  
        this->position = pos;
        this->radius = rad;
        this->color = col;
        res = 300;
        mass = EARTH_MASS;
        timeState = 1;
    };
        
    void render() const {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2d(position.x, position.y);
        for (int i = 0; i < this->res; i++) {
            float angle = 2.0f * 3.141592653589f * (static_cast<float>(i)/this->res);
            float x = position.x + cos(angle) * this->radius;  
            float y = position.y + sin(angle) * this->radius;  
            glVertex2d(x, y);
        }
        glEnd();

        glEnableClientState( GL_VERTEX_ARRAY );
        glVertexPointer(2, GL_FLOAT, 0, trails.data());
        glDrawArrays( GL_LINE_STRIP, 0, trails.size());
        glDisableClientState( GL_VERTEX_ARRAY );
    }

    void update (float deltaTime) {
        prevPosition = position;
        velocity.x += acceleration.x * deltaTime;
        velocity.y += acceleration.y * deltaTime;

        position.x += velocity.x * deltaTime;
        position.y += velocity.y * deltaTime;
        
        trails.push_back(position);
        timeState = 0;
    }

    void updateVerlet(float deltaTime) {
        currPosition = position;
        position.x = -prevPosition.x + (2*currPosition.x) + (acceleration.x*deltaTime*deltaTime);
        position.y = -prevPosition.y + (2*currPosition.y) + (acceleration.y*deltaTime*deltaTime);
        prevPosition = currPosition; 

        trails.push_back(position);
        if (trails.size() > 300) {
            trails.erase(trails.begin());
        }
    }


    void addForce (const glm::vec2 &force) {
        // std::cout << "force in the x direction " << force.x << std::endl;
        // std::cout << "force in the y direction " << force.y << std::endl;
        acceleration.x = force.x / mass;
        acceleration.y = force.y / mass;
    }

    void resetAcceleration() {
        acceleration.x = 0.0;
        acceleration.y = 0.0;
    }

    void setVelocity(float x, float y) {
        velocity.x = x;
        velocity.y = y;
    }
};

struct simulation {
    float deltaTime;
    glm::vec2 cameraOffset;
    float zoomLevel;
    std::vector<std::unique_ptr<body>> objects;

    simulation() {}

    float dy, dx, dis, gravForce;
    std::vector<float> direction;
    glm::vec2 partialForces;

    void initilaize() {
        objects.emplace_back(new body(glm::vec2 (3.0f * SCR_WIDTH/4.0f, SCR_HEIGHT/2.0f), 10.0f, glm::vec3(1.0f, 1.0f, 1.0f)));
        objects.emplace_back(new body(glm::vec2 (SCR_WIDTH/2.0f, SCR_HEIGHT/2.0f), 10.0f, glm::vec3(1.0f, 1.0f, 1.0f)));
        // objects.emplace_back(new body(glm::vec2 (SCR_WIDTH/4.0f, SCR_HEIGHT/2.0f), 10.0f, glm::vec3(1.0f, 1.0f, 1.0f)));
    }

    void update(float deltaTime) {
        // Resets the forces of all the objects
        for (auto& obj : objects) {
            obj->resetAcceleration();
        }

        // Calculates the forces of gravity between objects
        for (auto& obj1 : objects) {
            for (auto& obj2 : objects) {
                if (&obj1 == &obj2) {continue;}
                dy = obj2->position.y - obj1->position.y;
                dx = obj2->position.x - obj1->position.x;
                dis = std::sqrt(dy*dy + dx*dx);
                // std::cout << "dy | dx | dis " << dy << dx << dis << std::endl; // Correct numbers
                direction = {dx/dis, dy/dis};

                dis *= 100000; // Scaling Factor

                // std::cout << "cos(theta) | sin(theta) " << direction[0] << " " << direction [1] << std::endl; // Correct Numbers
                gravForce = G_CONSTANT * obj2->mass * obj1->mass / (dis* dis);
                
                partialForces.x = gravForce * direction[0];
                partialForces.y = gravForce * direction[1];
                
                obj1->addForce(partialForces);

                if (obj1->timeState) {
                    obj1->update(deltaTime);
                    // std::cout<<"changedTimeState"<<std::endl;
                } else {
                    obj1->updateVerlet(deltaTime);
                }
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (auto& obj: objects) {
            obj->render();
        }   
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

    simulation sim = simulation();
    sim.initilaize();
    sim.objects[0]->setVelocity(0, 1500);
    sim.objects[1]->setVelocity(0,-1500);

    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        sim.update(deltaTime);

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
