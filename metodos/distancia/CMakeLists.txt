CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

## ================
## = Project name =
## ================

PROJECT( FILTRO )

## ============
## = Packages =
## ============

FIND_PACKAGE( OpenCV REQUIRED )
#FIND_PACKAGE(ITK REQUIRED)
#IF(ITK_FOUND)
#  INCLUDE(${ITK_USE_FILE})
##ENDIF(ITK_FOUND)

## ================================================
## = Do not allow to build inside the source tree =
## ================================================

IF(PROJECT_BINARY_DIR STREQUAL ${PROJECT_SOURCE_DIR})
  MESSAGE(FATAL_ERROR "Building in the source tree is not allowed")
ENDIF(PROJECT_BINARY_DIR STREQUAL ${PROJECT_SOURCE_DIR})

## =====================================
## = Where to put executables and libs =
## =====================================

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
MARK_AS_ADVANCED(
  CMAKE_BACKWARDS_COMPATIBILITY
  EXECUTABLE_OUTPUT_PATH
  LIBRARY_OUTPUT_PATH
  )

## =======================
## = Programs to compile =
## =======================

ADD_EXECUTABLE( distancia distancia.cxx )
TARGET_LINK_LIBRARIES( distancia ${OpenCV_LIBS} )

#ADD_EXECUTABLE( otsu otsu.cxx )
#TARGET_LINK_LIBRARIES( otsu ${OpenCV_LIBS} )

