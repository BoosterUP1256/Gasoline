include(FetchContent)

# SFML
FetchContent_Declare(
  SFML
  URL https://github.com/SFML/SFML/archive/refs/tags/2.6.1.zip
)
add_subdirectory(sfml)

# Dear ImGui
FetchContent_Declare(
  imgui
  URL https://github.com/ocornut/imgui/archive/v1.90.zip
)

FetchContent_MakeAvailable(imgui)

# ImGui-SFML
FetchContent_Declare(
  imgui-sfml
  URL https://github.com/SFML/imgui-sfml/archive/v2.6.zip
)
add_subdirectory(imgui-sfml)