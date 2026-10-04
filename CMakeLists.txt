cmake_minimum_required(VERSION 3.16)
project(assignment2 CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(FreeGLUT CONFIG QUIET)
if(FreeGLUT_FOUND)
  set(GLUT_LIB FreeGLUT::freeglut)
else()
  find_package(GLUT REQUIRED)
  set(GLUT_LIB GLUT::GLUT)
endif()

add_executable(part1 part1_windows_animation/main.cpp)
target_link_libraries(part1 OpenGL::GL ${GLUT_LIB})

add_executable(part2 part2_cube/main.cpp)
target_link_libraries(part2 OpenGL::GL ${GLUT_LIB} GLEW::GLEW)