cmake_minimum_required(VERSION 3.14...3.27)

project(
  MonteCarlo
  VERSION 0.1
  DESCRIPTION "Implement MC Simulations for Muon Detector")

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

find_package(CMocka CONFIG REQUIRED)
enable_testing()

add_subdirectory(tests)
