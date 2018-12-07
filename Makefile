ccfiles=$(shell find kernel -name "*.cc")
ccfwkfiles=$(shell find fwk -name "*.cc")
hhfiles=$(shell find kernel -name "*.h")


.PHONY: kernel.exe doxygen

kernel.exe: $(ccfiles) $(hhfiles)
	g++ -g -std=gnu++11 $(ccfiles) $(ccfwkfiles) -o dcs_kernel.exe -I. -I ./kernel/extern/CAEN_VME8100_1_0linux/CAENCrateToolBox/SDK/common  -I ./kernel/extern/CAEN_VME8100_1_0linux/CAENCrateToolBox/SDK/V8100SDK -L./kernel/extern/CAEN_VME8100_1_0linux/CAENCrateToolBox/SDK/V8100SDK -lV8100SDK -lc -DUNIX   -lcaenhvwrapper -lncurses -lpthread -ldl -lm -g -O0 -lyaml-cpp -lmemcached -Wpedantic  -DLINUX -D__GCC

doxygen:
	cd docs;doxygen

HVcontrol.exe:
	g++ extras/PadmeHV-tmp/HVcontrol.c  -I. -Lcaenlib -lcaenhvwrapper -lc -o HVcontrol.exe -DUNIX -I caenlib/ -lcaenhvwrapper -lncurses -lpthread -ldl -lm

setvolt_ecal_all:	setvolt_ecal_all.cc
	g++ -g  setvolt_ecal_all.cc  -I. -Lcaenlib -lcaenhvwrapper -lc -o setvolt_ecal_all.exe -DUNIX -I caenlib/ -lcaenhvwrapper -lncurses -lpthread -ldl -lm

setvolt_ecal_single:	setvolt_ecal_single.cc
	g++ -g setvolt_ecal_single.cc  -I. -Lcaenlib -lcaenhvwrapper -lc -o setvolt_ecal_single.exe -DUNIX -I caenlib/ -lcaenhvwrapper -lncurses -lpthread -ldl -lm

readfile_ecal:	readfile_ecal.cc
	g++ -o readfile_ecal.exe readfile_ecal.cc

readfile_sac:	readfile_sac.cc
	g++ -o readfile_sac.exe readfile_sac.cc

readfile_cosmics:	readfile_cosmics.cc
	g++ -o readfile_cosmics.exe readfile_cosmics.cc

mon_summ_hvalarm:	mon_summ_hvalarm.cc
	g++ -o mon_summ_hvalarm.exe mon_summ_hvalarm.cc
