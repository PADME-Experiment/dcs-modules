#include "DrvCaen8301.h"   //in c file
#include"fwk/utlCommaDashListToVector.h"
//#include <caenlib/CAENHVWrapper.h>
//#include "extern/CAENHVWrapper/CAENHVWrapper.h"
// #include "extern/CAEN_VME8301_1_0linux/CAENCrateToolBox/SDK/V8301SDK/V8301.h"
extern "C"{
#include"V8301.h"
}

#include<iostream>
#include <unistd.h>
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */
#include <stdio.h>
#include <string.h>

  DrvCaen8301_except&
DrvCaen8301_except::CAEN8301RetStatus(int retstatus, const std::string& add)
  {std::string error=(
		       retstatus==0     ? "Command wrapper correctly executed"                                            :
		       retstatus==1     ? "Error of operative system"                                                    :
		       retstatus==2     ? "Write error in communication channel"                                          :
		       retstatus==3     ? "Read error in communication channel"                                           :
		       retstatus==4     ? "Time out in server communication"                                              :
		       "Unknown");
		     
		     
    std::string str("CAEN8301 returned error ("+std::to_string(retstatus)+") : '" + "'  "+error);
    if(add.size()>0)str+="   "+add;

    switch (retstatus){
    case 1            : throw DrvCaen8301_except::Except_SYSERR               (str) ;break;
    case 2            : throw DrvCaen8301_except::Except_WRITEERR             (str) ;break;
    case 3            : throw DrvCaen8301_except::Except_READERR              (str) ;break;
    case 4            : throw DrvCaen8301_except::Except_TIMEERR              (str) ;break;
    }
  }


int DrvCaen8301::ComInit(V8301_data vme_handler)
{
  bool retbool;
  const char *ip = fIPAddress.c_str();
  // try to open the connection
  retbool=V8301_open_eth(&vme_handler,ip);
  if(retbool != true){
    DrvCaen8301_except::CAEN8301RetStatus(1,
					  "IP = "+fIPAddress);
    return 1;
  }
  // and close it
  V8301_close(&vme_handler);
  
  return 0;
}

void DrvCaen8301::ComDeinit(V8301_data handle)
{
  // not really necessary : we close the handle every time
  V8301_close(&handle);
}


void DrvCaen8301::AssertInit()
{
  INFO("DrvCaen8301::AssertInit()");
  //#warning CAEN ComInit
  //V8301_data vme_handler;
  ComInit(vme_handler);
  AssertInitAllOwned();
  //ProcessUpdateListTemp();
}

void DrvCaen8301::Finalize()
{
  INFO("DrvCaen8301::Finalize()");
  this->VDeviceBase::Finalize();
  //#warning CAEN ComDeinit
  ComDeinit(vme_handler);
  JoinThread();
}

void DrvCaen8301::OnCycleLocal()
{
  CMD_ERROR_CODE err;
  bool retbool;

  // sleep 30 secs between one enquiry and the other
  sleep(30);

  const char *ip = fIPAddress.c_str();
  printf(" opening connection to %s \n",ip);
  // try to open the connection
  retbool=V8301_open_eth(&vme_handler,ip);
  if(retbool != true){
    DrvCaen8301_except::CAEN8301RetStatus(1,
					  "IP = "+fIPAddress);
  }

  time_t rawtime;
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );

  // old human readable timestamp
  //char *timestamp;
  //timestamp=asctime(timeinfo);

  // new formatted timestring
  char timestamp[25];
  strftime (timestamp,25,"%Y-%m-%d %T",timeinfo);
 
  int lengthstr=strlen(timestamp);
  //printf(" timestamp length = %d \n",lengthstr);
  timestamp[lengthstr]='\0';

  UINT8 ui8;
  UINT32 ui32;
  const int length=100;
  V8301_channel_status chan_status;
  bool ret_bool;
  char mon_name[6][length];
  //float vset[6];
  //float vmin[6];
  //float vmax[6];
  float vmon[6]={0.,0.,0.,0.,0.,0.};
  float iset[6]={0.,0.,0.,0.,0.,0.};
  float imin[6]={0.,0.,0.,0.,0.,0.};
  float imax[6]={0.,0.,0.,0.,0.,0.};
  float imon[6]={0.,0.,0.,0.,0.,0.};
  int numch = 6;

  for(UINT8 ch=0;ch<numch;++ch){
    int ich=int(ch);
    // std::cout<<"vme8301 channel " << ich<<std::endl;
    
    ret_bool=V8301_mon_name(&vme_handler, ch, mon_name[ich], length, &err); 
    // std::cout<<"mon_name  "<<chararr<<"  "<<ret_bool<<std::endl;
    // ret_bool=V8301_mon_vset(&vme_handler, ch, &vset[ich]             , &err); 
    //std::cout<<"mon_vset  "<<f      <<"  "<<ret_bool<<std::endl;
    //ret_bool=V8301_mon_vmin(&vme_handler, ch, &vmin[ich]             , &err); 
    // std::cout<<"mon_vmin  "<<f      <<"  "<<ret_bool<<std::endl;
    //ret_bool=V8301_mon_vmax(&vme_handler, ch, &vmax[ich]             , &err); 
    // std::cout<<"mon_vmax  "<<f      <<"  "<<ret_bool<<std::endl;
    //ret_bool=V8301_mon_vres(&vme_handler, ch, &f             , &err); std::cout<<"mon_vres  "<<f      <<"  "<<ret_bool<<std::endl;
    // ret_bool=V8301_mon_ovp (&vme_handler, ch, &f             , &err); std::cout<<"mon_ovp   "<<f      <<"  "<<ret_bool<<std::endl;
    //ret_bool=V8301_mon_uvp (&vme_handler, ch, &f             , &err); std::cout<<"mon_uvp   "<<f      <<"  "<<ret_bool<<std::endl;
    ret_bool=V8301_mon_vmon(&vme_handler, ch, &vmon[ich]             , &err); 
    // std::cout<<"mon_vmon  "<<f      <<"  "<<ret_bool<<std::endl;
    ret_bool=V8301_mon_iset(&vme_handler, ch, &iset[ich]             , &err); 
    // std::cout<<"mon_iset  "<<f      <<"  "<<ret_bool<<std::endl;
    ret_bool=V8301_mon_imin(&vme_handler, ch, &imin[ich]             , &err); 
    // std::cout<<"mon_imin  "<<f      <<"  "<<ret_bool<<std::endl;
    ret_bool=V8301_mon_imax(&vme_handler, ch, &imax[ich]             , &err); 
    // std::cout<<"mon_imax  "<<f      <<"  "<<ret_bool<<std::endl;
    // ret_bool=V8301_mon_ires(&vme_handler, ch, &f             , &err); std::cout<<"mon_ires  "<<f      <<"  "<<ret_bool<<std::endl;
    ret_bool=V8301_mon_imon(&vme_handler, ch, &imon[ich]             , &err); 
    // std::cout<<"mon_imon  "<<f      <<"  "<<ret_bool<<std::endl;
    // ret_bool=V8301_mon_stat(&vme_handler, ch, &chan_status,           , &err); std::cout<<"mon_stat  "<<chan_status<<"  "<<ret_bool<<std::endl;
    // std::cout<<std::endl;
  }
  // std::cout<<std::endl;

  V8301_RS232_BITRATE  rs_bitrate;
  V8301_CANBUS_BITRATE can_bitrate;
  V8301_FAN_SPEED_SET  fanspeed;
  V8301_crate_status crate_stat;

  //UINT8 numch_8;
  float pstemp;
  UINT32 mon_fan1;
  UINT32 mon_fan2;
  UINT32 mon_fan3;
  float futemp;

  char crname[length];
  char ipaddr[length];
  char macaddr[length];

  ret_bool=V8301_mon_crname ( &vme_handler, crname,length, &err);
  // std::cout<<"mon_crname   "<<chararr       <<std::endl;
  //ret_bool=V8301_mon_numch  ( &vme_handler, &numch_8          , &err);
  // std::cout<<"mon_numch    "<<+ui8          <<std::endl;
  // ret_bool=V8301_mon_psfrel ( &vme_handler, chararr,length, &err);std::cout<<"mon_psfrel   "<<chararr       <<std::endl;
  ret_bool=V8301_mon_pstemp ( &vme_handler, &pstemp            , &err);
  // std::cout<<"mon_pstemp   "<<f            <<std::endl;
  // ret_bool=V8301_mon_pssnum ( &vme_handler, &ui32         , &err);std::cout<<"mon_pssnum   "<<ui32         <<std::endl;
  ret_bool=V8301_mon_fansp  ( &vme_handler, &fanspeed     , &err);
  // ;std::cout<<"mon_fansp    "<<fanspeed     <<std::endl;
  ret_bool=V8301_mon_fan1   ( &vme_handler, &mon_fan1     , &err);
  // std::cout<<"mon_fan1     "<<ui32         <<std::endl;
  ret_bool=V8301_mon_fan2   ( &vme_handler, &mon_fan2     , &err);
  // std::cout<<"mon_fan2     "<<ui32         <<std::endl;
  ret_bool=V8301_mon_fan3   ( &vme_handler, &mon_fan3     , &err);
  // std::cout<<"mon_fan3     "<<ui32         <<std::endl;
  //ret_bool=V8301_mon_fufrel ( &vme_handler, chararr,length, &err);std::cout<<"mon_fufrel   "<<chararr       <<std::endl;
  ret_bool=V8301_mon_futemp ( &vme_handler, &futemp            , &err);
  // std::cout<<"mon_futemp   "<<f            <<std::endl;
  // ret_bool=V8301_mon_fusnum ( &vme_handler, &ui32         , &err);std::cout<<"mon_fusnum   "<<ui32         <<std::endl;
  // // ret_bool=V8301_mon_crst   ( &vme_handler, &crate_stat   , &err);std::cout<<"mon_crst     "<<crate_stat   <<std::endl;
  // ret_bool=V8301_mon_vpmax  ( &vme_handler, &f            , &err);std::cout<<"mon_vpmax    "<<f            <<std::endl;
  // ret_bool=V8301_mon_vpmin  ( &vme_handler, &f            , &err);std::cout<<"mon_vpmin    "<<f            <<std::endl;
  // ret_bool=V8301_mon_rs232br( &vme_handler, &rs_bitrate   , &err);std::cout<<"mon_rs232br  "<<rs_bitrate   <<std::endl;
  // ret_bool=V8301_mon_canbr  ( &vme_handler, &can_bitrate  , &err);std::cout<<"mon_canbr    "<<can_bitrate  <<std::endl;
  // ret_bool=V8301_mon_canadd ( &vme_handler, &ui8          , &err);std::cout<<"mon_canadd   "<<ui8          <<std::endl;
  ret_bool=V8301_mon_ipadd  ( &vme_handler, ipaddr,length, &err);
  // std::cout<<"mon_ipadd    "<<chararr       <<std::endl;
  // ret_bool=V8301_mon_ipmsk  ( &vme_handler, chararr,length, &err);std::cout<<"mon_ipmsk    "<<chararr       <<std::endl;
  // ret_bool=V8301_mon_ipgtw  ( &vme_handler, chararr,length, &err);std::cout<<"mon_ipgtw    "<<chararr       <<std::endl;
  ret_bool=V8301_mon_macadd ( &vme_handler, macaddr,length, &err);
  // std::cout<<"mon_macadd   "<<chararr       <<std::endl;

  printf(" closing connection to %s \n",ip);
  V8301_close(&vme_handler);

  FILE * pFile;
  // int n;
  std::string filename="data/CAEN8301_";
  filename+=fIPAddress;

  pFile = fopen (filename.c_str(),"w");


  // int numch=int(numch_8);
  // then print it out for debug
  /*
  printf(" Timestamp = %.25s \n",timestamp);
  printf(" NIM CRATE %s - IP = %s - MAC = %s -numch = %d \n",crname,ipaddr,macaddr,numch);
  printf(" fanspeed = %d - mon_fan1=%u - mon_fan2=%u - mon_fan3=%u \n",fanspeed,mon_fan1,mon_fan2,mon_fan3);
  printf(" futemp= %f  - pstemp= %f \n",futemp,pstemp);
  */
  
  fprintf(pFile,"%.24s;%d;%d;%u;%u;%u;%f;%f\n",timestamp,numch,fanspeed,mon_fan1,mon_fan2,mon_fan3,futemp,pstemp);


  for(int nch=0;nch<numch;nch++){
    /*
    printf(" nch = %d - mon_name = %s \n",nch,mon_name[nch]);
    printf(" vmon=%f \n",vmon[nch]);
    printf(" iset=%f - imin=%f - imax=%f - imon=%f \n",iset[nch],imin[nch],imax[nch],imon[nch]);
    */
    fprintf(pFile,"%d;%s;%f;%f;%f;%f;%f \n",nch,mon_name[nch],vmon[nch],iset[nch],imin[nch],imax[nch],imon[nch]);
  }
  fclose(pFile);

  // and print to monitor fan speed and temp
   // and send value to file
  FILE * pFile3;
  std::string filename3="monitor/CAENNIM";

  //default is right nim crate
  std::string right="192.168.62.20";
  std::string newcrate="192.168.62.22";
  std::string position="boh";
  if (fIPAddress.compare(right)==0)  position.assign("right");
  if (fIPAddress.compare(newcrate)==0)   position.assign("new");
 
  filename3+=position;
  filename3+=".txt";
  pFile3 = fopen (filename3.c_str(),"w");

  // write values to file
  fprintf(pFile3,"PLOTID CAEN_NIM_%s \nPLOTNAME CAEN NIM %s crate fan and temp \nPLOTTYPE activetext \n",position.c_str(),position.c_str());
  fprintf(pFile3,"DATA  [ {\"title\":\"Timestamp\",\"current\":{\"value\":\"%.24s\"}}",timestamp);
  fprintf(pFile3,",{\"title\":\"IP address \",\"current\":{\"value\":\"%s\"}}",ipaddr);
  fprintf(pFile3,",{\"title\":\"MAC address \",\"current\":{\"value\":\"%s\"}}",macaddr);
  fprintf(pFile3,",{\"title\":\"FAN setup speed \",\"current\":{\"value\":\"\%d\"}}",fanspeed);
  fprintf(pFile3,",{\"title\":\"FAN1 speed \",\"warn\":{\"min\":\"1900\"},\"alarm\":{\"min\":\"1400\"},\"current\":{\"value\":\"%u\",\"col\":\"#00CC00\"}}",mon_fan1);
  fprintf(pFile3,",{\"title\":\"FAN2 speed \",\"warn\":{\"min\":\"1900\"},\"alarm\":{\"min\":\"1400\"},\"current\":{\"value\":\"%u\",\"col\":\"#00CC00\"}}",mon_fan2);
  fprintf(pFile3,",{\"title\":\"FAN3 speed \",\"warn\":{\"min\":\"1900\"},\"alarm\":{\"min\":\"1400\"},\"current\":{\"value\":\"%u\",\"col\":\"#00CC00\"}}",mon_fan3);
  fprintf(pFile3,",{\"title\":\"FU temp\",\"warn\":{\"max\":\"30\"},\"alarm\":{\"max\":\"45\"},\"current\":{\"value\":\"%.2f\",\"col\":\"#00CC00\"}}",futemp);
  fprintf(pFile3,",{\"title\":\"PS temp\",\"warn\":{\"max\":\"40\"},\"alarm\":{\"max\":\"50\"},\"current\":{\"value\":\"%.2f\",\"col\":\"#00CC00\"}} ]\n",pstemp);
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
