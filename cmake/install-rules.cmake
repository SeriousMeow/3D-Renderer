if(PROJECT_IS_TOP_LEVEL)
  set(CMAKE_INSTALL_INCLUDEDIR
      "include/Renderer-${PROJECT_VERSION}"
      CACHE STRING "")
  set_property(CACHE CMAKE_INSTALL_INCLUDEDIR PROPERTY TYPE PATH)
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package Renderer)

install(
  DIRECTORY include/
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
  COMPONENT Renderer_Development)

install(
  TARGETS Renderer_Renderer
  EXPORT RendererTargets
  ARCHIVE #
          COMPONENT Renderer_Development
  INCLUDES #
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

write_basic_package_version_file("${package}ConfigVersion.cmake"
                                 COMPATIBILITY SameMajorVersion)

# Allow package maintainers to freely override the path for the configs
set(Renderer_INSTALL_CMAKEDIR
    "${CMAKE_INSTALL_LIBDIR}/cmake/${package}"
    CACHE STRING "CMake package config location relative to the install prefix")
set_property(CACHE Renderer_INSTALL_CMAKEDIR PROPERTY TYPE PATH)
mark_as_advanced(Renderer_INSTALL_CMAKEDIR)

install(
  FILES cmake/install-config.cmake
  DESTINATION "${Renderer_INSTALL_CMAKEDIR}"
  RENAME "${package}Config.cmake"
  COMPONENT Renderer_Development)

install(
  FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
  DESTINATION "${Renderer_INSTALL_CMAKEDIR}"
  COMPONENT Renderer_Development)

install(
  EXPORT RendererTargets
  NAMESPACE Renderer::
  DESTINATION "${Renderer_INSTALL_CMAKEDIR}"
  COMPONENT Renderer_Development)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
