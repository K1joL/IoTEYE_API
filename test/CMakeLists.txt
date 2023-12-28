cmake_minimum_required(VERSION 3.14)

project(ioteyeapiTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(ioteyeapi REQUIRED)
  enable_testing()
endif()

# ---- Tests ----

add_executable(ioteyeapi_test source/ioteyeapi_test.cpp)
target_link_libraries(ioteyeapi_test PRIVATE ioteyeapi::ioteyeapi)
target_compile_features(ioteyeapi_test PRIVATE cxx_std_20)

add_test(NAME ioteyeapi_test COMMAND ioteyeapi_test)

# ---- End-of-file commands ----

add_folders(Test)
