set(MY_CURR ${CMAKE_CURRENT_LIST_DIR})

set(_demos)

## glfw_demo

find_package(GLEW)

if(GLEW_FOUND)
  list(APPEND _demos glfw_demo)

  add_executable(glfw_demo
    ${MY_ROOT}/src/base/glfw_base.cpp
    ${MY_ROOT}/src/demo/glfw_demo.cpp
  )
  target_include_directories(glfw_demo PUBLIC
    "$<BUILD_INTERFACE:${MY_ROOT}/src>"
  )
  target_link_libraries(glfw_demo GLEW::GLEW)
  target_link_glfw3_libs(glfw_demo)
else()
  status("Skip glfw_demo, as GLEW not found")
endif()

## imgui_demo

list(APPEND _demos imgui_demo)

add_executable(imgui_demo
  ${MY_ROOT}/src/base/glfw_base.cpp
  ${MY_ROOT}/src/base/imgui_base.cpp
  ${MY_ROOT}/src/base/imgui_glfw_base.cpp
  ${MY_ROOT}/src/demo/imgui_demo.cpp
)
target_include_directories(imgui_demo PUBLIC
  "$<BUILD_INTERFACE:${MY_ROOT}/src>"
)
target_link_libraries(imgui_demo imgui)
target_link_glfw3_libs(imgui_demo)

## install

if(_demos)
  install(TARGETS ${_demos}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )
endif()

unset(_demos)
