# check if we have c++11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	message("USING -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
	message("USING -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../IRTKSimple2/common++/include
	${CMAKE_CURRENT_SOURCE_DIR}/../IRTKSimple2/contrib++/include
	${CMAKE_CURRENT_SOURCE_DIR}/../IRTKSimple2/geometry++/include
	${CMAKE_CURRENT_SOURCE_DIR}/../IRTKSimple2/image++/include
	${CMAKE_CURRENT_SOURCE_DIR}/../IRTKSimple2/packages/transformation/include
	${CMAKE_CURRENT_SOURCE_DIR}/../IRTKSimple2/packages/registration/include
	${CMAKE_CURRENT_SOURCE_DIR}
	)

#need to check if Unix is sane # add boost dependencies
find_package( Boost 1.46 REQUIRED COMPONENTS program_options filesystem system thread)
add_definitions(-DUSE_BOOST=1)

if ( NOT Boost_FOUND )
message( STATUS "Boost could not be found." )
   set( BOOST_ROOT ${BOOST_ROOT} CACHE PATH "Please enter path to Boost include folder." FORCE )
endif ()
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_executable(reconstruction_GPU2 reconstruction.cc irtkReconstructionGPU.cc 
          irtkReconstructionGPU.h 
          perfstats.h
          stackMotionEstimator.cpp stackMotionEstimator.h)

target_link_libraries(reconstruction_GPU2 ${IRTK_LIBRARIES} ${TBB_LIBRARIES} ${GSL_LIBRARIES} )

if(UNIX)
target_link_libraries(reconstruction_GPU2 ${Boost_LIBRARIES})
endif(UNIX)

IF(NOT USE_CPU)
  INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists_GPU.cmake)
ENDIF()

