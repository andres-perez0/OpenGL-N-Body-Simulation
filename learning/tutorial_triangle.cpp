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

    float vertices[] = {
        0.0, 0.5, 0.0,   // top corner
        -0.5, -0.5, 0.0, // bottom left corner
        0.5, -0.5, 0.0   // bottom right corner
    };

    // loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT); // clears the screen at the start of each loop iteration

        // render the OpenGL here
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glDrawArrays(GL_TRIANGLES,0, 3);
        glDisableClientState(GL_VERTEX_ARRAY);

        // swap front and back buffers
        glfwSwapBuffers(window);
        // poll and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}