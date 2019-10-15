cmake_minimum_required(VERSION 3.14)

project(dronePlot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(OpenGL_GL_PREFERENCE "LEGACY")
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLU REQUIRED)
find_package(Threads REQUIRED)


add_definitions (-D_GNU_SOURCE)
add_definitions (-D_REENTRANT)
add_definitions (-D_THREAD_SAFE)
add_definitions (-D_POSIX_PTHREAD_SEMANTICS)
add_definitions("-std=c++17 -Wall -Wextra -pthread")
#-Werror")

add_executable(dronePlot
    main.cpp
    Plot.cpp
	NNGInterface.cpp
)

target_link_libraries(dronePlot
    ${OPENGL_gl_LIBRARY}
    GLU
    glut
    glfw
    pthread
    nng
    json11
Threads::Threads
)
