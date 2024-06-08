#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "CubeMap.hpp"
#include "Model.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

int main() {
  /* Initialize the library */
  if (!glfwInit()) {
    std::cerr << "Failed to Initialize GLFW" << std::endl;
    return -1;
  }

  // Explicitly specifying OpenGL core versions else it will go in compatibility
  // mode
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Enabling Debug Context for OpenGL
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

  /* Create a windowed mode window and its OpenGL context */
  GLFWwindow *const window =
      glfwCreateWindow(Renderer::w_width - 1, Renderer::w_height - 1,
                       "ICTC Modeling", nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  // hide the cursor and capture it.
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  // Sets framebuffersize of the current window
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetWindowSize(window, Renderer::w_width, Renderer::w_height);

  // Gets mouse postion when mouse moves
  glfwSetCursorPosCallback(window, mouse_callback);

  // Gets scroll offset when scrolling mouse wheel
  glfwSetScrollCallback(window, scroll_callback);

  // For Vsync
  glfwSwapInterval(1);

  // Init only after creating a valid OpenGL context
  if (const auto err = glewInit(); err != GLEW_OK)
    /* Problem: glewInit failed, something is seriously wrong. */
    std::cerr << "Error: " << glewGetErrorString(err) << std::endl;

  std::cout << glGetString(GL_VERSION) << std::endl;

  // Checking if debug mode is activated
  int flags;
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
    // glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(GLDebugMessageCallback, nullptr);
  }

  // For blending, i.e. for textures with RGBA values
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // enabling depth_test sense depth using z-buffer
  glEnable(GL_DEPTH_TEST);

  constexpr float planeVertices[] = {// positions  //texture coordinates
                                     -1, 0, -1, 0, 1, 1,  0, -1, 1, 1,
                                     1,  0, 1,  1, 0, -1, 0, 1,  0, 0};

  constexpr unsigned int planeIndices[] = {0, 1, 2, 2, 3, 0};

  const VertexArray planeVA;
  const VertexBuffer planeVB(planeVertices, sizeof(planeVertices));
  const IndexBuffer planeIB(planeIndices,
                            sizeof(planeIndices) / sizeof(unsigned int));

  VertexBufferLayout planeLayout;
  // positions
  planeLayout.Push<float>(3);
  // texture coordinates
  planeLayout.Push<float>(2);

  planeVA.AddBuffer(planeVB, planeLayout);

  Shader planeShader("res/shaders/renderTexture.glsl");
  const auto planeModelMatrix =
      glm::scale(glm::translate(glm::mat4(1.f), glm::vec3(10.f, -1.75f, 0.f)),
                 glm::vec3(100.f));
  planeShader.Bind();
  planeShader.SetUniform("u_Model", planeModelMatrix);

  Texture planeTexture("res/images/groundTexture.png");
  planeShader.SetUniform("u_Texture", 0);

  constexpr float vertices[] = {
      // front
      -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0,
      // back
      -1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0};

  constexpr unsigned int indices[] = {// front
                                      0, 1, 2, 2, 3, 0,
                                      // right
                                      1, 5, 6, 6, 2, 1,
                                      // back
                                      7, 6, 5, 5, 4, 7,
                                      // left
                                      4, 0, 3, 3, 7, 4,
                                      // bottom
                                      4, 5, 1, 1, 0, 4,
                                      // top
                                      3, 2, 6, 6, 7, 3};

  const VertexArray va; // Light Source VAO

  // Vertex Buffer Object
  const VertexBuffer vb(vertices, sizeof(vertices));

  const IndexBuffer ib(indices, sizeof(indices) / sizeof(unsigned int));

  // Specifying Layout
  VertexBufferLayout layout;
  // For vertex position
  layout.Push<float>(3);
  va.AddBuffer(vb, layout);

  // Position of light source
  constexpr glm::vec3 light_pos(17.2f, 20.0f, 2.0f);

  Shader lampShader("res/shaders/lamp.glsl");
  const auto lampModelMatrix =
      glm::scale(glm::translate(glm::mat4(1.f), light_pos),
                 glm::vec3(0.1f)); // a smaller cube
  lampShader.Bind();
  lampShader.SetUniform("u_Model", lampModelMatrix);

  // model Loading
  Model home("res/objects/ICTC/ictc.fbx");
  Model room("res/objects/ICTC/ICTCRoom.fbx");
  Model clock("res/objects/ICTC/ICTCClock.fbx");
  Model writing("res/objects/ICTC/ICTCWrite.fbx");
  Model glass("res/objects/ICTC/ictc_glass.fbx");

  Model car("res/objects/car/Lamborghini_Aventador.fbx");

  Shader lightingShader("res/shaders/lightingMap.glsl");
  lightingShader.Bind();
  lightingShader.SetUniform("u_Light.position", light_pos);
  lightingShader.SetUniform("u_Light.ambient", glm::vec3(0.2f));
  lightingShader.SetUniform("u_Light.diffuse", glm::vec3(0.8f));
  lightingShader.SetUniform("u_Light.specular", glm::vec3(1.0f));
  lightingShader.SetUniform("u_Light.constant", .01f);
  lightingShader.SetUniform("u_Light.linear", 0.002f);
  lightingShader.SetUniform("u_Light.quadratic", 0.0013f);
  lightingShader.SetUniform("u_Material.shininess", 128.0f);

  constexpr auto lightingModelMatrix =
      glm::translate(glm::mat4(1.f), glm::vec3(0.f, -1.75f, 0.f));
  // lightingModelMatrix = glm::scale(lightingModelMatrix, glm::vec3(2.f));

  const auto carModelMatrix =
      glm::scale(glm::translate(glm::mat4(1.f), glm::vec3(7.f, -1.75f, 5.f)),
                 glm::vec3(0.01f));

  // Renderer::EnableCullFace();

  // NOTE: For SkyBox
  const CubeMap skyBox(std::vector<std::string>{
      "res/skybox/right.jpg", "res/skybox/left.jpg", "res/skybox/top.jpg",
      "res/skybox/bottom.jpg", "res/skybox/front.jpg", "res/skybox/back.jpg"});

  Shader skyBoxShader("res/shaders/cubeMap.glsl");
  skyBoxShader.Bind();
  skyBoxShader.SetUniform("u_CubeMap", 0);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // To keep the background at a certain color
    glClearColor(0.539f, 0.808f, 0.922f, 1.f);

    Renderer::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // projection transformation
    const auto &view = Renderer::camera.GetViewMatrix();
    lightingShader.Bind();
    lightingShader.SetUniform("u_Model", lightingModelMatrix);
    lightingShader.SetUniform("u_ViewPos", Renderer::camera.GetPosition());
    lightingShader.SetUniform("u_View", view);
    const auto projection = glm::perspective(
        glm::radians(Renderer::camera.GetFOV()),
        static_cast<float>(Renderer::w_width) / Renderer::w_height, 0.1f,
        100.f);
    lightingShader.SetUniform("u_Projection", projection);

    home.Draw(lightingShader);
    room.Draw(lightingShader);
    clock.Draw(lightingShader);
    writing.Draw(lightingShader);

    lightingShader.SetUniform("u_Model", carModelMatrix);
    car.Draw(lightingShader);

    // also draw the lamp object
    lampShader.Bind();
    lampShader.SetUniform("u_Projection", projection);
    lampShader.SetUniform("u_View", view);
    Renderer::Draw(va, ib);

    planeShader.Bind();
    planeShader.SetUniform("u_Projection", projection);
    planeShader.SetUniform("u_View", view);
    planeTexture.Bind();
    Renderer::Draw(planeVA, planeIB);

    // !NOTE: Draw skyBox at Last
    skyBoxShader.Bind();
    // remove translation from the view matrix
    skyBoxShader.SetUniform("u_View", glm::mat4(glm::mat3(view)));
    skyBoxShader.SetUniform("u_Projection", projection);
    skyBox.Draw(va, ib);

    lightingShader.Bind();
    lightingShader.SetUniform("u_Model", lightingModelMatrix);
    glass.Draw(lightingShader);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}
