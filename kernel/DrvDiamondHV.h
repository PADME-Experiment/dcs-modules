#ifndef  _DCS_kernel_drv_DrvPadmeDiamond_h_
#define  _DCS_kernel_drv_DrvPadmeDiamond_h_ 1
//#include "DrvPadmeDiamond.h"   //in c file

#include"VDeviceDriver.h"
// #include "VCaenHVBoard.h"


#include "fwk/fwkException.h"
#include "fwk/utlMessageBus.h"

#include<vector>
#include<memory>
#include<cstdint>
#include<sstream>
#include<mutex>

class DrvPadmeDiamond_except:public fwk::Exception{
  public:
    class Except_OK                   ;
    class Except_SYSERR               ;
    class Except_WRITEERR             ;
    class Except_READERR              ;
    class Except_TIMEERR              ;
    class Except_DOWN                 ;
    class Except_NOTPRES              ;
  public:

    //DrvPadmeDiamond_except(){fType="DrvPadmeDiamond_except";}
    static DrvPadmeDiamond_except& PadmeDiamondRetStatus(int i,int retstat,const std::string& add=std::string());
    DrvPadmeDiamond_except(const std::string&a):fwk::Exception(a){}
    //virtual ~DrvPadmeDiamond_except(){}
};



class DrvPadmeDiamond_except::Except_OK                   : public DrvPadmeDiamond_except {public: Except_OK                  (const std::string&s):DrvPadmeDiamond_except(s){ fwk::Exception::SetType(*this); }; };
class DrvPadmeDiamond_except::Except_SYSERR               : public DrvPadmeDiamond_except {public: Except_SYSERR              (const std::string&s):DrvPadmeDiamond_except(s){ fwk::Exception::SetType(*this); }; };
class DrvPadmeDiamond_except::Except_WRITEERR             : public DrvPadmeDiamond_except {public: Except_WRITEERR            (const std::string&s):DrvPadmeDiamond_except(s){ fwk::Exception::SetType(*this); }; };
class DrvPadmeDiamond_except::Except_READERR              : public DrvPadmeDiamond_except {public: Except_READERR             (const std::string&s):DrvPadmeDiamond_except(s){ fwk::Exception::SetType(*this); }; };
class DrvPadmeDiamond_except::Except_TIMEERR              : public DrvPadmeDiamond_except {public: Except_TIMEERR             (const std::string&s):DrvPadmeDiamond_except(s){ fwk::Exception::SetType(*this); }; };
class DrvPadmeDiamond_except::Except_DOWN                 : public DrvPadmeDiamond_except {public: Except_DOWN                (const std::string&s):DrvPadmeDiamond_except(s){ fwk::Exception::SetType(*this); }; };
class DrvPadmeDiamond_except::Except_NOTPRES              : public DrvPadmeDiamond_except {public: Except_NOTPRES             (const std::string&s):DrvPadmeDiamond_except(s){ fwk::Exception::SetType(*this); }; };








class DrvPadmeDiamond:public VDeviceDriver{
  public:
    DrvPadmeDiamond(const std::string&lab,VDeviceBase*parp):VDeviceDriver(lab,parp){}
    ~DrvPadmeDiamond(){}

    void OnStartLocal(){}
    void OnStopLocal (){}
    void SetLocalParams(std::set<std::string>){}
    void UpdateAllLocalParams(){}

    void AssertInit();
    void SetParams(std::set<std::string>);
    void DebugDump();

    void SetIPAddress(const std::string&s){fIPAddress=s;}
    //void SetUsername (const std::string&s){fUsername =s;}
    //void SetPassword (const std::string&s){fPassword =s;}
    void SetPort(const std::string&s){fPort=s;}

  private:
    void ComDeinit(int);
    int  ComInit();
    void Finalize();
    void OnCycleLocal();

  public:
    const int & GetPadmeDiamondHandle()const{return fPadmeDiamondHandle;}
    std::mutex& PadmeDiamondHandle_mutex(){return fPadmeDiamondHandle_mutex;}

  private:
    int fPadmeDiamondHandle; ///< This handle is to be used only for reading.
    std::mutex fPadmeDiamondHandle_mutex;///< mutex barrier for multithreading
    //std::lock_guard<std::mutex> guard(fPadmeDiamondHandle_mutex);
    std::string fIPAddress;
    std::string fPort;
    //std::string fUsername;
    //std::string fPassword;


};


#endif
