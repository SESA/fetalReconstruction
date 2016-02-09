# Finding GNU scientific library GSL
FIND_PACKAGE(GSL REQUIRED)
set(GSL_LIBRARIES ${GSL_LIBRARIES} ${GSL_LIB})
#IF (GSL_FOUND)
  INCLUDE_DIRECTORIES(${GSL_INCLUDE_DIR})
  LINK_DIRECTORIES(${GSL_LINK_DIRECTORIES})
  LINK_LIBRARIES(${GSL_LIBRARIES})
#ENDIF (GSL_FOUND)

SET(BUILD_OUT_OF_IRTK_TREE TRUE)
if(USE_SYSTEM_IRTK)
#if IRTK_SOURCE_DIR is set, we are building within the IRTK source tree
  if(IRTK_SOURCE_DIR)
    SET(BUILD_OUT_OF_IRTK_TREE FALSE)
  else(IRTK_SOURCE_DIR)
    SET(BUILD_OUT_OF_IRTK_TREE TRUE)
  endif(IRTK_SOURCE_DIR)
INCLUDE_DIRECTORIES(${IRTK_DIR}/../packages/registration/include)
endif(USE_SYSTEM_IRTK)


if(BUILD_OUT_OF_IRTK_TREE AND USE_SYSTEM_IRTK)
# we do not need to search for IRTK for in source tree build
find_package(IRTK REQUIRED)
#message("${IRTK_LIBRARIES_DIR}")
include_directories(${IRTK_INCLUDE_DIRS})
link_directories(${IRTK_LIBRARIES_DIR})
endif(BUILD_OUT_OF_IRTK_TREE AND USE_SYSTEM_IRTK)


FIND_PACKAGE(TBB REQUIRED)
IF (TBB_FOUND)
    # Attention: DO NOT define TBB_DEPRECATED by default or before including the
    #            other TBB header files, in particular parallel_for. The deprecated
    #            behavior of parallel_for is to not choose the chunk size (grainsize)
    #            automatically!
    #
    # http://software.intel.com/sites/products/documentation/doclib/tbb_sa/help/tbb_userguide/Automatic_Chunking.htm
    ADD_DEFINITIONS(-DHAS_TBB)
    INCLUDE_DIRECTORIES(${TBB_INCLUDE_DIRS})
    LINK_DIRECTORIES(${TBB_LIBRARY_DIRS})
    LINK_LIBRARIES(${TBB_LIBRARIES})
ENDIF (TBB_FOUND)
#message("ignore the support TBB error. it works. Do not ignore the not found error!")

if(USE_SYSTEM_IRTK)
FIND_PACKAGE(OpenCV REQUIRED)
include_directories(${OpenCV_DIR}/include ${OpenCV_DIR}/include/opencv)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
else(USE_SYSTEM_IRTK)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/IRTKSimple2/)
set(IRTK_LIBRARIES common++ contrib++ geometry++ image++ registration++ transformation++ zlib)
endif(USE_SYSTEM_IRTK)

INCLUDE_DIRECTORIES(${TBB_INSTALL_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/reconstructionGPU2/)

## patch based preparations 
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/patch-based/)



