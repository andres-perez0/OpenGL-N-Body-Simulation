#include <Gl/glew.h>
#include <GLFW/glfw3.h>

#define SCR_HEIGHT 1000
#define SCR_WIDTH 1000

int main(void) {
    GLFWwindow *window;

    // initialize the glfw
    if (!glfwInit()) {
        return -1;
    }

    // create a windowed mode and its opengl context
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,"OpenGl Proj. Tut.", NULL, NULL);

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

    GLfloat vertices[] = {
        SCR_WIDTH / 2, SCR_HEIGHT / 2
    };

    // Scales the viewport
    glViewport (0.0f, 0.0f, SCR_WIDTH, SCR_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();   
    glOrtho(0, SCR_WIDTH, 0, SCR_HEIGHT, 0, 1); // Anything near or far isnt drawn
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT); // clears the screen at the start of each loop iteration

        // render the OpenGL here
        // normalizes the cordinate, so it might look ilongated depending on the screen size
        glEnable(GL_POINT_SMOOTH);
        glEnableClientState(GL_VERTEX_ARRAY);
        
        glPointSize(50);
        glVertexPointer(2, GL_FLOAT, 0, vertices); // (three features, type float, stride - gap between vertices, address of the verticies)
        glDrawArrays(GL_POINTS, 0, 1); // Start at the zero point and there is one count

        glDisable(GL_POINT_SMOOTH);
        glDisableClientState(GL_VERTEX_ARRAY);


        // swap front and back buffers
        glfwSwapBuffers(window);
        // poll and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}