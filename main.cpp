#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "Camera.hpp"

// Position of light source
#define LIGHT_POS glm::vec3(1.2f, 1.0f, 2.0f)

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

    /*
    const float positions[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};
        */

    // const float positions[] = {
    //     // positions   // texture Coords
    //     -0.5f, -0.5f, 0.f, 0.f,
    //     0.5f, -0.5f, 1.f, 0.f,
    //     0.5f, 0.5f, 1.f, 1.f,
    //     -0.5f, 0.5f, 0.f, 1.f};

    // const uint indices[] = {
    //     0, 1, 2,
    //     2, 3, 0};
    /*
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};
    */

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
    VertexArray cubeVAO;  // Cube VAO
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
    /*
    //For texture
    // layout.Push<float>(2);
    // For color
    // layout.Push<float>(3);
    */
    cubeVAO.AddBuffer(vb, layout);
    lightVAO.AddBuffer(vb, layout);

    //Element Buffer Object or Index Buffer Object
    // IndexBuffer ib(indices, 6);

    // glm::mat4 proj = glm::ortho(-2.f, 2.f, -1.5f, 1.5f);

    Shader lightingShader("res/shaders/lightingMap.glsl");
    Shader lampShader("res/shaders/lamp.glsl");

    Texture diffuseMap("res/images/container.png");
    Texture specularMap("res/images/container_specular.png");
    diffuseMap.Bind();
    specularMap.Bind(1);
    // Set the uniform u_Texture to the one bound above
    // lightingShader.SetUniform("u_Texture", 0);

    // !be sure to activate shader when setting uniforms/drawing objects
    lightingShader.Bind();
    lightingShader.SetUniform("u_Light.position", LIGHT_POS);
    lightingShader.SetUniform("u_Light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    lightingShader.SetUniform("u_Light.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    lightingShader.SetUniform("u_Light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    lightingShader.SetUniform("u_Material.diffuse", 0);
    lightingShader.SetUniform("u_Material.specular", 1);
    lightingShader.SetUniform("u_Material.shininess", 64.0f);

    // world transformation for cube
    lightingShader.SetUniform("u_Model", glm::mat4(1.f));

    lampShader.Bind();
    glm::mat4 model = glm::translate(glm::mat4(1.f), LIGHT_POS);
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    lampShader.SetUniform("u_Model", model);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        Renderer::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // shader.SetUniform("u_Color", r, 0.3f, 0.8f, 1.0f);

        lightingShader.Bind();
        // view transformation
        const glm::mat4 view = Renderer::camera.GetViewMatrix();
        lightingShader.SetUniform("u_View", view);

        lightingShader.SetUniform("u_ViewPos", Renderer::camera.GetPosition());

        // projection transformation
        const glm::mat4 projection = glm::perspective(glm::radians(Renderer::camera.GetFOV()),
                                                      static_cast<float>(Renderer::w_width) / Renderer::w_height, 0.1f, 100.f);
        lightingShader.SetUniform("u_Projection", projection);

        diffuseMap.Bind();
        specularMap.Bind(1);

        // render the cube
        Renderer::Draw(cubeVAO, 36);

        // also draw the lamp object
        lampShader.Bind();
        lampShader.SetUniform("u_Projection", projection);
        lampShader.SetUniform("u_View", view);

        Renderer::Draw(lightVAO, 36);

        /*
        // Renderer::Draw(va, ib);
        for (uint i = 0; i < 10; ++i)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.f), cubePositions[i]);
            model = glm::rotate(model, static_cast<float>(glfwGetTime()) + glm::radians(20.f * i), glm::vec3(1.f, 0.3f, 0.5f));
            shader.SetUniform("u_Model", model);
            Renderer::Draw(va, 36);
        }
        */

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
