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


