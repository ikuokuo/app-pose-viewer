cmake_minimum_required(VERSION 3.0)
project(start-opengl VERSION 1.0.0 LANGUAGES C CXX)

include(cmake/global.cmake)

set(MY_ROOT "${PROJECT_SOURCE_DIR}")

# cmake options

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()
if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
#set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# flags

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wformat")
if(NOT HOST_OS STREQUAL "Win")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
endif()
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

string(STRIP "${CMAKE_CXX_FLAGS}" CMAKE_CXX_FLAGS)
status("CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

# packages

LIST(APPEND CMAKE_MODULE_PATH "${MY_ROOT}/cmake/modules")

## threads

find_package(Threads QUIET)

macro(target_link_threads NAME)
  if(THREADS_HAVE_PTHREAD_ARG)
    target_compile_options(PUBLIC ${NAME} "-pthread")
  endif()
  if(CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(${NAME} "${CMAKE_THREAD_LIBS_INIT}")
  endif()
endmacro()

## glm

# libglm-dev is outdated (ext/), use explicit version instead
#find_package(GLM REQUIRED)
set(GLM_ROOT "${MY_ROOT}/third_party/glm")

## stb

set(STB_ROOT "${MY_ROOT}/third_party/stb")

## assimp

find_package(ASSIMP REQUIRED)

## imgui

set(IMGUI_ROOT "${MY_ROOT}/third_party/imgui")

set(IMGUI_SRCS
  ${IMGUI_ROOT}/imgui.cpp
  ${IMGUI_ROOT}/imgui_demo.cpp
  ${IMGUI_ROOT}/imgui_draw.cpp
  ${IMGUI_ROOT}/imgui_widgets.cpp
  ${IMGUI_ROOT}/examples/imgui_impl_glfw.cpp
  ${IMGUI_ROOT}/examples/imgui_impl_opengl3.cpp
  ${IMGUI_ROOT}/examples/libs/gl3w/GL/gl3w.c
)
add_library(imgui STATIC ${IMGUI_SRCS})
target_include_directories(imgui PUBLIC
  "$<BUILD_INTERFACE:${IMGUI_ROOT}>"
  "$<BUILD_INTERFACE:${IMGUI_ROOT}/examples>"
  "$<BUILD_INTERFACE:${IMGUI_ROOT}/examples/libs/gl3w>"
)
target_compile_definitions(imgui PUBLIC
  IMGUI_IMPL_OPENGL_LOADER_GL3W
)

## target_link_glfw3_libs(NAME)
macro(target_link_glfw3_libs NAME)
  if(MINGW)  # MinGW
    execute_process(COMMAND pkg-config --cflags glfw3 OUTPUT_VARIABLE _glfw3_cflags OUTPUT_STRIP_TRAILING_WHITESPACE)
    target_compile_options(${NAME} PUBLIC ${_glfw3_cflags})
    target_link_libraries(${NAME}
      -lglfw3 -lgdi32 -lopengl32 -limm32
    )
    unset(_glfw3_cflags)
  elseif(APPLE)  # macOS
    target_include_directories(${NAME} PUBLIC
      "$<BUILD_INTERFACE:/usr/local/include>"
    )
    execute_process(COMMAND pkg-config --libs glfw3 OUTPUT_VARIABLE _glfw3_libs OUTPUT_STRIP_TRAILING_WHITESPACE)
    target_link_libraries(${NAME}
      ${_glfw3_libs} "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo" -L/usr/local/lib
    )
    unset(_glfw3_libs)
  elseif(UNIX)  # Linux
    execute_process(COMMAND pkg-config --cflags glfw3 OUTPUT_VARIABLE _glfw3_cflags OUTPUT_STRIP_TRAILING_WHITESPACE)
    target_compile_options(${NAME} PUBLIC ${_glfw3_cflags})

    execute_process(COMMAND pkg-config --static --libs glfw3 OUTPUT_VARIABLE _glfw3_libs OUTPUT_STRIP_TRAILING_WHITESPACE)
    target_link_libraries(${NAME} -lGL ${_glfw3_libs})

    unset(_glfw3_cflags)
    unset(_glfw3_libs)
  endif()
endmacro()

# targets

set(_outdir "${MY_ROOT}/_output")
set_outdir(
  ARCHIVE "${_outdir}/lib"
  LIBRARY "${_outdir}/lib"
  RUNTIME "${_outdir}/bin"
)
unset(_outdir)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

include_directories(
  ${GLM_ROOT}
  ${STB_ROOT}
  ${ASSIMP_INCLUDE_DIR}
  ${MY_ROOT}/include
)

## poseviewer

add_executable(poseviewer
  ${MY_ROOT}/src/base/glfw_base.cpp
  ${MY_ROOT}/src/base/imgui_base.cpp
  ${MY_ROOT}/src/base/imgui_glfw_base.cpp
  ${MY_ROOT}/src/app_imgui.cpp
  ${MY_ROOT}/src/app_imgui_glfw.cpp
  ${MY_ROOT}/src/main.cpp
)

target_include_directories(poseviewer PUBLIC
  "$<BUILD_INTERFACE:${MY_ROOT}/src>"
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/src>"
)

target_link_libraries(poseviewer imgui)
target_link_glfw3_libs(poseviewer)

# install

install(TARGETS poseviewer
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# others

if(BUILD_DEMOS)
  add_subdirectory(src/demo)
endif()
