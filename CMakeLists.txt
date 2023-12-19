# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.8 FATAL_ERROR)

project (main VERSION 1.0)

include_directories("Headers/")
set(SRC_LIST Sources/IoTEYE_API.cpp)
set(HEAD_LIST Headers/IoTEYE_API.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23 -Wall")

find_package(cpr CONFIG REQUIRED)

add_executable(main main.cpp ${SRC_LIST})
target_link_libraries(main PRIVATE cpr::cpr)
