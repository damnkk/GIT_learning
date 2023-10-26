cmake_minimum_required(VERSION 3.25)

project(basketBall)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_EXPORT_COMPILECOMMANDS ON)
message("------------------------")
set(sources ./src/geometry.cpp
            ./src/physicalSolver.cpp)
            message("${sources}")
add_executable(ball main.cpp ${sources})
target_include_directories(ball PUBLIC 
                           "./external/glm",
                           "./include"
                           "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/include"
                           )
target_link_libraries(ball PUBLIC "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/lib/x64/easyXa.lib")                       



# FILE(GLOB sources ./volumeSrc/*.cpp ./volumeSrc/*.c)
# message("--------")
# message("${sources}")
# add_executable(volumeRender ${sources})
# find_package(glew REQUIRED)
# find_package(zlib)
# find_package(openGL)
# add_subdirectory(external/glfw)
# message("${GLEW_INCLUDE_DIR}")
# target_include_directories(volumeRender PUBLIC 
#                            "./external/glm"
#                            "./external"
#                            "./external/glfw"
#                            "${GLEW_INCLUDE_DIR}"
#                            "./volumeSrc"
#                            )
# target_link_libraries(volumeRender PUBLIC 
#                         openGL32
#                         GLEW::GLEW
#                         glfw
#                         ZLIB::ZLIB)               

