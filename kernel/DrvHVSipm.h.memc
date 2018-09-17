#ifndef  _DCS_kernel_drv_DrvHVSipm_h_
#define  _DCS_kernel_drv_DrvHVSipm_h_ 1
//#include "DrvHVSipm.h"   //in c file

#include"VDeviceDriver.h"
// #include "VCaenHVBoard.h"


#include "fwk/fwkException.h"
#include "fwk/utlMessageBus.h"

#include<vector>
#include<memory>
#include<cstdint>
#include<sstream>
#include<mutex>

class DrvHVSipm_except:public fwk::Exception{
  public:
    class Except_OK                   ;
    class Except_SYSERR               ;
    class Except_WRITEERR             ;
    class Except_READERR              ;
    class Except_TIMEERR              ;
    class Except_DOWN                 ;
    class Except_NOTPRES              ;
  public:

    //DrvHVSipm_except(){fType="DrvHVSipm_except";}
    static DrvHVSipm_except& HVSipmRetStatus(int i,int retstat,const std::string& add=std::string());
    DrvHVSipm_except(const std::string&a):fwk::Exception(a){}
    //virtual ~DrvHVSipm_except(){}
};



class DrvHVSipm_except::Except_OK                   : public DrvHVSipm_except {public: Except_OK                  (const std::string&s):DrvHVSipm_except(s){ fwk::Exception::SetType(*this); }; };
class DrvHVSipm_except::Except_SYSERR               : public DrvHVSipm_except {public: Except_SYSERR              (const std::string&s):DrvHVSipm_except(s){ fwk::Exception::SetType(*this); }; };
class DrvHVSipm_except::Except_WRITEERR             : public DrvHVSipm_except {public: Except_WRITEERR            (const std::string&s):DrvHVSipm_except(s){ fwk::Exception::SetType(*this); }; };
class DrvHVSipm_except::Except_READERR              : public DrvHVSipm_except {public: Except_READERR             (const std::string&s):DrvHVSipm_except(s){ fwk::Exception::SetType(*this); }; };
class DrvHVSipm_except::Except_TIMEERR              : public DrvHVSipm_except {public: Except_TIMEERR             (const std::string&s):DrvHVSipm_except(s){ fwk::Exception::SetType(*this); }; };
class DrvHVSipm_except::Except_DOWN                 : public DrvHVSipm_except {public: Except_DOWN                (const std::string&s):DrvHVSipm_except(s){ fwk::Exception::SetType(*this); }; };
class DrvHVSipm_except::Except_NOTPRES              : public DrvHVSipm_except {public: Except_NOTPRES             (const std::string&s):DrvHVSipm_except(s){ fwk::Exception::SetType(*this); }; };








class DrvHVSipm:public VDeviceDriver{
  public:
    DrvHVSipm(const std::string&lab,VDeviceBase*parp):VDeviceDriver(lab,parp){}
    ~DrvHVSipm(){}

    void OnStartLocal(){}
    void OnStopLocal (){}
    void SetLocalParams(std::set<std::string>){}
    void UpdateAllLocalParams(){}

    void AssertInit();
    void SetParams(std::set<std::string>);
    void DebugDump();

    void SetIPAddress(const std::string&s){fIPAddress=s;}
    void SetPort(const std::string&s){fPort=s;}
    //void SetUsername (const std::string&s){fUsername =s;}
    //void SetPassword (const std::string&s){fPassword =s;}

  private:
    void ComDeinit(int);
    int  ComInit();
    void Finalize();
    void OnCycleLocal();

  public:
    const int & GetHVSipmHandle()const{return fHVSipmHandle;}
    std::mutex& HVSipmHandle_mutex(){return fHVSipmHandle_mutex;}

  private:
    int fHVSipmHandle; ///< This handle is to be used only for reading.
    std::mutex fHVSipmHandle_mutex;///< mutex barrier for multithreading
    //std::lock_guard<std::mutex> guard(fHVSipmHandle_mutex);
    std::string fIPAddress;
    std::string fPort;
    //std::string fUsername;
    //std::string fPassword;


};


#endif
