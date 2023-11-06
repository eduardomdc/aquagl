cmake_minimum_required(VERSION 2.6)
project (
    learn-opengl
    LANGUAGES CXX
)

set(OpenGL_GL_PREFERENCE LEGACY)
set(GLAD_INCLUDE_DIRS /usr/include/glad)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_subdirectory(glad)

include_directories(
    ${OPENGL_INCLUDE_DIRS} 
    ${GLFW_INCLUDE_DIRS}
    #${GLAD_INCLUDE_DIRS}
)

set(MY_SOURCE_FILES
    ../src/main.cpp
    ../src/image_loader.cpp
    ../src/camera.cpp
    ../src/app.cpp
    ../src/cave.cpp
    ../src/marchingcubes.cpp
    ../src/perlin.cpp
    ../src/simplex.cpp
)

link_directories(${GLAD_INCLUDE_DIRS})

add_executable(main ${MY_SOURCE_FILES})

target_link_libraries(main ${OPENGL_LIBRARIES} glfw glad)
