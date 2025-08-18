#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <math.h>   //  for std::sqrt()
#include <iostream> //  for std::cout()
class Vector2D {
public:
    float x;
    float y;
    
    // Constructor Declaration
    Vector2D(float in_x=0.0f, float in_y = 0.0f);

    // Member function declarations
    Vector2D operator*(float scalar) const;
    Vector2D operator/(float scalar) const;
    float magnitude() const;
    Vector2D normalize() const;
    void info() const;
};

// Non-member function declarations (free functions)
Vector2D operator+(const Vector2D &a, const Vector2D &b);
Vector2D operator-(const Vector2D &a, const Vector2D &b);

#endif 