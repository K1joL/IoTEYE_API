cmake_minimum_required (VERSION 3.8 FATAL_ERROR)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23 -Wall")

project (main VERSION 1.0)

include_directories("include/")
set(SRC_LIST src/IoTEYE_API.cpp)
set(HEAD_LIST include/IoTEYE_API.h)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 3b15fa82ea74739b574d705fea44959b58142eb8) # Replace with your desired git commit from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)

# add_compile_definitions(IoTeyeDEBUG)
add_executable(main main.cpp ${SRC_LIST})
target_link_libraries(main PRIVATE cpr::cpr)
