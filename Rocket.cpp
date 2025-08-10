/*
Task: Define the Rocket class and render a static representation of it and the ground.
Instructions:
Create Rocket.h and Rocket.cpp.
Define a Rocket class with:
A Vector2D position; member.
A constructor to initialize its starting position (e.g., top center of the screen).
A void render() const; method.
In Rocket::render(), use OpenGL primitives (e.g., glBegin(GL_QUADS) or glBegin(GL_TRIANGLES)) to draw a simple rectangle or triangle for the rocket. Use glColor3f to set its color.
In main.cpp, create a Rocket object.
Inside the main game loop, call rocket.render().
Draw a horizontal line at the bottom of the screen to represent the ground/landing pad using glBegin(GL_LINES).
*/

