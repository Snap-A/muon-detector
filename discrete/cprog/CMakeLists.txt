cmake_minimum_required(VERSION 3.14...3.27)

project(
  Discrete
  VERSION 0.1
  DESCRIPTION "Implement numerical integration for Muon Detector"
  LANGUAGES C)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  find_package(Doxygen)
  if (Doxygen_FOUND)
    add_subdirectory(docs)
  else()
    message(STATUS "Doxygen not found, not building docs")
  endif()
endif()

add_subdirectory(lib)

add_subdirectory(apps)
