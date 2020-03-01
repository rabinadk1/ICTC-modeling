#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "Model.hpp"

// Position of light source
#define LIGHT_POS glm::vec3(1.2f, 1.0f, 5.0f)

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
    GLFWwindow *window = glfwCreateWindow(Renderer::w_width, Renderer::w_height, "Learning OpenGL", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // hide the cursor and capture it.
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Sets framebuffersize of the current window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Gets mouse postion when mouse moves
    glfwSetCursorPosCallback(window, mouse_callback);

    //Gets scroll offset when scrolling mouse wheel
    glfwSetScrollCallback(window, scroll_callback);

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
        // glDebugMessageControl(GL_DEBUG_SOURCE_API,
        //                       GL_DEBUG_TYPE_ERROR, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE);
    }

    //For blending, i.e. for textures with RGBA values
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //enabling depth_test sense depth using z-buffer
    glEnable(GL_DEPTH_TEST);

    const float vertices[] = {
        // positions          // normals        // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

    // Vertex Array Objects
    // VertexArray cubeVAO;  // Cube VAO
    VertexArray lightVAO; // Light Source VAO

    // Vertex Buffer Object
    VertexBuffer vb(vertices, sizeof(vertices));

    // Specifying Layout
    VertexBufferLayout layout;
    // For vertex position
    layout.Push<float>(3);
    // For normal direction
    layout.Push<float>(3);
    // For texture coordinates
    layout.Push<float>(2);

    lightVAO.AddBuffer(vb, layout);

    Shader lightingShader("res/shaders/lighting.glsl");
    Shader lampShader("res/shaders/lamp.glsl");

    // modal Loading
    Model modelObject("res/objects/ranipokhari/Ranipokhari2.obj");
    // Model modelObject("res/objects/nanosuit/nanosuit.obj");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // glClearColor(1, 0, 1, 1);
        Renderer::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // shader.SetUniform("u_Color", r, 0.3f, 0.8f, 1.0f);

        // projection transformation
        lightingShader.Bind();
        lightingShader.SetUniform("u_Light.position", LIGHT_POS);
        lightingShader.SetUniform("u_Light.direction", Renderer::camera.GetFront());
        lightingShader.SetUniform("u_Light.cutoff", glm::cos(glm::radians(25.0f)));
        lightingShader.SetUniform("u_Light.outerCutoff", glm::cos(glm::radians(35.0f)));

        lightingShader.SetUniform("u_ViewPos", Renderer::camera.GetPosition());
        lightingShader.SetUniform("u_View", Renderer::camera.GetViewMatrix());
        lightingShader.SetUniform("u_Light.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
        lightingShader.SetUniform("u_Light.diffuse", glm::vec3(0.8f, 0.8f, 1.0f));
        lightingShader.SetUniform("u_Light.specular", glm::vec3(1.0f, 1.0f, 0.8f));
        lightingShader.SetUniform("u_Light.constant", 1.0f);
        lightingShader.SetUniform("u_Light.linear", 0.09f);
        lightingShader.SetUniform("u_Light.quadratic", 0.032f);
        glm::mat4 projection = glm::perspective(glm::radians(Renderer::camera.GetFOV()),
                                                static_cast<float>(Renderer::w_width) / Renderer::w_height, 0.1f, 100.f);
        lightingShader.SetUniform("u_Projection", projection);

        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, -1.75f, 0.f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
        lightingShader.SetUniform("u_Model", modelMatrix);
        lightingShader.SetUniform("u_Material.shininess", .078125f * 128.0f);
        lightingShader.SetUniform("u_Material.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        lightingShader.SetUniform("u_Material.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        lightingShader.SetUniform("u_Material.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        modelObject.Draw(lightingShader);

        // also draw the lamp object
        lampShader.Bind();
        glm::mat4 model = glm::translate(glm::mat4(1.f), LIGHT_POS);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lampShader.SetUniform("u_Model", model);
        lampShader.SetUniform("u_Projection", projection);
        lampShader.SetUniform("u_View", Renderer::camera.GetViewMatrix());

        Renderer::Draw(lightVAO, 36);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
