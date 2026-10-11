#include <stdio.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "graphics/renderer.h"

int main(void)
{
    if (!glfwInit())
    {
        printf("Failed to initialize GLFW\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(
        1280,
        720,
        "topoOptim",
        NULL,
        NULL
    );

    if (window == NULL)
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    printf("OpenGL: %s\n", glGetString(GL_VERSION));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));

    if (!renderer_init())
    {
        printf("Failed to initialize renderer\n");

        glfwDestroyWindow(window);
        glfwTerminate();

        return 1;
    }

    while (!glfwWindowShouldClose(window))
    {
        renderer_render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    renderer_shutdown();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
