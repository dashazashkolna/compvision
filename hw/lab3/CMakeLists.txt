cmake_minimum_required(VERSION 3.16)
project(lab3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Eigen3 REQUIRED)
find_package(Ceres REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(task1 task1.cpp)
add_executable(task2 task2.cpp)
add_executable(task3 task3.cpp)

target_link_libraries(task1 ${CERES_LIBRARIES})
target_link_libraries(task2 ${CERES_LIBRARIES})
target_link_libraries(task3 ${CERES_LIBRARIES} ${OpenCV_LIBS})