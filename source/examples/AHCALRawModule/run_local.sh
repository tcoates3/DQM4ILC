# script to run without job interface
# tested by A. Irles, 23 March 2016

#
# works in all steps except for the analysis module.
# !!
source /space/airqui/Programs/DQM4HEP/ilcinstall/v01-17-07/init_ilcsoft.sh
export LD_LIBRARY_PATH=/space/airqui/Programs/DQM4HEP/lib:$LD_LIBRARY_PATH
export PATH=/space/airqui/Programs/DQM4HEP/bin:$PATH

export DIM_DNS_NODE=localhost

dns &
#sudo 
# sudo not needed ?? if needed, all the rest should be run in a different
# terminal
# and all the settings should be set again (LD_LIBRARY_PATH, etc)
dimjc_daemon &

export DQM4HEP_PLUGIN_DLL=/space/airqui/Programs/DQM4HEP/lib/libDQM4ILC.so
dqm4hep_start_event_collector -c ahcal_event_collector -s LCIOStreamer &

dqm4hep_start_monitor_element_collector -c ahcal_me_collector &

dqm4hep_start_lcio_file_service -f /space/airqui/Programs/Run_30270__23p08p2015__12p29p10.slcio -c ahcal_event_collector -r -t 100000 &

#The analysis module fails, it does not complains about the AHCALRawModule, but about other stuff...
# if I run the randomodule in examples, I see the same errors.
# BAD CONFIGURATION in the xml file ?
# Wrong communication with the streamer ?

export DQM4HEP_PLUGIN_DLL=/space/airqui/Programs/DQM4HEP/lib/libDQM4ILC.so/space/airqui/Programs/DQM4HEP/lib/libAHCALRawModule.so
dqm4hep_start_analysis_module -f /space/airqui/Programs/DQM4HEP/DQM4ILC/source/examples/AHCALRawModule/lcioAHCALTests.xml






