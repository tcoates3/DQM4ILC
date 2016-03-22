#######################################################
# cmake file for building DQMCore package
# @author Eté Rémi , IPNL
# Copyright (c) CNRS / IPNL
#######################################################

# include directories
INCLUDE_DIRECTORIES( BEFORE include )

# require proper c++
ADD_DEFINITIONS( "-pedantic -Wunused-value -O2" )
ADD_DEFINITIONS("-Wno-long-long -Wreturn-type -Werror")

# -------------------------------------------------
# build the global library


AUX_SOURCE_DIRECTORY( src SRC_FILES )

ADD_SHARED_LIBRARY( ${PROJECT_NAME} ${SRC_FILES} )

INSTALL(
        TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib
)

# -------------------------------------------------
# build lcio file service executable

ADD_EXECUTABLE( dqm4hep_start_lcio_file_service main/dqm4hep_start_lcio_file_service.cc )
TARGET_LINK_LIBRARIES( dqm4hep_start_lcio_file_service ${PROJECT_NAME} )

INSTALL (
    TARGETS dqm4hep_start_lcio_file_service
    RUNTIME DESTINATION bin
)

INSTALL_DIRECTORY( include DESTINATION . FILES_MATCHING PATTERN "*.h" )


