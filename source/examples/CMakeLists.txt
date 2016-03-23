#######################################################
# cmake file for building DQM4ILC examples
# @author Eté Rémi , IPNL
# modifuied by A. Irles DESY
# Copyright (c) CNRS / IPNL
#######################################################


ADD_SHARED_LIBRARY( CaloHitModule CaloHitModule/CaloHitModule.cc )
ADD_SHARED_LIBRARY( AHCALRawModule AHCALRawModule/AHCALRawModule.cc )

TARGET_LINK_LIBRARIES( CaloHitModule ${PROJECT_NAME} )
TARGET_LINK_LIBRARIES( AHCALRawModule ${PROJECT_NAME} )

INSTALL( 
	TARGETS CaloHitModule AHCALRawModule
	LIBRARY DESTINATION  lib 
)


