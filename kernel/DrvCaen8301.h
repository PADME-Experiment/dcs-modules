#ifndef  _DCS_kernel_drv_DrvCaen8301_h_
#define  _DCS_kernel_drv_DrvCaen8301_h_ 1
#include "DrvCaen8301.h"   //in c file
extern "C"{
#include"V8301.h"
}

// #include "extern/CAEN_VME8301_1_0linux/CAENCrateToolBox/SDK/V8301SDK/V8301.h"

#include"VDeviceDriver.h"

#include "fwk/fwkException.h"
#include "fwk/utlMessageBus.h"

#include<vector>
#include<memory>
#include<cstdint>
#include<sstream>
#include<mutex>

class DrvCaen8301_except:public fwk::Exception{
  public:
    class Except_OK                   ;
    class Except_SYSERR               ;
    class Except_WRITEERR             ;
    class Except_READERR              ;
    class Except_TIMEERR              ;
  public:

    //DrvCaenHV_except(){fType="DrvCaenHV_except";}
    static DrvCaen8301_except& CAEN8301RetStatus(int retstat,const std::string& add=std::string());
    DrvCaen8301_except(const std::string&a):fwk::Exception(a){}
    //virtual ~DrvCaenHV_except(){}
};



class DrvCaen8301_except::Except_OK                   : public DrvCaen8301_except {public: Except_OK                  (const std::string&s):DrvCaen8301_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaen8301_except::Except_SYSERR               : public DrvCaen8301_except {public: Except_SYSERR              (const std::string&s):DrvCaen8301_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaen8301_except::Except_WRITEERR             : public DrvCaen8301_except {public: Except_WRITEERR            (const std::string&s):DrvCaen8301_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaen8301_except::Except_READERR              : public DrvCaen8301_except {public: Except_READERR             (const std::string&s):DrvCaen8301_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaen8301_except::Except_TIMEERR              : public DrvCaen8301_except {public: Except_TIMEERR             (const std::string&s):DrvCaen8301_except(s){ fwk::Exception::SetType(*this); }; };





class DrvCaen8301:public VDeviceDriver{
  public:
    DrvCaen8301(const std::string&lab,VDeviceBase*parp):VDeviceDriver(lab,parp){}
    ~DrvCaen8301(){}
    void OnStartLocal(){}
    // void OnCycleLocal(){}
    void OnStopLocal (){}
    void SetLocalParams(std::set<std::string>){}
    void UpdateAllLocalParams(){}

    void Finalize();
    void AssertInit();
    void SetParams(std::set<std::string>);

    void SetIPAddress(const std::string&s){fIPAddress=s;}

  private:
    void ComDeinit(V8301_data);
    int  ComInit(V8301_data);
    void OnCycleLocal();

  public:
    // const int & GetCaenCrateHandle()const{return fCaenCrateHandle;}
    // std::mutex& GetCaenCrateHandle_mutex(){return fCaenCrateHandle_mutex;}

  private:
    // int fCaenCrateHandle; ///< This handle is to be used only for reading.
    V8301_data vme_handler;
    std::mutex fCaenCrateHandle_mutex;///< mutex barrier for multithreading
    //std::lock_guard<std::mutex> guard(fCaenCrateHandle_mutex);
    std::string fIPAddress;
};


#endif
