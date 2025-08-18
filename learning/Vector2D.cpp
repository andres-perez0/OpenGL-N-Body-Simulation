#include "Vector2D.h"

// Constructor definition
Vector2D::Vector2D(float in_x, float in_y) : x(in_x), y(in_y) {}

// Member function definitions
Vector2D Vector2D::operator*(float scalar) const {
    return Vector2D(x * scalar, y * scalar);
}

Vector2D Vector2D::operator/(float scalar) const {
    if (scalar == 0.0f) {
        std::cout << "your scalar is zero" << std::endl;
        return Vector2D();
    }
    return Vector2D(x / scalar, y / scalar);
}

float Vector2D::magnitude() const {
    return std::sqrt(x * x + y * y);
}

Vector2D Vector2D::normalize() const {
    float mag = magnitude();
    if (mag == 0) {
        std::cout << "your magnitude is zero" << std::endl;
        return Vector2D();
    }
    return Vector2D(x / mag, y / mag);
}

void Vector2D::info() const {
    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
}

// Non-member function definitions (free functions)
Vector2D operator+(const Vector2D &a, const Vector2D &b) {
    return Vector2D(a.x + b.x, a.y + b.y);
}

Vector2D operator-(const Vector2D &a, const Vector2D &b) {
    return Vector2D(a.x - b.x, a.y - b.y);
}