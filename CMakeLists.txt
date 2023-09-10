cmake_minimum_required(VERSION 3.25)

project(basketBall)

set(CXX_STANDARD 17)


add_executable(ball main.cpp "./src/geometry.cpp")
target_include_directories(ball PUBLIC 
                           "./external/glm",
                           "./include"
                           "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/include"
                           )
target_link_libraries(ball PUBLIC "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/lib/x64/easyXa.lib")                       
