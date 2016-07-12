cmake_minimum_required(VERSION 3.3)
project(ImageProcessor)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


#GLFW
include_directories(${PROJECT_SOURCE_DIR}/GLFW/include)
link_directories(${PROJECT_SOURCE_DIR}/GLFW/lib)

#GLEW
include_directories(${PROJECT_SOURCE_DIR}/GLEW/include)
link_directories(${PROJECT_SOURCE_DIR}/GLEW/lib)

#other
include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/libs)

set(SOURCE_FILES
        main.cpp include/Effects)
add_executable(ImageProcessor ${SOURCE_FILES})

add_definitions(-DGLEW_STATIC)


#specify required libraries
set(libs
        glfw3
        glew32
        SOIL
        opengl32)

target_link_libraries(ImageProcessor ${libs})
