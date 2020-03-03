cmake_minimum_required(VERSION 3.0.0)
project(learnopengl VERSION 0.1.0)

include(CTest)
enable_testing()

if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic -Weffc++)
endif()

add_executable(learnopengl main.cpp IndexBuffer.cpp VertexBuffer.cpp VertexArray.cpp Shader.cpp Renderer.cpp Texture.cpp Camera.cpp Mesh.cpp Model.cpp CubeMap.cpp stb_image.cpp)
target_include_directories(learnopengl PRIVATE includes)
target_link_libraries(learnopengl PRIVATE GLEW glfw GL X11 dl assimp)
target_compile_features(learnopengl PRIVATE cxx_auto_type)
target_compile_features(learnopengl PRIVATE cxx_nullptr)
target_compile_features(learnopengl PRIVATE cxx_range_for)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
