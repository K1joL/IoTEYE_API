cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    ioteyeapi
    VERSION 1.0.0
    DESCRIPTION "Library for IoTEYE server using"
    HOMEPAGE_URL "-"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    ioteyeapi_ioteyeapi
    source/ioteyeapi.cpp
)
add_library(ioteyeapi::ioteyeapi ALIAS ioteyeapi_ioteyeapi)

include(GenerateExportHeader)
generate_export_header(
    ioteyeapi_ioteyeapi
    BASE_NAME ioteyeapi
    EXPORT_FILE_NAME export/ioteyeapi/ioteyeapi_export.hpp
    CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(ioteyeapi_ioteyeapi PUBLIC IOTEYEAPI_STATIC_DEFINE)
endif()

set_target_properties(
    ioteyeapi_ioteyeapi PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME ioteyeapi
    OUTPUT_NAME ioteyeapi
)

target_include_directories(
    ioteyeapi_ioteyeapi ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(
    ioteyeapi_ioteyeapi SYSTEM
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_compile_features(ioteyeapi_ioteyeapi PUBLIC cxx_std_20)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Examples ----

if(PROJECT_IS_TOP_LEVEL)
  option(BUILD_EXAMPLES "Build examples tree." "${ioteyeapi_DEVELOPER_MODE}")
  if(BUILD_EXAMPLES)
    add_subdirectory(example)
  endif()
endif()

# ---- Developer mode ----

if(NOT ioteyeapi_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of ioteyeapi"
  )
endif()

include(cmake/dev-mode.cmake)
