#ifndef  _DCS_kernel_drv_DrvEcalT_h_
#define  _DCS_kernel_drv_DrvEcalT_h_ 1
//#include "DrvEcalT.h"   //in c file

#include"VDeviceDriver.h"
// #include "VCaenHVBoard.h"


#include "fwk/fwkException.h"
#include "fwk/utlMessageBus.h"

#include<vector>
#include<memory>
#include<cstdint>
#include<sstream>
#include<mutex>
#include <string.h>



class DrvEcalT_except:public fwk::Exception{
  public:
    class Except_OK                   ;
    class Except_SYSERR               ;
    class Except_WRITEERR             ;
    class Except_READERR              ;
    class Except_TIMEERR              ;
    class Except_DOWN                 ;
    class Except_NOTPRES              ;
  public:

    //DrvEcalT_except(){fType="DrvEcalT_except";}
    static DrvEcalT_except& EcalTRetStatus(int i,int retstat,const std::string& add=std::string());
    DrvEcalT_except(const std::string&a):fwk::Exception(a){}
    //virtual ~DrvEcalT_except(){}
};



class DrvEcalT_except::Except_OK                   : public DrvEcalT_except {public: Except_OK                  (const std::string&s):DrvEcalT_except(s){ fwk::Exception::SetType(*this); }; };
class DrvEcalT_except::Except_SYSERR               : public DrvEcalT_except {public: Except_SYSERR              (const std::string&s):DrvEcalT_except(s){ fwk::Exception::SetType(*this); }; };
class DrvEcalT_except::Except_WRITEERR             : public DrvEcalT_except {public: Except_WRITEERR            (const std::string&s):DrvEcalT_except(s){ fwk::Exception::SetType(*this); }; };
class DrvEcalT_except::Except_READERR              : public DrvEcalT_except {public: Except_READERR             (const std::string&s):DrvEcalT_except(s){ fwk::Exception::SetType(*this); }; };
class DrvEcalT_except::Except_TIMEERR              : public DrvEcalT_except {public: Except_TIMEERR             (const std::string&s):DrvEcalT_except(s){ fwk::Exception::SetType(*this); }; };
class DrvEcalT_except::Except_DOWN                 : public DrvEcalT_except {public: Except_DOWN                (const std::string&s):DrvEcalT_except(s){ fwk::Exception::SetType(*this); }; };
class DrvEcalT_except::Except_NOTPRES              : public DrvEcalT_except {public: Except_NOTPRES             (const std::string&s):DrvEcalT_except(s){ fwk::Exception::SetType(*this); }; };



class DrvEcalT:public VDeviceDriver{
  public:
    DrvEcalT(const std::string&lab,VDeviceBase*parp):VDeviceDriver(lab,parp){}
    ~DrvEcalT(){}

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
    
    int WriteString(int,std::string);
    int WaitForData(int);
    int ReadString(int,char*);

    void DeviceClear(int,char*);
    void SetNODELAY(int);
 
  public:
    const int & GetEcalTHandle()const{return fEcalTHandle;}
    std::mutex& EcalTHandle_mutex(){return fEcalTHandle_mutex;}

  private:
    int fEcalTHandle; ///< This handle is to be used only for reading.
    std::mutex fEcalTHandle_mutex;///< mutex barrier for multithreading
    //std::lock_guard<std::mutex> guard(fEcalTHandle_mutex);
    std::string fIPAddress;
    std::string fPort;
    //std::string fUsername;
    //std::string fPassword;
    
};

#endif
