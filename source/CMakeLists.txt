#######################################################
# cmake file for building DQM4ILC package
# @author Eté Rémi , IPNL
# Copyright (c) CNRS / IPNL
#######################################################


ADD_SUBDIRECTORY( common )

IF( BUILD_EXAMPLES )
	ADD_SUBDIRECTORY( examples )
ENDIF()