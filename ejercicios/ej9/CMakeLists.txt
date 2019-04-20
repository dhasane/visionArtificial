CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

## ================
## = Project name =
## ================

PROJECT( Intro_ITK_OCV )

## ============
## = Packages =
## ============

FIND_PACKAGE(ITK REQUIRED)
IF(ITK_FOUND)
  INCLUDE(${ITK_USE_FILE})
ENDIF(ITK_FOUND)

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

ADD_EXECUTABLE( watershed watershed.cxx )
TARGET_LINK_LIBRARIES( watershed ${ITK_LIBRARIES} )

#ADD_EXECUTABLE( confidence confidence.cxx )
#TARGET_LINK_LIBRARIES( confidence ${ITK_LIBRARIES} )

ADD_EXECUTABLE( connected connected.cxx )
TARGET_LINK_LIBRARIES( connected ${ITK_LIBRARIES} )

