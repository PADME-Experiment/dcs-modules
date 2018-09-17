#ifndef  _DCS_kernel_drv_DrvPadmeAmb_h_
#define  _DCS_kernel_drv_DrvPadmeAmb_h_ 1
//#include "DrvPadmeAmb.h"   //in c file

#include"VDeviceDriver.h"


#include "fwk/fwkException.h"
#include "fwk/utlMessageBus.h"

#include<vector>
#include<memory>
#include<cstdint>
#include<sstream>
#include<mutex>
#include <libmemcached/memcached.h>

class DrvPadmeAmb_except:public fwk::Exception{
  public:
    class Except_OK                   ;
    class Except_SYSERR               ;
    class Except_WRITEERR             ;
    class Except_READERR              ;
    class Except_TIMEERR              ;
    class Except_DOWN                 ;
    class Except_NOTPRES              ;
  public:

    //DrvPadmeAmb_except(){fType="DrvPadmeAmb_except";}
    static DrvPadmeAmb_except& PadmeAmbRetStatus(int i,int retstat,const std::string& add=std::string());
    DrvPadmeAmb_except(const std::string&a):fwk::Exception(a){}
    //virtual ~DrvPadmeAmb_except(){}
};



class DrvPadmeAmb_except::Except_OK                   : public DrvPadmeAmb_except {public: Except_OK                  (const std::string&s):DrvPadmeAmb_except(s){ fwk::Exception::SetType(*this); }; };
class DrvPadmeAmb_except::Except_SYSERR               : public DrvPadmeAmb_except {public: Except_SYSERR              (const std::string&s):DrvPadmeAmb_except(s){ fwk::Exception::SetType(*this); }; };
class DrvPadmeAmb_except::Except_WRITEERR             : public DrvPadmeAmb_except {public: Except_WRITEERR            (const std::string&s):DrvPadmeAmb_except(s){ fwk::Exception::SetType(*this); }; };
class DrvPadmeAmb_except::Except_READERR              : public DrvPadmeAmb_except {public: Except_READERR             (const std::string&s):DrvPadmeAmb_except(s){ fwk::Exception::SetType(*this); }; };
class DrvPadmeAmb_except::Except_TIMEERR              : public DrvPadmeAmb_except {public: Except_TIMEERR             (const std::string&s):DrvPadmeAmb_except(s){ fwk::Exception::SetType(*this); }; };
class DrvPadmeAmb_except::Except_DOWN                 : public DrvPadmeAmb_except {public: Except_DOWN                (const std::string&s):DrvPadmeAmb_except(s){ fwk::Exception::SetType(*this); }; };
class DrvPadmeAmb_except::Except_NOTPRES              : public DrvPadmeAmb_except {public: Except_NOTPRES             (const std::string&s):DrvPadmeAmb_except(s){ fwk::Exception::SetType(*this); }; };








class DrvPadmeAmb:public VDeviceDriver{
  public:
    DrvPadmeAmb(const std::string&lab,VDeviceBase*parp):VDeviceDriver(lab,parp){}
    ~DrvPadmeAmb(){}

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

  private:
    void ComDeinit(int);
    int  ComInit();
    void Finalize();
    void OnCycleLocal();

  public:
    const int & GetPadmeAmbHandle()const{return fPadmeAmbHandle;}
    std::mutex& PadmeAmbHandle_mutex(){return fPadmeAmbHandle_mutex;}

  private:
    int fPadmeAmbHandle; ///< This handle is to be used only for reading.
    std::mutex fPadmeAmbHandle_mutex;///< mutex barrier for multithreading
    //std::lock_guard<std::mutex> guard(fPadmeAmbHandle_mutex);
    std::string fIPAddress;
    std::string fPort;
    memcached_st *memc;

    //std::string fUsername;
    //std::string fPassword;


};


#endif
