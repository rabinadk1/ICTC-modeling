#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // Explicitly specifying OpenGL core versions else it will go in compatibility mode
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Enabling Debug Context for OpenGL
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow *window = glfwCreateWindow(640, 640, "Hello World", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Sets framebuffersize of the current window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // For Vsync
    glfwSwapInterval(1);

    // Init only after creating a valid OpenGL context
    if (const uint err = glewInit() != GLEW_OK)
        /* Problem: glewInit failed, something is seriously wrong. */
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    // Checking if debug mode is activated
    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        // glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(GLDebugMessageCallback, nullptr);
        // glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE);
    }

    // const float positions[] = {
    //     // positions         // colors
    //     -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
    //     0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    //     0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    //     -0.5f, 0.5f, 0.5f, 0.25f, 0.f};

    const float positions[] = {
        // positions   // texture Coords
        -0.5f, -0.5f, 0.f, 0.f,
        0.5f, -0.5f, 1.f, 0.f,
        0.5f, 0.5f, 1.f, 1.f,
        -0.5f, 0.5f, 0.f, 1.f};

    const uint indices[] = {
        0, 1, 2,
        2, 3, 0};

    // Vertext Array Object
    VertexArray va;

    // Vertex Buffer Object
    VertexBuffer vb(positions, sizeof(positions));

    // Specifying Layout
    VertexBufferLayout layout;
    // For vertex position
    layout.Push<float>(2);
    //For texture
    layout.Push<float>(2);
    // For color
    // layout.Push<float>(3);
    va.AddBuffer(vb, layout);

    //Element Buffer Object or Index Buffer Object
    IndexBuffer ib(indices, 6);

    Shader shader("res/shaders/renderTexture.glsl");
    shader.Bind();

    Texture texture("res/images/chernoLogo.jpg");
    texture.Bind();
    // Set the uniform u_Texture to the one bound above
    shader.SetUniform("u_Texture", 0);

    /*
    Unbinded to bind another object Latero on
    TODO: Remove later on and don;t Unbind **anything**
    */
    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();

    // float r = 0.f, increment = 0.05f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        Renderer::Clear();

        shader.Bind();
        // shader.SetUniform("u_Color", r, 0.3f, 0.8f, 1.0f);

        Renderer::Draw(va, ib, shader);

        /*
        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;
        r += increment;
        */

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
