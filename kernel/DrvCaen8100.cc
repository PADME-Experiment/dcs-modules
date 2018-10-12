#include "DrvCaen8100.h"   //in c file
#include"fwk/utlCommaDashListToVector.h"
//#include <caenlib/CAENHVWrapper.h>
//#include "extern/CAENHVWrapper/CAENHVWrapper.h"
// #include "extern/CAEN_VME8100_1_0linux/CAENCrateToolBox/SDK/V8100SDK/V8100.h"
extern "C"{
#include"V8100.h"
}


#include<iostream>
#include <unistd.h>
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */
#include <stdio.h>
#include <string.h>

  DrvCaen8100_except&
DrvCaen8100_except::CAEN8100RetStatus(int retstatus, const std::string& add)
  {std::string error=(
		       retstatus==0     ? "Command wrapper correctly executed"                                            :
		       retstatus==1     ? "Error of operative system"                                                    :
		       retstatus==2     ? "Write error in communication channel"                                          :
		       retstatus==3     ? "Read error in communication channel"                                           :
		       retstatus==4     ? "Time out in server communication"                                              :
		       "Unknown");
		     
		     
    std::string str("CAEN8100 returned error ("+std::to_string(retstatus)+") : '" + "'  "+error);
    if(add.size()>0)str+="   "+add;

    switch (retstatus){
    case 1            : throw DrvCaen8100_except::Except_SYSERR               (str) ;break;
    case 2            : throw DrvCaen8100_except::Except_WRITEERR             (str) ;break;
    case 3            : throw DrvCaen8100_except::Except_READERR              (str) ;break;
    case 4            : throw DrvCaen8100_except::Except_TIMEERR              (str) ;break;
    }
  }


int DrvCaen8100::ComInit(V8100_data vme_handler)
{
  bool retbool;
  const char *ip = fIPAddress.c_str();
  // try to open the connection
  retbool=V8100_open_eth(&vme_handler,ip);
  if(retbool != true){
    DrvCaen8100_except::CAEN8100RetStatus(3,
					  "IP = "+fIPAddress);
    return 1;
  }
  // and close it
  V8100_close(&vme_handler);
  
  return 0;
}

void DrvCaen8100::ComDeinit(V8100_data handle)
{
  // not really necessary : we close the handle every time
  V8100_close(&handle);
}


void DrvCaen8100::AssertInit()
{
  INFO("DrvCaen8100::AssertInit()");
  //#warning CAEN ComInit
  //V8100_data vme_handler;
  ComInit(vme_handler);
  AssertInitAllOwned();
  //ProcessUpdateListTemp();
}

void DrvCaen8100::Finalize()
{
  INFO("DrvCaen8100::Finalize()");
  this->VDeviceBase::Finalize();
  //#warning CAEN ComDeinit
  // V8100_data vme_handler;
  ComDeinit(vme_handler);
  JoinThread();
}

void DrvCaen8100::OnCycleLocal()
{
  int ierrors = 0;
  CMD_ERROR_CODE err;
  bool retbool;
  // V8100_data vme_handler;

  // sleep 30 secs between one enquiry and the other
  sleep(30);

  const char *ip = fIPAddress.c_str();

  //  label retry open
retry_open:

  printf(" opening connection to %s \n",ip);
  // try to open the connection
  retbool=V8100_open_eth(&vme_handler,ip);
  if(retbool != true){
    ierrors++;
    if(ierrors<3) {
      //DrvCaen8100_except::CAEN8100RetStatus(3,"IP = "+fIPAddress);
     ERROR("DrvCaen8100  error reading socket ! retrying ..");
     printf(" retry opening the socket %d ...",ierrors);
     return;
     // goto retry_open;
    } else {
     DrvCaen8100_except::CAEN8100RetStatus(1,"IP = "+fIPAddress);
     printf(" Socket to %s closed \n",ip);
     // goto end_all;
     return; 
    }
  }


  UINT8 ui8;
  UINT32 ui32;
  const int length=100;
  V8100_channel_status chan_status;
  bool ret_bool;
  char mon_name[3][length];
  float vset[3];
  float vmin[3];
  float vmax[3];
  float vmon[3];
  float iset[3];
  float imin[3];
  float imax[3];
  float imon[3];
  

  for(UINT8 ch=0;ch<3;++ch){
    int ich=int(ch);
    // std::cout<<"vme8100 channel " << ich<<std::endl;
    
    ret_bool=V8100_mon_name(&vme_handler, ch, mon_name[ich], length, &err); 
    // std::cout<<"mon_name  "<<chararr<<"  "<<ret_bool<<std::endl;
    ret_bool=V8100_mon_vset(&vme_handler, ch, &vset[ich]             , &err); 
    //std::cout<<"mon_vset  "<<f      <<"  "<<ret_bool<<std::endl;
    ret_bool=V8100_mon_vmin(&vme_handler, ch, &vmin[ich]             , &err); 
    // std::cout<<"mon_vmin  "<<f      <<"  "<<ret_bool<<std::endl;
    ret_bool=V8100_mon_vmax(&vme_handler, ch, &vmax[ich]             , &err); 
    // std::cout<<"mon_vmax  "<<f      <<"  "<<ret_bool<<std::endl;
    //ret_bool=V8100_mon_vres(&vme_handler, ch, &f             , &err); std::cout<<"mon_vres  "<<f      <<"  "<<ret_bool<<std::endl;
    // ret_bool=V8100_mon_ovp (&vme_handler, ch, &f             , &err); std::cout<<"mon_ovp   "<<f      <<"  "<<ret_bool<<std::endl;
    //ret_bool=V8100_mon_uvp (&vme_handler, ch, &f             , &err); std::cout<<"mon_uvp   "<<f      <<"  "<<ret_bool<<std::endl;
    ret_bool=V8100_mon_vmon(&vme_handler, ch, &vmon[ich]             , &err); 
    // std::cout<<"mon_vmon  "<<f      <<"  "<<ret_bool<<std::endl;
    ret_bool=V8100_mon_iset(&vme_handler, ch, &iset[ich]             , &err); 
    // std::cout<<"mon_iset  "<<f      <<"  "<<ret_bool<<std::endl;
    ret_bool=V8100_mon_imin(&vme_handler, ch, &imin[ich]             , &err); 
    // std::cout<<"mon_imin  "<<f      <<"  "<<ret_bool<<std::endl;
    ret_bool=V8100_mon_imax(&vme_handler, ch, &imax[ich]             , &err); 
    // std::cout<<"mon_imax  "<<f      <<"  "<<ret_bool<<std::endl;
    // ret_bool=V8100_mon_ires(&vme_handler, ch, &f             , &err); std::cout<<"mon_ires  "<<f      <<"  "<<ret_bool<<std::endl;
    ret_bool=V8100_mon_imon(&vme_handler, ch, &imon[ich]             , &err); 
    // std::cout<<"mon_imon  "<<f      <<"  "<<ret_bool<<std::endl;
    // ret_bool=V8100_mon_stat(&vme_handler, ch, &chan_status,           , &err); std::cout<<"mon_stat  "<<chan_status<<"  "<<ret_bool<<std::endl;
    // std::cout<<std::endl;
  }
  // std::cout<<std::endl;

  V8100_RS232_BITRATE  rs_bitrate;
  V8100_CANBUS_BITRATE can_bitrate;
  V8100_FAN_SPEED_SET  fanspeed;
  V8100_crate_status crate_stat;

  UINT8 numch_8=3;
  float pstemp;
  UINT32 mon_fan1;
  UINT32 mon_fan2;
  UINT32 mon_fan3;
  float futemp;

  char crname[length];
  char ipaddr[length];
  char macaddr[length];

  ret_bool=V8100_mon_crname ( &vme_handler, crname,length, &err);
  // std::cout<<"mon_crname   "<<chararr       <<std::endl;
  
  // numch gives problem sometimes
  //ret_bool=V8100_mon_numch  ( &vme_handler, &numch_8          , &err);
  // std::cout<<"mon_numch    "<<+ui8          <<std::endl;
  // ret_bool=V8100_mon_psfrel ( &vme_handler, chararr,length, &err);std::cout<<"mon_psfrel   "<<chararr       <<std::endl;
  ret_bool=V8100_mon_pstemp ( &vme_handler, &pstemp            , &err);
  // std::cout<<"mon_pstemp   "<<f            <<std::endl;
  // ret_bool=V8100_mon_pssnum ( &vme_handler, &ui32         , &err);std::cout<<"mon_pssnum   "<<ui32         <<std::endl;
  ret_bool=V8100_mon_fansp  ( &vme_handler, &fanspeed     , &err);
  // ;std::cout<<"mon_fansp    "<<fanspeed     <<std::endl;
  ret_bool=V8100_mon_fan1   ( &vme_handler, &mon_fan1     , &err);
  // std::cout<<"mon_fan1     "<<ui32         <<std::endl;
  ret_bool=V8100_mon_fan2   ( &vme_handler, &mon_fan2     , &err);
  // std::cout<<"mon_fan2     "<<ui32         <<std::endl;
  ret_bool=V8100_mon_fan3   ( &vme_handler, &mon_fan3     , &err);
  // std::cout<<"mon_fan3     "<<ui32         <<std::endl;
  //ret_bool=V8100_mon_fufrel ( &vme_handler, chararr,length, &err);std::cout<<"mon_fufrel   "<<chararr       <<std::endl;
  ret_bool=V8100_mon_futemp ( &vme_handler, &futemp            , &err);
  // std::cout<<"mon_futemp   "<<f            <<std::endl;
  // ret_bool=V8100_mon_fusnum ( &vme_handler, &ui32         , &err);std::cout<<"mon_fusnum   "<<ui32         <<std::endl;
  // // ret_bool=V8100_mon_crst   ( &vme_handler, &crate_stat   , &err);std::cout<<"mon_crst     "<<crate_stat   <<std::endl;
  // ret_bool=V8100_mon_vpmax  ( &vme_handler, &f            , &err);std::cout<<"mon_vpmax    "<<f            <<std::endl;
  // ret_bool=V8100_mon_vpmin  ( &vme_handler, &f            , &err);std::cout<<"mon_vpmin    "<<f            <<std::endl;
  // ret_bool=V8100_mon_rs232br( &vme_handler, &rs_bitrate   , &err);std::cout<<"mon_rs232br  "<<rs_bitrate   <<std::endl;
  // ret_bool=V8100_mon_canbr  ( &vme_handler, &can_bitrate  , &err);std::cout<<"mon_canbr    "<<can_bitrate  <<std::endl;
  // ret_bool=V8100_mon_canadd ( &vme_handler, &ui8          , &err);std::cout<<"mon_canadd   "<<ui8          <<std::endl;
  ret_bool=V8100_mon_ipadd  ( &vme_handler, ipaddr,length, &err);
  // std::cout<<"mon_ipadd    "<<chararr       <<std::endl;
  // ret_bool=V8100_mon_ipmsk  ( &vme_handler, chararr,length, &err);std::cout<<"mon_ipmsk    "<<chararr       <<std::endl;
  // ret_bool=V8100_mon_ipgtw  ( &vme_handler, chararr,length, &err);std::cout<<"mon_ipgtw    "<<chararr       <<std::endl;
  ret_bool=V8100_mon_macadd ( &vme_handler, macaddr,length, &err);
  // std::cout<<"mon_macadd   "<<chararr       <<std::endl;

  printf(" closing connection to %s \n",ip);

  // and close the socket
  V8100_close(&vme_handler);

  time_t rawtime;
  struct tm * timeinfo;
  char *timestamp;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  //printf ( "The current date/time is: %s", asctime (timeinfo) );
  timestamp=asctime(timeinfo);
  int lengthstr=strlen(timestamp);
  //printf(" timestamp length = %d \n",lengthstr);
  timestamp[lengthstr]='\0';

  FILE * pFile;
  // int n;
  std::string filename="data/CAEN8100_";
  filename+=fIPAddress;

  pFile = fopen (filename.c_str(),"w");

  int numch=int(numch_8);
  // then print it out for debug
  printf(" Timestamp = %.24s \n",timestamp);
  printf(" VME CRATE %s - IP = %s - MAC = %s -numch = %d \n",crname,ipaddr,macaddr,numch);
  printf(" fanspeed = %d - mon_fan1=%u - mon_fan2=%u - mon_fan3=%u \n",fanspeed,mon_fan1,mon_fan2,mon_fan3);
  printf(" futemp= %f  - pstemp= %f \n",futemp,pstemp);

  fprintf(pFile,"%.24s;%s;%d;%d;%u;%u;%u;%f;%f\n",timestamp,ipaddr,numch,fanspeed,mon_fan1,mon_fan2,mon_fan3,futemp,pstemp);
  
  for(int nch=0;nch<numch;nch++){
    printf(" nch = %d - mon_name = %s \n",nch,mon_name[nch]);
    printf(" vset=%f - vmin=%f - vmax=%f - vmon=%f \n",vset[nch],vmin[nch],vmax[nch],vmon[nch]);
    printf(" iset=%f - imin=%f - imax=%f - imon=%f \n",iset[nch],imin[nch],imax[nch],imon[nch]);

    fprintf(pFile,"%d;%s;%f;%f;%f;%f;%f;%f;%f;%f \n",nch,mon_name[nch],vset[nch],vmin[nch],vmax[nch],vmon[nch],iset[nch],imin[nch],imax[nch],imon[nch]);
  }

  fclose(pFile);

  // and print to monitor fan speed and temp
   // and send value to file
  FILE * pFile3;
  std::string right="192.168.62.3";
  std::string left="192.168.62.4";
  std::string position="boh";
  if (fIPAddress.compare(right)==0)  position.assign("right");
  if (fIPAddress.compare(left)==0)   position.assign("left");
  std::string filename3="CAENVME";
  filename3+=position;
  filename3+=".txt";
  pFile3 = fopen (filename3.c_str(),"w");

  // write values to file
  fprintf(pFile3,"PLOTID CAEN_VME_%s \nPLOTNAME CAEN VME crate fan and temp \nPLOTTYPE text \n",position.c_str());
  fprintf(pFile3,"DATA  [ [\"Timestamp\",\"%.24s\"]",timestamp);
  fprintf(pFile3,",[\"IP address \",\"%s\"]",ipaddr);
  fprintf(pFile3,",[\"FAN setup speed \",\%d]",fanspeed);
  fprintf(pFile3,",[\"FAN1 speed \",\%u]",mon_fan1);
  fprintf(pFile3,",[\"FAN2 speed \",\%u]",mon_fan2);
  fprintf(pFile3,",[\"FAN3 speed \",\%u]",mon_fan3);
  fprintf(pFile3,",[\"FU temp \",\%.2f]",futemp);
  fprintf(pFile3,",[\"PS temp \",\%.2f] ]\n",pstemp);
  fclose(pFile3);

  // and copy file to monitor@l0padme3
  std::string scp2="scp -q ";
  scp2+=filename3;
  scp2+=" monitor@l0padme3:PadmeMonitor/watchdir/. ";
  // cout << " scp command " << scp2 << endl; 
  char * writable2 = new char[scp2.size() + 1];
  std::copy(scp2.begin(), scp2.end(), writable2);
  writable2[scp2.size()] = '\0'; // don't forget the terminating 0
  // scp to monitor@l0padme3
  system(writable2);
  

}
