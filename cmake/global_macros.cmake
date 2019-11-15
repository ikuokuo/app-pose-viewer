include(CMakeParseArguments)

# status(TEXT [IF cond text [ELIF cond text] [ELSE cond text]])
macro(status TEXT)
  set(options)
  set(oneValueArgs)
  set(multiValueArgs IF ELIF ELSE)
  cmake_parse_arguments(THIS "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN})

  #message(STATUS "TEXT: ${TEXT}")
  #message(STATUS "THIS_IF: ${THIS_IF}")
  #message(STATUS "THIS_ELIF: ${THIS_ELIF}")
  #message(STATUS "THIS_ELSE: ${THIS_ELSE}")

  set(__msg_list "")
  set(__continue TRUE)

  if(__continue AND DEFINED THIS_IF)
    #message(STATUS "-- THIS_IF: ${THIS_IF}")
    list(LENGTH THIS_IF __if_len)
    if(${__if_len} GREATER 1)
      list(GET THIS_IF 0 __if_cond)
      if(${__if_cond})
        list(REMOVE_AT THIS_IF 0)
        list(APPEND __msg_list ${THIS_IF})
        set(__continue FALSE)
      endif()
    else()
      message(FATAL_ERROR "status() IF must have cond and text, >= 2 items")
    endif()
  endif()

  if(__continue AND DEFINED THIS_ELIF)
    #message(STATUS "-- THIS_ELIF: ${THIS_ELIF}")
    list(LENGTH THIS_ELIF __elif_len)
    if(${__elif_len} GREATER 1)
      list(GET THIS_ELIF 0 __elif_cond)
      if(${__elif_cond})
        list(REMOVE_AT THIS_ELIF 0)
        list(APPEND __msg_list ${THIS_ELIF})
        set(__continue FALSE)
      endif()
    else()
      message(FATAL_ERROR "status() ELIF must have cond and text, >= 2 items")
    endif()
  endif()

  if(__continue AND DEFINED THIS_ELSE)
    #message(STATUS "-- THIS_ELSE: ${THIS_ELSE}")
    list(LENGTH THIS_ELSE __else_len)
    if(${__else_len} GREATER 0)
      list(APPEND __msg_list ${THIS_ELSE})
    else()
      message(FATAL_ERROR "status() ELSE must have text, >= 1 items")
    endif()
  endif()

  string(REPLACE ";" "" __msg_list "${__msg_list}")
  message(STATUS "${TEXT}${__msg_list}")
endmacro()

# status_var(variableName)
macro(status_var variableName)
  message(STATUS "${variableName}: ${${variableName}}")
endmacro()

# set_outdir(ARCHIVE outdir LIBRARY outdir RUNTIME outdir [KEEP_CONFIG])
macro(set_outdir)
  set(options KEEP_CONFIG)
  set(oneValueArgs ARCHIVE LIBRARY RUNTIME)
  set(multiValueArgs)
  cmake_parse_arguments(THIS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(THIS_ARCHIVE)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${THIS_ARCHIVE})
  endif()
  if(THIS_LIBRARY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${THIS_LIBRARY})
  endif()
  if(THIS_RUNTIME)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${THIS_RUNTIME})
  endif()

  if(NOT THIS_KEEP_CONFIG)
    foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
      string(TOUPPER ${CONFIG} CONFIG)
      set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
      set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
      set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    endforeach()
  endif()
endmacro()

# set_target_outdir(NAME ARCHIVE outdir LIBRARY outdir RUNTIME outdir [KEEP_CONFIG])
macro(set_target_outdir NAME)
  set(options KEEP_CONFIG)
  set(oneValueArgs ARCHIVE LIBRARY RUNTIME)
  set(multiValueArgs)
  cmake_parse_arguments(THIS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  #message(STATUS "NAME: ${NAME}")
  #message(STATUS "THIS_ARCHIVE: ${THIS_ARCHIVE}")
  #message(STATUS "THIS_LIBRARY: ${THIS_LIBRARY}")
  #message(STATUS "THIS_RUNTIME: ${THIS_RUNTIME}")
  #message(STATUS "THIS_KEEP_CONFIG: ${THIS_KEEP_CONFIG}")

  if(THIS_ARCHIVE)
    set_target_properties(${NAME} PROPERTIES
      ARCHIVE_OUTPUT_DIRECTORY "${THIS_ARCHIVE}"
    )
  endif()
  if(THIS_LIBRARY)
    set_target_properties(${NAME} PROPERTIES
      LIBRARY_OUTPUT_DIRECTORY "${THIS_LIBRARY}"
    )
  endif()
  if(THIS_RUNTIME)
    set_target_properties(${NAME} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${THIS_RUNTIME}"
    )
  endif()

  if(NOT THIS_KEEP_CONFIG)
    foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
      string(TOUPPER ${CONFIG} CONFIG)
      if(THIS_ARCHIVE)
        set_target_properties(${NAME} PROPERTIES
          ARCHIVE_OUTPUT_DIRECTORY_${CONFIG} "${THIS_ARCHIVE}"
        )
      endif()
      if(THIS_LIBRARY)
        set_target_properties(${NAME} PROPERTIES
          LIBRARY_OUTPUT_DIRECTORY_${CONFIG} "${THIS_LIBRARY}"
        )
      endif()
      if(THIS_RUNTIME)
        set_target_properties(${NAME} PROPERTIES
          RUNTIME_OUTPUT_DIRECTORY_${CONFIG} "${THIS_RUNTIME}"
        )
      endif()
    endforeach()
  endif()
endmacro()

# set_version_values(VARIABLE)
macro(set_version_values VARIABLE)
  string(REPLACE "." ";" __version_list "${${VARIABLE}}")
  list(LENGTH __version_list __len)
  if(${__len} GREATER 0)
    list(GET __version_list 0 ${VARIABLE}_MAJOR)
  endif()
  if(${__len} GREATER 1)
    list(GET __version_list 1 ${VARIABLE}_MINOR)
  endif()
  if(${__len} GREATER 2)
    list(GET __version_list 2 ${VARIABLE}_PATCH)
  endif()
  if(${__len} GREATER 3)
    list(GET __version_list 3 ${VARIABLE}_TWEAK)
  endif()
endmacro()
