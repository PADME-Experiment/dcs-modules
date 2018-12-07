#ifndef  _DCS_kernel_DrvCaenSY4527_h_
#define  _DCS_kernel_DrvCaenSY4527_h_ 1
#include "DrvCaenSY4527.h"   //in c file
#include "DrvCaenHV.h"
#include"VDeviceDriver.h"
#include "VCaenHVBoard.h"

#include "fwk/fwkException.h"
#include "fwk/utlMessageBus.h"

#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include "CAENHVWrapper.h"
#include <ctype.h>
#include <iostream>

class DrvCaenSY4527:public VDeviceDriver{
  public:
 DrvCaenSY4527(const std::string&lab,VDeviceBase*parp):VDeviceDriver(lab,parp){}
  ~DrvCaenSY4527(){}

  void OnStartLocal(){}
  //void OnCycleLocal(){}
  void OnStopLocal (){}
  void SetLocalParams(std::set<std::string>){}
  void UpdateAllLocalParams(){}
 
  void Finalize();
  void AssertInit();
  void SetParams(std::set<std::string>);
 
  
  void SetIPAddress(const std::string&s){fIPAddress=s;}
  void SetUsername (const std::string&s){fUsername =s;}
  void SetPassword (const std::string&s){fPassword =s;}
  
  private:
    void ComDeinit(int);
    int  ComInit();
    void OnCycleLocal();

 public:
    //TODO Setters for all parameters
    void GetCrateMap();
    //const int & GetCaenCrateHandle()const{return fCaenCrateHandle;}
    //std::mutex& GetCaenCrateHandle_mutex(){return fCaenCrateHandle_mutex;}

 
 public:
    const int & GetCaenCrateHandle()const{return fCaenCrateHandle;}
    std::mutex& GetCaenCrateHandle_mutex(){return fCaenCrateHandle_mutex;}

 private:
    int fCaenCrateHandle; ///< This handle is to be used only for reading.
    std::mutex fCaenCrateHandle_mutex;///< mutex barrier for multithreading
    //std::lock_guard<std::mutex> guard(fCaenCrateHandle_mutex);
    std::string fIPAddress;
    std::string fUsername;
    std::string fPassword;


 public:
    // void DebugDump(){  }
};

#endif

