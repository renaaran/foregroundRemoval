cmake_minimum_required(VERSION 3.0.0)
set (CMAKE_CXX_STANDARD 17)

project(ForegroundRemoval VERSION 0.1.0)

set(OpenCV_DIR "/usr/local/lib")
find_package(OpenCV REQUIRED)
find_package(OpenCV REQUIRED COMPONENTS core imgproc)

include_directories(/usr/local/include/opencv4)
link_directories(/usr/local/lib)

include(CTest)
enable_testing()

add_executable(ForegroundRemoval main.cpp image.hpp rgbimage.hpp
    rgbimage.cpp algorithm.hpp foregroundremove.hpp foregroundremove.cpp)
target_link_libraries(ForegroundRemoval stdc++fs ${OpenCV_LIBS})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)