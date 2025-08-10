#include "Vector2D.h"
// #include <iostream>
// #include <math.h>

// struct Vector2D {
//     // You can include these operator function within the struct! 
//     // Member functions vs free functions
//     // 
//     float x;
//     float y;

//     // Constructor
//     Vector2D(float in_x = 0.0f, float in_y = 0.0f) : x(in_x), y (in_y) {}

//     // Memeber Function for vector operations
//     Vector2D operator* (float scalar) const {
//         return Vector2D(x * scalar, y* scalar);
//     }

//     Vector2D operator/ (float scalar) const {
//         if (scalar == 0.0f) {
//             std::cout << "your scalar is zero" << std::endl;
//             return Vector2D();
//         }
//         return Vector2D(x / scalar, y / scalar);
//     }

//     Vector2D normalize() const {
//         float mag = magnitude();
//         if (mag == 0) {
//             std::cout << "your magnitude is zero" << std::endl;
//             return Vector2D();
//         }
//         return Vector2D(x / mag, y / mag);
//     }

//     float magnitude() const {
//         return std::sqrt(x*x + y*y);
//     }
    
//     void info() const {
//         std::cout << "x: " << x << std::endl;
//         std::cout << "y: " << y << std::endl;
//     }
// };

// // Non-member function for vector addition 
// // Vector addition/subtraction are operations between two vectors. Symmetric
// Vector2D operator+ (const Vector2D &a, const Vector2D &b) {
//     return Vector2D(a.x + b.x, a.y + b.y);
// }
// Vector2D operator- (const Vector2D &a, const Vector2D &b) {
//     return Vector2D(a.x - b.x, a.y - b.y);
// }

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