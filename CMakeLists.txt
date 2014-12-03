cmake_minimum_required(VERSION 2.6)

set(PROJ_NAME CImgMatchingVisualization)
PROJECT(${PROJ_NAME})

# Prevent compilation in-source
if( ${CMAKE_BINARY_DIR} STREQUAL ${PROJECT_SOURCE_DIR} )
  Message( " " )
  Message( FATAL_ERROR "Source and build  directories are the same.
 Create an empty build directory,
 change into it and re-invoke cmake")
endif()


include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


##############################################
## External libraries
##############################################
list( APPEND CMAKE_MODULE_PATH
    ${PROJECT_SOURCE_DIR}/extern
)

# CImg
find_package( CImg REQUIRED )
list( APPEND PROJ_INCLUDE_DIRS
    ${CImg_INCLUDE_DIRS}
)
list( APPEND
    PROJ_LIBRARY_DIRS
    ${CImg_SYSTEM_LIBS_DIR}
)

# Add CIMG Flags to Compilation Flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CIMG_CFLAGS}")

link_directories(${CImg_SYSTEM_LIBS_DIR})
include_directories(${CImg_INCLUDE_DIRS})

add_executable(${PROJ_NAME}
    cimgConvertColor.hpp
    cimgDrawLineThick.hpp
    cimgMatchingViewer.hpp
	main.cpp
)
target_link_libraries(${PROJ_NAME}
	${CImg_SYSTEM_LIBS}
)
