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
#include "camera.h"

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

    glfwSwapInterval(1);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    printf("Using OpenGL Version %s\n\n", glGetString(GL_VERSION));

    glViewport(0, 0, win_width, win_height);
    glEnable(GL_DEPTH_TEST);

    glfwSetCursorPosCallback(window, Clbk_Mouse);

    Camera globalCam;
    InitCameraYp(&globalCam, window, (float)(win_width / 2), (float)(win_height / 2),
        (vec3s) { 0.0f, 0.0f, 5.0f },   // position
        90.0f, 0.0f,                     // yaw, pitch
        (vec3s) { 0.0f, 1.0f, 0.0f },   // world up vec
        2.0f,                           // mvmt speed
        0.1f);                          // look sens

    //float vertices[] = {
    //    // positions                // colors                   // texcoords
    //    -0.5f,  -0.5f,  0.0f,       1.0f,   0.0f,   0.0f,       0.0f, 0.0f,     // btm l
    //    -0.5f,  0.5f,   0.0f,       0.0f,   1.0f,   0.0f,       0.0f, 1.0f,     // top l
    //    0.5f,   0.5f,   0.0f,       0.0f,   0.0f,   1.0f,       1.0f, 1.0f,     // top r
    //    0.5f,   -0.5f,  0.0f,       1.0f,   0.0f,   1.0f,       1.0f, 0.0f      // btm r
    //};

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 1.0f,   0.0f,   0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f,   1.0f,   0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f,   0.0f,   1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f,   0.0f,   0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f,   1.0f,   0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,   0.0f,   1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f, 1.0f,   0.0f,   0.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 0.0f,   1.0f,   0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 0.0f,   0.0f,   1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f,   0.0f,   0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f,   1.0f,   0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f,   0.0f,   1.0f, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, 1.0f,   0.0f,   0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f,   1.0f,   0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,   0.0f,   1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f,   0.0f,   0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f,   1.0f,   0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f,   0.0f,   1.0f, 1.0f, 0.0f,

         0.5f,  0.5f,  0.5f, 1.0f,   0.0f,   0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f,   1.0f,   0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f,   0.0f,   1.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f,   0.0f,   0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f,   1.0f,   0.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 0.0f,   0.0f,   1.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 1.0f,   0.0f,   0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f,   1.0f,   0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f,   0.0f,   1.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f,   0.0f,   0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f,   1.0f,   0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,   0.0f,   1.0f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f, 1.0f,   0.0f,   0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 0.0f,   1.0f,   0.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 0.0f,   0.0f,   1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f,   0.0f,   0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f,   1.0f,   0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f,   0.0f,   1.0f, 0.0f, 1.0f
    };

    unsigned int floats_per_vertex = 8;
    /*unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };*/

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
    /*unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

    glBindVertexArray(0);
    
    unsigned int shader_id = LoadShaders(SHADER_DIR "/basic.vert", SHADER_DIR "/basic.frag");
    glUseProgram(shader_id);

    unsigned int glslptr_model = glGetUniformLocation(shader_id, "model");
    unsigned int glslptr_view = glGetUniformLocation(shader_id, "view");
    unsigned int glslptr_projection = glGetUniformLocation(shader_id, "projection");
    
    while (!glfwWindowShouldClose(window))
    {
        CamUpdateDT(&globalCam);
        ProcessKBInput(window, &globalCam);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4s model = GLMS_MAT4_IDENTITY;
        mat4s view;
        mat4s projection;

        //CamSetYaw(&globalCam, 90 + glm_deg(glfwGetTime()));

        model = glms_rotate(model, glfwGetTime(), glms_vec3_normalize((vec3s) { 1.0f, 1.0f, 0.0f }));
        view = GetViewMatrix(&globalCam);
        projection = glms_perspective(glm_rad(60.0f), (float)win_width / (float)win_height, 0.1f, 100.0f);

        glUniformMatrix4fv(glslptr_model, 1, GL_FALSE, model.raw);
        glUniformMatrix4fv(glslptr_view, 1, GL_FALSE, view.raw);
        glUniformMatrix4fv(glslptr_projection, 1, GL_FALSE, projection.raw);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, woodfloor);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, trollface);
        glUniform1i(glGetUniformLocation(shader_id, "uTexWood"), 0);
        glUniform1i(glGetUniformLocation(shader_id, "uTexTroll"), 1);

        glBindVertexArray(vao);
        //glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(float) / floats_per_vertex);
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