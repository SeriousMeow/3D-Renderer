# ---- Dependencies ----

set(extract_timestamps "")
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24")
  set(extract_timestamps DOWNLOAD_EXTRACT_TIMESTAMP YES)
endif()

# ---- Developer mode parameters ----

if(Renderer_DEVELOPER_MODE)
  set(DOXYGEN_EXTRACT_ALL YES)
  set(DOXYGEN_EXTRACT_PRIVATE YES)
  set(DOXYGEN_EXTRACT_STATIC YES)
  set(DOXYGEN_EXTRACT_LOCAL_METHODS YES)
  set(DOXYGEN_GENERATE_TODOLIST YES)
  set(DOXYGEN_GENERATE_TESTLIST YES)
  set(DOXYGEN_GENERATE_BUGLIST YES)
else()
  set(DOXYGEN_EXTRACT_ALL NO)
  set(DOXYGEN_EXTRACT_PRIVATE NO)
  set(DOXYGEN_EXTRACT_STATIC NO)
  set(DOXYGEN_EXTRACT_LOCAL_METHODS NO)
  set(DOXYGEN_GENERATE_TODOLIST NO)
  set(DOXYGEN_GENERATE_TESTLIST NO)
  set(DOXYGEN_GENERATE_BUGLIST NO)
endif()

# ---- Declare documentation target ----

set(DOXYGEN_OUTPUT_DIRECTORY
    "${PROJECT_BINARY_DIR}/docs"
    CACHE PATH "Path for the generated Doxygen documentation")

set(working_dir "${PROJECT_BINARY_DIR}/docs")

foreach(file IN ITEMS Doxyfile)
  configure_file("docs/${file}.in" "${working_dir}/${file}" @ONLY)
endforeach()

set(config "${working_dir}/Doxyfile")

add_custom_target(
  docs
  COMMAND "${CMAKE_COMMAND}" -E remove_directory
          "${DOXYGEN_OUTPUT_DIRECTORY}/html" "${DOXYGEN_OUTPUT_DIRECTORY}/xml"
  COMMAND "doxygen" "${config}"
  COMMENT "Building documentation using Doxygen"
  WORKING_DIRECTORY "${working_dir}"
  VERBATIM)
