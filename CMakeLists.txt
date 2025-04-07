cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
  Renderer
  VERSION 0.0.1
  DESCRIPTION "Курсовой проект по созданию 3D рендерера на C++"
  HOMEPAGE_URL "https://example.com/"
  LANGUAGES CXX)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

include(cmake/external_libs.cmake)

# ---- Declare library ----

add_library(Renderer_Renderer)
add_library(Renderer::Renderer ALIAS Renderer_Renderer)

target_compile_definitions(Renderer_Renderer PUBLIC RENDERER_STATIC_DEFINE)

set_target_properties(
  Renderer_Renderer
  PROPERTIES VERSION "${PROJECT_VERSION}"
             SOVERSION "${PROJECT_VERSION_MAJOR}"
             EXPORT_NAME Renderer
             OUTPUT_NAME Renderer)

target_include_directories(
  Renderer_Renderer ${warning_guard}
  PUBLIC "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>")

target_include_directories(Renderer_Renderer PRIVATE ${glm_SOURCE_DIR})
target_include_directories(Renderer_Renderer PRIVATE ${assimp_SOURCE_DIR})

target_link_libraries(Renderer_Renderer PRIVATE assimp::assimp)

target_compile_features(Renderer_Renderer PUBLIC cxx_std_20)

# ---- Add sources ----

add_subdirectory(source)

# ---- Install rules ----

# if(NOT CMAKE_SKIP_INSTALL_RULES) include(cmake/install-rules.cmake) endif()

# ---- Examples ----

if(PROJECT_IS_TOP_LEVEL)
  option(BUILD_EXAMPLES "Build examples tree." "${Renderer_DEVELOPER_MODE}")
  if(BUILD_EXAMPLES)
    add_subdirectory(example)
  endif()
endif()

# ---- Docs ----

include(cmake/docs.cmake)

# ---- Developer mode ----

if(NOT Renderer_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
    AUTHOR_WARNING "Developer mode is intended for developers of Renderer")
endif()

include(cmake/dev-mode.cmake)
