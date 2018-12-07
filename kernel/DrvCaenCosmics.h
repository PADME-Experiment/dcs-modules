#ifndef  _DCS_kernel_drv_DrvCaenCosmics_h_
#define  _DCS_kernel_drv_DrvCaenCosmics_h_ 1
#include "DrvCaenCosmics.h"   //in c file

#include"VDeviceDriver.h"
#include "VCaenHVBoard.h"


#include "fwk/fwkException.h"
#include "fwk/utlMessageBus.h"

#include<vector>
#include<memory>
#include<cstdint>
#include<sstream>
#include<mutex>
class DrvCaenCosmics_except:public fwk::Exception{
  public:
    class Except_OK                   ;
    class Except_SYSERR               ;
    class Except_WRITEERR             ;
    class Except_READERR              ;
    class Except_TIMEERR              ;
    class Except_DOWN                 ;
    class Except_NOTPRES              ;
    class Except_SLOTNOTPRES          ;
    class Except_NOSERIAL             ;
    class Except_MEMORYFAULT          ;
    class Except_OUTOFRANGE           ;
    class Except_EXECCOMNOTIMPL       ;
    class Except_GETPROPNOTIMPL       ;
    class Except_SETPROPNOTIMPL       ;
    class Except_PROPNOTFOUND         ;
    class Except_EXECNOTFOUND         ;
    class Except_NOTSYSPROP           ;
    class Except_NOTGETPROP           ;
    class Except_NOTSETPROP           ;
    class Except_NOTEXECOMM           ;
    class Except_SYSCONFCHANGE        ;
    class Except_PARAMPROPNOTFOUND    ;
    class Except_PARAMNOTFOUND        ;
    class Except_NODATA               ;
    class Except_DEVALREADYOPEN       ;
    class Except_TOOMANYDEVICEOPEN    ;
    class Except_INVALIDPARAMETER     ;
    class Except_FUNCTIONNOTAVAILABLE ;
    class Except_SOCKETERROR          ;
    class Except_COMMUNICATIONERROR   ;
    class Except_NOTYETIMPLEMENTED    ;
    class Except_CONNECTED            ;
    class Except_NOTCONNECTED         ;
    class Except_OS                   ;
    class Except_LOGINFAILED          ;
    class Except_LOGOUTFAILED         ;
    class Except_LINKNOTSUPPORTED     ;
    class Except_USERPASSFAILED       ;
  public:

    //DrvCaenCosmics_except(){fType="DrvCaenCosmics_except";}
    static DrvCaenCosmics_except& CAENWrapperRetStatus(int i,int retstat,const std::string& add=std::string());
    DrvCaenCosmics_except(const std::string&a):fwk::Exception(a){}
    //virtual ~DrvCaenCosmics_except(){}
};



class DrvCaenCosmics_except::Except_OK                   : public DrvCaenCosmics_except {public: Except_OK                  (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_SYSERR               : public DrvCaenCosmics_except {public: Except_SYSERR              (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_WRITEERR             : public DrvCaenCosmics_except {public: Except_WRITEERR            (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_READERR              : public DrvCaenCosmics_except {public: Except_READERR             (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_TIMEERR              : public DrvCaenCosmics_except {public: Except_TIMEERR             (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_DOWN                 : public DrvCaenCosmics_except {public: Except_DOWN                (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_NOTPRES              : public DrvCaenCosmics_except {public: Except_NOTPRES             (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_SLOTNOTPRES          : public DrvCaenCosmics_except {public: Except_SLOTNOTPRES         (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_NOSERIAL             : public DrvCaenCosmics_except {public: Except_NOSERIAL            (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_MEMORYFAULT          : public DrvCaenCosmics_except {public: Except_MEMORYFAULT         (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_OUTOFRANGE           : public DrvCaenCosmics_except {public: Except_OUTOFRANGE          (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_EXECCOMNOTIMPL       : public DrvCaenCosmics_except {public: Except_EXECCOMNOTIMPL      (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_GETPROPNOTIMPL       : public DrvCaenCosmics_except {public: Except_GETPROPNOTIMPL      (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_SETPROPNOTIMPL       : public DrvCaenCosmics_except {public: Except_SETPROPNOTIMPL      (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_PROPNOTFOUND         : public DrvCaenCosmics_except {public: Except_PROPNOTFOUND        (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_EXECNOTFOUND         : public DrvCaenCosmics_except {public: Except_EXECNOTFOUND        (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_NOTSYSPROP           : public DrvCaenCosmics_except {public: Except_NOTSYSPROP          (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_NOTGETPROP           : public DrvCaenCosmics_except {public: Except_NOTGETPROP          (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_NOTSETPROP           : public DrvCaenCosmics_except {public: Except_NOTSETPROP          (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_NOTEXECOMM           : public DrvCaenCosmics_except {public: Except_NOTEXECOMM          (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_SYSCONFCHANGE        : public DrvCaenCosmics_except {public: Except_SYSCONFCHANGE       (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_PARAMPROPNOTFOUND    : public DrvCaenCosmics_except {public: Except_PARAMPROPNOTFOUND   (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_PARAMNOTFOUND        : public DrvCaenCosmics_except {public: Except_PARAMNOTFOUND       (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_NODATA               : public DrvCaenCosmics_except {public: Except_NODATA              (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_DEVALREADYOPEN       : public DrvCaenCosmics_except {public: Except_DEVALREADYOPEN      (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_TOOMANYDEVICEOPEN    : public DrvCaenCosmics_except {public: Except_TOOMANYDEVICEOPEN   (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_INVALIDPARAMETER     : public DrvCaenCosmics_except {public: Except_INVALIDPARAMETER    (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_FUNCTIONNOTAVAILABLE : public DrvCaenCosmics_except {public: Except_FUNCTIONNOTAVAILABLE(const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_SOCKETERROR          : public DrvCaenCosmics_except {public: Except_SOCKETERROR         (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_COMMUNICATIONERROR   : public DrvCaenCosmics_except {public: Except_COMMUNICATIONERROR  (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_NOTYETIMPLEMENTED    : public DrvCaenCosmics_except {public: Except_NOTYETIMPLEMENTED   (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_CONNECTED            : public DrvCaenCosmics_except {public: Except_CONNECTED           (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_NOTCONNECTED         : public DrvCaenCosmics_except {public: Except_NOTCONNECTED        (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_OS                   : public DrvCaenCosmics_except {public: Except_OS                  (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_LOGINFAILED          : public DrvCaenCosmics_except {public: Except_LOGINFAILED         (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_LOGOUTFAILED         : public DrvCaenCosmics_except {public: Except_LOGOUTFAILED        (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_LINKNOTSUPPORTED     : public DrvCaenCosmics_except {public: Except_LINKNOTSUPPORTED    (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };
class DrvCaenCosmics_except::Except_USERPASSFAILED       : public DrvCaenCosmics_except {public: Except_USERPASSFAILED      (const std::string&s):DrvCaenCosmics_except(s){ fwk::Exception::SetType(*this); }; };









class DrvCaenCosmics:public VDeviceDriver{
  public:
    DrvCaenCosmics(const std::string&lab,VDeviceBase*parp):VDeviceDriver(lab,parp){}
    ~DrvCaenCosmics(){}
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
    const int & GetCaenCrateHandle()const{return fCaenCrateHandle;}
    std::mutex& GetCaenCrateHandle_mutex(){return fCaenCrateHandle_mutex;}

  private:
    int fCaenCrateHandle; ///< This handle is to be used only for reading.
    std::mutex fCaenCrateHandle_mutex;///< mutex barrier for multithreading
    //std::lock_guard<std::mutex> guard(fCaenCrateHandle_mutex);
    std::string fIPAddress;
    std::string fUsername;
    std::string fPassword;
};


#endif
