cmake_minimum_required(VERSION 3.5.0)
project(ictc-modeling VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

add_compile_options(-Wall -Wextra -Wpedantic -Weffc++)


file(GLOB src_files CONFIGURE_DEPENDS "*.cpp")
add_executable(ictc-modeling ${src_files})

target_include_directories(ictc-modeling PRIVATE includes)
target_include_directories(ictc-modeling SYSTEM PRIVATE vendor_includes)

target_link_directories(ictc-modeling PRIVATE vendor_libs)
target_link_libraries(ictc-modeling PRIVATE GLEW glfw GL assimp)

target_compile_features(ictc-modeling PRIVATE cxx_auto_type)
target_compile_features(ictc-modeling PRIVATE cxx_nullptr)
target_compile_features(ictc-modeling PRIVATE cxx_range_for)
