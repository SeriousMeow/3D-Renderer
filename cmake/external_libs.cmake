include(FetchContent)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 0af55ccecd98d4e5a8d1fad7de25ba429d60e863 # v1.0.1
)

FetchContent_Declare(
  assimp
  GIT_REPOSITORY https://github.com/assimp/assimp.git
  GIT_TAG c35200e38ea8f058812b83de2ef32c6093b0ece2 # v5.4.3
)

FetchContent_MakeAvailable(glm)

# Опции assimp

set(BUILD_SHARED_LIBS OFF)
set(ASSIMP_NO_EXPORT OFF)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_INSTALL OFF)

FetchContent_MakeAvailable(assimp)
