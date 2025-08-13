## Takeaways

- Member Functions vs Non-member / Free Functions
    - Member functions are decliared as a part of the struct or class
        - Member functions have an implicity first parameter, a pointer called `this`, which points to the object on which the function is called. 
    - A free function is declared outside of any struct or class. It is a standalone function that can take one or more objects as parameters
        - Because they only interact with the `public` interface of the class, it is less dependent on the internal implementation details of the class.
        - To allow for `scalar * vector` (communativity), the operator must be a free function.
        - Friend function exists! 

        *"Prefer non-member non-friend functions to memeber functions"*
        - Scott Meyers
            - The reasoning is that the less a function knows about the interal workings of a class, the better the encapsulation
            - If it requires access to the private state of the object. If it can be implemented using only the public interface, it's a strong candidate for being a non-member function. 
                - Maintainability: If a function had direct access to private members, any changes to those members would require rewritting the function. Encapsulation localizes changes.



    -`&` character is a critical part of C++
        - pass-by-reference solution (`const Vector2D&`)
```c++
    // Pass-by-reference to const (efficient and safe)
    Vector2D operator+(const Vector2D& a, const Vector2D& b) {
        return Vector2D(a.x + b.x, a.y + b.y);
    }
```
- It's more efficient because it avoid the overhead of copying the objects'
- `const` keyword is an additional and cruical part of the best practice. It means the function is not modifying object it is referencing.

- In this example, we see `&` in the return type of our compouds assignment operator(`+=`,`-=`).

```cpp
Vector2D& operator+=(const Vector2D& other) {
    this->x += other.x;
    this->y += other.y;
    return *this; // Returns a reference to the modified object
}
```

- Here the return type means the function is returning a reference to an object, not a copy of it.
    - Specifically, `return *this;` returns a reference to current object (`*this`), which has just been modified. 

    - This allows for chaining of operations like `(vec1 += vec2) += vec3`.


- Key Points for Source Files
    - Includes the header
    - Scope Resolution Operator (::): For each member function, you use the scope resolution operator to tell the compiler that the function belongs to the class
    - Function Bodies

- OpenGL uses the right handed system 
```terminal
    Y
    |
    |----> X
    / 
   /
  z
```
    - Y: + up         - down
    - X: + right      - left
    - Z: + towards    - alway

- Reminder: Range-Based Loops in C
    - `for (auto& element : container)` - simplifies iterating over collections of elements
        - `auto` means the compiler automatically determines the data type of the `element`
        - `&` means that you're addressing by reference the actual item element in the container, not a copy
- `cmath` vs `math`
    - `cmath` is just the cpp friendly version of `math` that places all the math functions into `namespace` requiring you to used `std::` before identify any math functionality like `std::sqrt()`
     

- `glfwSwapInterval(1)` matches the opengl to your frame rate, for me that's 60Hz, or 60fps, so intuititively, I thought I should divided the acceleration constant by 60 to properly get realistic simulations. However, this is really up to the system and can lead to problems if your system is not at the framerate.
    - `deltaTime` : concept
        - Keep track of the last frame's time
        - Inside the loop, calculate detlaTime
        - Update all the phyics using deltaTime

```c++
int main() {
    glfwSwapInterval(1);
    ...
    double lastFrameTime = glfwGetTime();

    while(!glfwWindowShouldClose(window)) {
        double currentFrameTime = glfwGetTime();
        double deltaTime = currentFrameTime - lastFrameTime;
        lastFrametime=currentFrameTime;
        ... 

    }

}
```

- `glViewport(0,0,SCR_WIDTH, SCR_HEIGHT);`
    - sets the bottom left corner to be 0,0
    - sets the window to be render to be SCR_WIDTH and HEGHT
    - sets up a mapping from noramlize device coordinates to widnow coodinates. 
        - Sets the normal -1 to 1 to the pixel dimensions of your window
- Orthographic Project Matrix Setup
    - The following is a common set up for a 2D rendering because it projects objects onto the screen without any perspective distortion (objects don't appear smaller the further away they are)
```c++
// Setup an orthographic projection for 2D rendering
    glMatrixMode(GL_PROJECTION);    // This tells OpenGl, we'll be modifying the projection matrix stack; aka transforming your 3D into a 2D space of the viewport
    glLoadIdentity(); // Provides a clean state by changing the matrix into it's identity matrix
    glOrtho(0.0, SCR_WIDTH, 0.0, SCR_HEIGHT, 0, 1); 
    // Sets up the boundaries
    // First four parameters are pretty self explanatory
    // 0, 1 are the near and far clipping planes, they define the depth of the viewing box on the z-axis, so like Anything beyond 0 and 1 in the Z-Axis will not be rendered
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
```

- The difference uses of the `for(auto% element : container)` vs nested for loops for computation
    - In the program I originally try to use a range based loop to calculate the collisions with, but if you use a range based loop for this. There is no way to avoid checking 1 to 2 and 2 to 1. So this is why you have to use a nested for-loop with an offset. 
    - while calculating the forces for the object, we don't actually mind this, it's actually what we want since they experiences the same forces from gravity, so with the include of the `if (&obj == & obj2) {continue;}` to avoid checking it with itself, we can calculate the forces of gravity with this method. 