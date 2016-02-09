project(FetalReconstruction)

cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0010 OLD)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH  ${CMAKE_SOURCE_DIR}/lib)

option(USE_SYSTEM_IRTK "use system IRTK version instead of simplified included version" OFF)
option(BUILD_WITH_CULA "build with CULA support, necessary for automatic motion measurement" OFF)
option(USE_CPU "build for CPU execution with no GPA support" ON)

IF(USE_CPU)
  INCLUDE(${CMAKE_SOURCE_DIR}/CMakeLists_CPU.cmake)
ELSE()
  INCLUDE(${CMAKE_SOURCE_DIR}/CMakeLists_GPU.cmake)
ENDIF()
