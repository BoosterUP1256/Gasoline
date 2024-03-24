add_executable(A3 "main.cpp" "Animation.h" "Animation.cpp" "Vec2.cpp" "Vec2.h" "Physics.h" "Physics.cpp" "Entity.h" "Components.h" "Scene_Play.h" "Scene_Play.cpp")
target_link_libraries(A3 PRIVATE ImGui-SFML::ImGui-SFML)

# Copy DLLs needed for runtime on Windows
if(WIN32)
  if (BUILD_SHARED_LIBS)
    add_custom_command(TARGET A3 POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
          $<TARGET_FILE:ImGui-SFML::ImGui-SFML>
          $<TARGET_FILE:sfml-graphics>
          $<TARGET_FILE:sfml-window>
          $<TARGET_FILE:sfml-system>
          $<TARGET_FILE_DIR:A3>)
  endif()
endif()