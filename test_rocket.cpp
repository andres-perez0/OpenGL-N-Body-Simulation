#include "Vector2D.h"

// for rocket class
#include <GLFW/glfw3.h>
#include <GL/gl.h>

struct Rocket{
    Vector2D position;

    Rocket(float x=0.0f, float y=0.0f) : position(Vector2D(x,y)) {}

    void rendor() const {
    }
};

int main() {
    Vector2D *vec1 = new Vector2D(2,3);
    
    std::cout << vec1 << std::endl; 
    std::cout << &vec1 << std::endl;

    vec1->info();
    Vector2D vec2 = vec1->normalize();
    vec2.info();

    std::cout << vec1->magnitude() << std::endl;
    delete vec1;
    vec1 = nullptr; // <--- Best practice: set the pointer to nullptr

    vec2.info();
    vec1->info();
    return 0;
}