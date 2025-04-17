add_example(example_texturing)

file(COPY "${CMAKE_SOURCE_DIR}/example/example_texturing/textured_cube.fbx"
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/example/example_texturing/cube_texture.png"
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
