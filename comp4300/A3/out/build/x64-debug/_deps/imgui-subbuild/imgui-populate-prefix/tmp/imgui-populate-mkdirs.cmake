# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/dev/Gasoline/comp4300/A3/out/build/x64-debug/_deps/imgui-src"
  "C:/dev/Gasoline/comp4300/A3/out/build/x64-debug/_deps/imgui-build"
  "C:/dev/Gasoline/comp4300/A3/out/build/x64-debug/_deps/imgui-subbuild/imgui-populate-prefix"
  "C:/dev/Gasoline/comp4300/A3/out/build/x64-debug/_deps/imgui-subbuild/imgui-populate-prefix/tmp"
  "C:/dev/Gasoline/comp4300/A3/out/build/x64-debug/_deps/imgui-subbuild/imgui-populate-prefix/src/imgui-populate-stamp"
  "C:/dev/Gasoline/comp4300/A3/out/build/x64-debug/_deps/imgui-subbuild/imgui-populate-prefix/src"
  "C:/dev/Gasoline/comp4300/A3/out/build/x64-debug/_deps/imgui-subbuild/imgui-populate-prefix/src/imgui-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/dev/Gasoline/comp4300/A3/out/build/x64-debug/_deps/imgui-subbuild/imgui-populate-prefix/src/imgui-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/dev/Gasoline/comp4300/A3/out/build/x64-debug/_deps/imgui-subbuild/imgui-populate-prefix/src/imgui-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()