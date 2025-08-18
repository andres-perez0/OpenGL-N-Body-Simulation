#include "body.h"
 
body::body(glm::vec2 pos, float rad, glm::vec3 col) {  
    this->position = pos;
    this->radius = rad;
    this->color = col;
    res = 300;
    mass = EARTH_MASS;
    timeState = 1;
};
    
void body::render() const {
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

void body::update (float deltaTime) {
    prevPosition = position;
    velocity.x += acceleration.x * deltaTime;
    velocity.y += acceleration.y * deltaTime;

    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
    
    trails.push_back(position);
    timeState = 0;
}

void body::updateVerlet(float deltaTime) {
    currPosition = position;
    position.x = -prevPosition.x + (2*currPosition.x) + (acceleration.x*deltaTime*deltaTime);
    position.y = -prevPosition.y + (2*currPosition.y) + (acceleration.y*deltaTime*deltaTime);
    prevPosition = currPosition; 

    trails.push_back(position);
    if (trails.size() > 300) {
        trails.erase(trails.begin());
    }
}


void body::addForce (const glm::vec2 &force) {
    // std::cout << "force in the x direction " << force.x << std::endl;
    // std::cout << "force in the y direction " << force.y << std::endl;
    acceleration.x = force.x / mass;
    acceleration.y = force.y / mass;
}

void body::resetAcceleration() {
    acceleration.x = 0.0;
    acceleration.y = 0.0;
}

void body::setVelocity(float x, float y) {
    velocity.x = x;
    velocity.y = y;
}