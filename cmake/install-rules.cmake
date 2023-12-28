if(PROJECT_IS_TOP_LEVEL)
  set(
      CMAKE_INSTALL_INCLUDEDIR "include/ioteyeapi-${PROJECT_VERSION}"
      CACHE STRING ""
  )
  set_property(CACHE CMAKE_INSTALL_INCLUDEDIR PROPERTY TYPE PATH)
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package ioteyeapi)

install(
    DIRECTORY
    include/
    "${PROJECT_BINARY_DIR}/export/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT ioteyeapi_Development
)

install(
    TARGETS ioteyeapi_ioteyeapi
    EXPORT ioteyeapiTargets
    RUNTIME #
    COMPONENT ioteyeapi_Runtime
    LIBRARY #
    COMPONENT ioteyeapi_Runtime
    NAMELINK_COMPONENT ioteyeapi_Development
    ARCHIVE #
    COMPONENT ioteyeapi_Development
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    ioteyeapi_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${package}"
    CACHE STRING "CMake package config location relative to the install prefix"
)
set_property(CACHE ioteyeapi_INSTALL_CMAKEDIR PROPERTY TYPE PATH)
mark_as_advanced(ioteyeapi_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${ioteyeapi_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT ioteyeapi_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${ioteyeapi_INSTALL_CMAKEDIR}"
    COMPONENT ioteyeapi_Development
)

install(
    EXPORT ioteyeapiTargets
    NAMESPACE ioteyeapi::
    DESTINATION "${ioteyeapi_INSTALL_CMAKEDIR}"
    COMPONENT ioteyeapi_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
