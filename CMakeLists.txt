########################################################
# cmake file for building DQM4ILC
# @author Remi Ete
CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)
########################################################


# project name
PROJECT( DQM4ILC )

### PACKAGE VERSIONS ########################################################
SET( ${PROJECT_NAME}_VERSION_MAJOR 1 )
SET( ${PROJECT_NAME}_VERSION_MINOR 1 )
SET( ${PROJECT_NAME}_VERSION_PATCH 0 )

###############################
# Require C++11
INCLUDE( CheckCXXCompilerFlag )

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)

IF( COMPILER_SUPPORTS_CXX11 )
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ELSE()
  MESSAGE( SEND_ERROR "DQMCore requires C++11 support. Please upgrade your compiler !" )
ENDIF()

# ----- CLANG 7.0+ throws error for  undefined-bool-conversion warnings in gear/tinyxml.h -----
# ----- error: 'this' pointer cannot be null in well-defined C++ code; pointer may be assumed to always convert to true [-Werror,-Wundefined-bool-conversion] -----
IF (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-undefined-bool-conversion")
ENDIF (CMAKE_CXX_COMPILER_ID MATCHES "Clang")

# ----- dependencies -----
FIND_PACKAGE( ILCUTIL COMPONENTS ILCSOFT_CMAKE_MODULES streamlog REQUIRED )
INCLUDE( ilcsoft_default_settings )

LIST( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )

# From DQM4HEP
FIND_PACKAGE( DQMCore REQUIRED )
FIND_PACKAGE( xdrstream REQUIRED )
FIND_PACKAGE( DIM REQUIRED )
FIND_PACKAGE( LOG4CXX REQUIRED )
FIND_PACKAGE( xdrlcio REQUIRED )

# From ILCSOFT
FIND_PACKAGE( LCIO REQUIRED )
FIND_PACKAGE( Marlin REQUIRED )
FIND_PACKAGE( GEAR REQUIRED )

FOREACH( pkg DQMCore LCIO xdrstream xdrlcio DIM LOG4CXX Marlin GEAR )
	INCLUDE_DIRECTORIES( ${${pkg}_INCLUDE_DIRS} )
	LINK_LIBRARIES( ${${pkg}_LIBRARIES} )
	ADD_DEFINITIONS ( ${${pkg}_DEFINITIONS} )
ENDFOREACH()

# ----- package options -----------------------------------------------------------------------
OPTION( INSTALL_DOC    "Set to OFF to skip build/install Documentation" OFF )
OPTION( BUILD_EXAMPLES "Whether to build DQM4ILC examples" OFF )

ADD_SUBDIRECTORY( source )

###############################
# Generate DQM4ILCeConfig.h
CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.h.cmake.in"
    "${PROJECT_SOURCE_DIR}/source/common/include/${PROJECT_NAME}Config.h" @ONLY )

# display some variables and write them to cache
DISPLAY_STD_VARIABLES()

###############################
# generate and install following configuration files
GENERATE_PACKAGE_CONFIGURATION_FILES( ${PROJECT_NAME}Config.cmake ${PROJECT_NAME}ConfigVersion.cmake ${PROJECT_NAME}LibDeps.cmake )
