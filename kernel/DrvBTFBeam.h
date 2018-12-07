#ifndef  _DCS_kernel_drv_DrvBTFBeam_h_
#define  _DCS_kernel_drv_DrvBTFBeam_h_ 1
//#include "DrvPadmeAmb.h"   //in c file

#include"VDeviceDriver.h"
// #include "VCaenHVBoard.h"


#include "fwk/fwkException.h"
#include "fwk/utlMessageBus.h"

#include<vector>
#include<memory>
#include<cstdint>
#include<sstream>
#include<mutex>
#include <libmemcached/memcached.h>

class DrvBTFBeam_except:public fwk::Exception{
  public:
    class Except_OK                   ;
    class Except_SYSERR               ;
    class Except_WRITEERR             ;
    class Except_READERR              ;
    class Except_TIMEERR              ;
    class Except_DOWN                 ;
    class Except_NOTPRES              ;
  public:

    //DrvBTFAmb_except(){fType="DrvBTFBeam_except";}
    static DrvBTFBeam_except& BTFBeamRetStatus(int i,int retstat,const std::string& add=std::string());
    DrvBTFBeam_except(const std::string&a):fwk::Exception(a){}
    //virtual ~DrvBTFBeam_except(){}
};



class DrvBTFBeam_except::Except_OK                   : public DrvBTFBeam_except {public: Except_OK                  (const std::string&s):DrvBTFBeam_except(s){ fwk::Exception::SetType(*this); }; };
class DrvBTFBeam_except::Except_SYSERR               : public DrvBTFBeam_except {public: Except_SYSERR              (const std::string&s):DrvBTFBeam_except(s){ fwk::Exception::SetType(*this); }; };
class DrvBTFBeam_except::Except_WRITEERR             : public DrvBTFBeam_except {public: Except_WRITEERR            (const std::string&s):DrvBTFBeam_except(s){ fwk::Exception::SetType(*this); }; };
class DrvBTFBeam_except::Except_READERR              : public DrvBTFBeam_except {public: Except_READERR             (const std::string&s):DrvBTFBeam_except(s){ fwk::Exception::SetType(*this); }; };
class DrvBTFBeam_except::Except_TIMEERR              : public DrvBTFBeam_except {public: Except_TIMEERR             (const std::string&s):DrvBTFBeam_except(s){ fwk::Exception::SetType(*this); }; };
class DrvBTFBeam_except::Except_DOWN                 : public DrvBTFBeam_except {public: Except_DOWN                (const std::string&s):DrvBTFBeam_except(s){ fwk::Exception::SetType(*this); }; };
class DrvBTFBeam_except::Except_NOTPRES              : public DrvBTFBeam_except {public: Except_NOTPRES             (const std::string&s):DrvBTFBeam_except(s){ fwk::Exception::SetType(*this); }; };








class DrvBTFBeam:public VDeviceDriver{
  public:
    DrvBTFBeam(const std::string&lab,VDeviceBase*parp):VDeviceDriver(lab,parp){}
    ~DrvBTFBeam(){}

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
    const int & GetBTFBeamHandle()const{return fBTFBeamHandle;}
    std::mutex& BTFBeamHandle_mutex(){return fBTFBeamHandle_mutex;}

  private:
    int fBTFBeamHandle; ///< This handle is to be used only for reading.
    std::mutex fBTFBeamHandle_mutex;///< mutex barrier for multithreading
    //std::lock_guard<std::mutex> guard(fBTFBeamHandle_mutex);
    std::string fIPAddress;
    std::string fPort;
   //std::string fUsername;
    //std::string fPassword;


};


#endif
