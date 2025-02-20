cmake_minimum_required(VERSION 3.14)

project(RendererExamples CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)

if(PROJECT_IS_TOP_LEVEL)
  find_package(Renderer REQUIRED)
endif()

add_custom_target(run-examples)

function(add_example NAME)
  add_executable(
    "${NAME}" "${NAME}.cpp"
              "${CMAKE_SOURCE_DIR}/example/BMP_Writer/BMP_Writer.cpp")
  target_include_directories("${NAME}" PRIVATE BMP_Writer)
  target_link_libraries("${NAME}" PRIVATE Renderer::Renderer)
  target_compile_features("${NAME}" PRIVATE cxx_std_20)
  add_custom_target(
    "run_${NAME}"
    COMMAND "${NAME}"
    VERBATIM)
  add_dependencies("run_${NAME}" "${NAME}")
  add_dependencies(run-examples "run_${NAME}")
endfunction()

add_subdirectory(example_simple_piramide)
add_subdirectory(example_load_obj)
add_subdirectory(example_multiple_objects)
