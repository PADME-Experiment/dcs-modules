#ifndef  _DCS_kernel_drv_DrvMimosaT_h_
#define  _DCS_kernel_drv_DrvT_h_ 1
//#include "DrvMimosaT.h"   //in c file

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



class DrvMimosaT_except:public fwk::Exception{
  public:
    class Except_OK                   ;
    class Except_SYSERR               ;
    class Except_WRITEERR             ;
    class Except_READERR              ;
    class Except_TIMEERR              ;
    class Except_DOWN                 ;
    class Except_NOTPRES              ;
  public:

    //DrvMimosaT_except(){fType="DrvMimosaT_except";}
    static DrvMimosaT_except& MimosaTRetStatus(int i,int retstat,const std::string& add=std::string());
    DrvMimosaT_except(const std::string&a):fwk::Exception(a){}
    //virtual ~DrvMimosaT_except(){}
};



class DrvMimosaT_except::Except_OK                   : public DrvMimosaT_except {public: Except_OK                  (const std::string&s):DrvMimosaT_except(s){ fwk::Exception::SetType(*this); }; };
class DrvMimosaT_except::Except_SYSERR               : public DrvMimosaT_except {public: Except_SYSERR              (const std::string&s):DrvMimosaT_except(s){ fwk::Exception::SetType(*this); }; };
class DrvMimosaT_except::Except_WRITEERR             : public DrvMimosaT_except {public: Except_WRITEERR            (const std::string&s):DrvMimosaT_except(s){ fwk::Exception::SetType(*this); }; };
class DrvMimosaT_except::Except_READERR              : public DrvMimosaT_except {public: Except_READERR             (const std::string&s):DrvMimosaT_except(s){ fwk::Exception::SetType(*this); }; };
class DrvMimosaT_except::Except_TIMEERR              : public DrvMimosaT_except {public: Except_TIMEERR             (const std::string&s):DrvMimosaT_except(s){ fwk::Exception::SetType(*this); }; };
class DrvMimosaT_except::Except_DOWN                 : public DrvMimosaT_except {public: Except_DOWN                (const std::string&s):DrvMimosaT_except(s){ fwk::Exception::SetType(*this); }; };
class DrvMimosaT_except::Except_NOTPRES              : public DrvMimosaT_except {public: Except_NOTPRES             (const std::string&s):DrvMimosaT_except(s){ fwk::Exception::SetType(*this); }; };



class DrvMimosaT:public VDeviceDriver{
  public:
    DrvMimosaT(const std::string&lab,VDeviceBase*parp):VDeviceDriver(lab,parp){}
    ~DrvMimosaT(){}

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
    const int & GetMimosaTHandle()const{return fMimosaTHandle;}
    std::mutex& MimosaTHandle_mutex(){return fMimosaTHandle_mutex;}

  private:
    int fMimosaTHandle; ///< This handle is to be used only for reading.
    std::mutex fMimosaTHandle_mutex;///< mutex barrier for multithreading
    //std::lock_guard<std::mutex> guard(fMimosaTHandle_mutex);
    std::string fIPAddress;
    std::string fPort;
    //std::string fUsername;
    //std::string fPassword;
    
};

#endif
