#ifndef  _DCS_kernel_drv_DrvPAdmeAmb_h_
#define  _DCS_kernel_drv_DrvPadmeAmb_h_ 1
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

class DrvBTFAmb_except:public fwk::Exception{
  public:
    class Except_OK                   ;
    class Except_SYSERR               ;
    class Except_WRITEERR             ;
    class Except_READERR              ;
    class Except_TIMEERR              ;
    class Except_DOWN                 ;
    class Except_NOTPRES              ;
  public:

    //DrvBTFAmb_except(){fType="DrvBTFAmb_except";}
    static DrvBTFAmb_except& BTFAmbRetStatus(int i,int retstat,const std::string& add=std::string());
    DrvBTFAmb_except(const std::string&a):fwk::Exception(a){}
    //virtual ~DrvBTFAmb_except(){}
};



class DrvBTFAmb_except::Except_OK                   : public DrvBTFAmb_except {public: Except_OK                  (const std::string&s):DrvBTFAmb_except(s){ fwk::Exception::SetType(*this); }; };
class DrvBTFAmb_except::Except_SYSERR               : public DrvBTFAmb_except {public: Except_SYSERR              (const std::string&s):DrvBTFAmb_except(s){ fwk::Exception::SetType(*this); }; };
class DrvBTFAmb_except::Except_WRITEERR             : public DrvBTFAmb_except {public: Except_WRITEERR            (const std::string&s):DrvBTFAmb_except(s){ fwk::Exception::SetType(*this); }; };
class DrvBTFAmb_except::Except_READERR              : public DrvBTFAmb_except {public: Except_READERR             (const std::string&s):DrvBTFAmb_except(s){ fwk::Exception::SetType(*this); }; };
class DrvBTFAmb_except::Except_TIMEERR              : public DrvBTFAmb_except {public: Except_TIMEERR             (const std::string&s):DrvBTFAmb_except(s){ fwk::Exception::SetType(*this); }; };
class DrvBTFAmb_except::Except_DOWN                 : public DrvBTFAmb_except {public: Except_DOWN                (const std::string&s):DrvBTFAmb_except(s){ fwk::Exception::SetType(*this); }; };
class DrvBTFAmb_except::Except_NOTPRES              : public DrvBTFAmb_except {public: Except_NOTPRES             (const std::string&s):DrvBTFAmb_except(s){ fwk::Exception::SetType(*this); }; };








class DrvBTFAmb:public VDeviceDriver{
  public:
    DrvBTFAmb(const std::string&lab,VDeviceBase*parp):VDeviceDriver(lab,parp){}
    ~DrvBTFAmb(){}

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
    const int & GetBTFAmbHandle()const{return fBTFAmbHandle;}
    std::mutex& BTFAmbHandle_mutex(){return fBTFAmbHandle_mutex;}

  private:
    int fBTFAmbHandle; ///< This handle is to be used only for reading.
    std::mutex fBTFAmbHandle_mutex;///< mutex barrier for multithreading
    //std::lock_guard<std::mutex> guard(fBTFAmbHandle_mutex);
    std::string fIPAddress;
    std::string fPort;
    //std::string fUsername;
    //std::string fPassword;


};


#endif
