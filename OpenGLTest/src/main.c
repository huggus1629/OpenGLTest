#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/struct.h>

#include <stdio.h>
#include <math.h>

#include "mathdefs.h"
#include "input.h"
#include "shaders.h"
#include "stb_image.h"
#include "textures.h"


#define FULLSCREEN 0
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

int main(void)
{
    stbi_set_flip_vertically_on_load(1);

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
        // positions                // colors                   // texcoords
        -0.5f,  -0.5f,  0.0f,       1.0f,   0.0f,   0.0f,       0.0f, 0.0f,     // btm l
        -0.5f,  0.5f,   0.0f,       0.0f,   1.0f,   0.0f,       0.0f, 1.0f,     // top l
        0.5f,   0.5f,   0.0f,       0.0f,   0.0f,   1.0f,       1.0f, 1.0f,     // top r
        0.5f,   -0.5f,  0.0f,       1.0f,   0.0f,   1.0f,       1.0f, 0.0f      // btm r
    };
    unsigned int floats_per_vertex = 8;
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    mat4s trans;

    // create texture
    unsigned int woodfloor = CreateTexture(GL_TEXTURE_2D, "/woodfloor0/woodfloor0_Color.png", GL_RGB, GL_REPEAT);
    unsigned int trollface = CreateTexture(GL_TEXTURE_2D, "/trollface/trollface.png", GL_RGBA, GL_REPEAT);

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, floats_per_vertex * sizeof(float), (const void*)0);                     // positions
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, floats_per_vertex * sizeof(float), (const void*)(3 * sizeof(float)));   // colors
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, floats_per_vertex * sizeof(float), (const void*)(6 * sizeof(float)));   // texcoords
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // element buffer object (stores vertex draw order)
    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
    
    unsigned int shader_id = LoadShaders(SHADER_DIR "/basic.vert", SHADER_DIR "/basic.frag");
    glUseProgram(shader_id);

    unsigned int glslptr_transform = glGetUniformLocation(shader_id, "transform");

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        trans = GLMS_MAT4_IDENTITY;
        trans = glms_translate(trans, (vec3s) { 0.25f, -0.25f, 0.0f });
        trans = glms_rotate(trans, glfwGetTime(), (vec3s) { 0.0f, 0.0f, 1.0f });
        trans = glms_scale(trans, (vec3s) { 0.5f, 0.5f, 0.5f });
        glUniformMatrix4fv(glslptr_transform, 1, GL_FALSE, trans.raw);

        glUniform1i(glGetUniformLocation(shader_id, "uTexWood"), 0);
        glUniform1i(glGetUniformLocation(shader_id, "uTexTroll"), 1);

        glBindVertexArray(vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, woodfloor);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, trollface);
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