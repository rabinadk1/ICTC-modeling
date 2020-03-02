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
#define LIGHT_POS glm::vec3(-4.2f, 5.0f, -2.0f)

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
    }

    //For blending, i.e. for textures with RGBA values
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //enabling depth_test sense depth using z-buffer
    glEnable(GL_DEPTH_TEST);

    const float vertices[] = {
        // positions
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,

        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,

        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f};

    const uint indices[] = {
        // front
        0, 1, 2,
        2, 3, 0,
        // right
        1, 5, 6,
        6, 2, 1,
        // back
        7, 6, 5,
        5, 4, 7,
        // left
        4, 0, 3,
        3, 7, 4,
        // bottom
        4, 5, 1,
        1, 0, 4,
        // top
        3, 2, 6,
        6, 7, 3};


    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f, 0.2f, 2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f)};

    // Vertex Array Objects
    // VertexArray cubeVAO;  // Cube VAO
    VertexArray lightVAO; // Light Source VAO

    // Vertex Buffer Object
    const VertexBuffer vb(vertices, sizeof(vertices));

    const IndexBuffer ib(indices, sizeof(indices) / sizeof(uint));

    // Specifying Layout
    VertexBufferLayout layout;
    // For vertex position
    layout.Push<float>(3);
    lightVAO.AddBuffer(vb, layout);

    // strictly use lighting.glsl for lightingShader
    Shader lightingShader("res/shaders/lighting.glsl");
    Shader lampShader("res/shaders/lamp.glsl");

    lightingShader.Bind();
    // directional light
    lightingShader.SetUniform("u_DirLight.direction", -0.2f, -1.0f, -0.3f);
    lightingShader.SetUniform("u_DirLight.ambient", 0.5f, 0.05f, 0.05f);
    lightingShader.SetUniform("u_DirLight.diffuse", 0.8f, 0.8f, 0.8f);
    lightingShader.SetUniform("u_DirLight.specular", 0.5f, 0.5f, 0.5f);

    glm::mat4 lightingModelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, -1.75f, 0.f));
    lightingModelMatrix = glm::scale(lightingModelMatrix, glm::vec3(0.2f));
    lightingShader.SetUniform("u_Model", lightingModelMatrix);
    lightingShader.SetUniform("u_Material.shininess", 128.0f);

    // model Loading
    Model glass("res/objects/ICTC/ictc_glass.fbx");
    Model home("res/objects/ICTC/ictc.fbx");
    Model room("res/objects/ICTC/ICTCRoom.fbx");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // To keep the background at a certain color
        glClearColor(0.52f, 0.81, 0.94, 1);

        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_BACK);
        // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

        Renderer::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // projection transformation
        const glm::mat4 lookAt = Renderer::camera.GetViewMatrix();
        lightingShader.Bind();

        // point light 1
        lightingShader.SetUniform("u_PointLight[0].position", pointLightPositions[0]);
        lightingShader.SetUniform("u_PointLight[0].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.SetUniform("u_PointLight[0].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.SetUniform("u_PointLight[0].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.SetUniform("u_PointLight[0].constant", 1.0f);
        lightingShader.SetUniform("u_PointLight[0].linear", 0.09f);
        lightingShader.SetUniform("u_PointLight[0].quadratic", 0.032f);

        // point light 2
        lightingShader.SetUniform("u_PointLight[1].position", pointLightPositions[1]);
        lightingShader.SetUniform("u_PointLight[1].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.SetUniform("u_PointLight[1].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.SetUniform("u_PointLight[1].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.SetUniform("u_PointLight[1].constant", 1.0f);
        lightingShader.SetUniform("u_PointLight[1].linear", 0.09f);
        lightingShader.SetUniform("u_PointLight[1].quadratic", 0.032f);

        lightingShader.SetUniform("u_ViewPos", Renderer::camera.GetPosition());
        lightingShader.SetUniform("u_View", Renderer::camera.GetViewMatrix());
        glm::mat4 projection = glm::perspective(glm::radians(Renderer::camera.GetFOV()),
                                                static_cast<float>(Renderer::w_width) / Renderer::w_height, 0.1f, 100.f);
        lightingShader.SetUniform("u_Projection", projection);

        glm::mat4 lightingModelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, -1.75f, 0.f));
        lightingModelMatrix = glm::scale(lightingModelMatrix, glm::vec3(0.2f));
        lightingShader.SetUniform("u_Model", lightingModelMatrix);
        home.Draw(lightingShader);
        room.Draw(lightingShader);
        glass.Draw(lightingShader);

        // also draw the lamp object
        lampShader.Bind();

        lampShader.SetUniform("u_Projection", projection);
        lampShader.SetUniform("u_View", Renderer::camera.GetViewMatrix());
        for (unsigned int i = 0; i < 2; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            lampShader.SetUniform("u_Model", model);
            Renderer::Draw(lightVAO, 36);
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
