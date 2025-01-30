#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <math.h>

#include "input.h"
#include "shaders.h"
#include "mathdefs.h"

#define FULLSCREEN 1
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

#define testvs SHADER_DIR"/basic.vert"

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    const GLFWvidmode* vidmode = glfwGetVideoMode(primary);
    const int win_width = FULLSCREEN ? vidmode->width : WINDOW_WIDTH;
    const int win_height = FULLSCREEN ? vidmode->height : WINDOW_HEIGHT;


    window = glfwCreateWindow(win_width, win_height, "Test Window", FULLSCREEN ? primary : NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        return -2;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    printf("Using OpenGL Version %s\n\n", glGetString(GL_VERSION));

    glViewport(0, 0, win_width, win_height);

    float vertices[] = {
        -1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0);
    glEnableVertexAttribArray(0);

    // element buffer object (stores vertex draw order)
    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
    
    unsigned int shader_id = LoadShaders(SHADER_DIR "/basic.vert", SHADER_DIR "/basic.frag");
    glUseProgram(shader_id);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float time = (float)glfwGetTime();
        float r = sinf(time) / 2 + 0.5f;
        float g = sinf(time + (float)(2 * PI / 3)) / 2 + 0.5f;
        float b = sinf(time + (float)(4 * PI / 3)) / 2 + 0.5f;

        int glslptr_timedColor = glGetUniformLocation(shader_id, "timedColor");
        glUniform3f(glslptr_timedColor, r, g, b);

        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glDeleteProgram(shader_id);
    glfwTerminate();

    fputs("Press Enter to quit...", stdout);
    fgetc(stdin);

    return 0;
}