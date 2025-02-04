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

    // create texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // set options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image
    int tex_w, tex_h, tex_channels;
    unsigned char* tex_data = stbi_load(TEXTURE_DIR "/woodfloor0/woodfloor0_Color.png", &tex_w, &tex_h, &tex_channels, 0);
    if (tex_data)
    {
        // apply image to texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_w, tex_h, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        printf("Failed to load texture\n");

    stbi_image_free(tex_data);
    glBindTexture(GL_TEXTURE_2D, 0);  // unbind texture

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

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D, texture);
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