#ifndef BODY_H
#define BODY_H

#define G_CONSTANT 6.674e-11
#define MOON_MASS 7.346e+22 // * 10^22 kg
#define MOON_RADIUS 1737.4*1000 // km -> m
#define EARTH_MASS 5.9722e+24 // *10^24
#define EARTH_RADIUS 6378*1000  // km -> m 

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // Required for glm::value_ptr'

#include <cmath>
#include <vector>
#include <memory>
#include <iostream>

class body {
public:
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

    // Contructor Declaration
    body(glm::vec2 pos, float rad, glm::vec3 col);

    // Member Function delarations
    void render() const;
    void update(float deltaTime);
    void updateVerlet(float deltaTime);
    void addForce (const glm::vec2 &force);
    void resetAcceleration();
    void setVelocity(float x, float y);
};

#endif