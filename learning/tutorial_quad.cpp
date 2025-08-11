#include <Gl/glew.h>
#include <GLFW/glfw3.h>

int main(void) {
    GLFWwindow *window;

    // initialize the glfw
    if (!glfwInit()) {
        return -1;
    }

    // create a windowed mode and its opengl context
    window = glfwCreateWindow(640,480,"OpenGl Proj. Tut.", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    // make the window's context current
    glfwMakeContextCurrent(window);

    // Previous quad
    // float vertices[] = {
    //     -0.5, 0.5, 0.0,   // top left
    //     0.5, 0.5, 0.0, // top right
    //     0.5, -0.5, 0.0,   // bottom right
    //     -0.5, -0.5, 0.0,   // bottom left
    // };

    float vertices[] = {
        0, 300, 0.0,   // top left
        300, 300, 0.0, // top right
        300, 0, 0.0,   // bottom right
        0, 0, 0.0,   // bottom left
    };

    // Scales the viewport
    glViewport (0.0f, 0.0f, 640, 480);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();   
    glOrtho(0, 640, 0, 480, 0, 1); // Anything near or far isnt drawn
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT); // clears the screen at the start of each loop iteration

        // render the OpenGL here
        // normalizes the cordinate, so it might look ilongated depending on the screen size
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertices); // (three features, type float, stride - gap between vertices, address of the verticies)
        glDrawArrays(GL_QUADS,0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);

        // swap front and back buffers
        glfwSwapBuffers(window);
        // poll and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}