########################################################
# cmake file for building DQM4ILC
# @author Remi Ete
CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)
########################################################


# project name
PROJECT( DQM4ILC )

### PACKAGE VERSIONS ########################################################
SET( ${PROJECT_NAME}_VERSION_MAJOR 1 )
SET( ${PROJECT_NAME}_VERSION_MINOR 0 )
SET( ${PROJECT_NAME}_VERSION_PATCH 0 )


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

# display some variables and write them to cache
DISPLAY_STD_VARIABLES()
