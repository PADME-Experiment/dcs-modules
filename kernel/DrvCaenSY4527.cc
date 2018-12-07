#include "DrvCaenSY4527.h"   //in c file
#include"fwk/utlCommaDashListToVector.h"
//#include <caenlib/CAENHVWrapper.h>
#include "extern/CAENHVWrapper/CAENHVWrapper.h"


  int
DrvCaenSY4527::ComInit()
{
  CAENHVRESULT ret;
  int handle;
  CAENHV_SYSTEM_TYPE_t sys_type = (CAENHV_SYSTEM_TYPE_t) SY4527; // 0: SY1527, 2: SY4527
  int link_type = LINKTYPE_TCPIP;
  ret = CAENHV_InitSystem(sys_type, link_type, (void*)fIPAddress.c_str(), fUsername.c_str(), fPassword.c_str(), &handle);
  if(ret != CAENHV_OK){
    DrvCaenHV_except::CAENWrapperRetStatus(handle,ret,
        "IP = "+fIPAddress+"  user = "+fUsername+"  pass = "+fPassword);
  }
  
  // and close the handle
  ret = CAENHV_DeinitSystem(handle);
  if(ret != CAENHV_OK)
    DrvCaenHV_except::CAENWrapperRetStatus(handle,ret);
  return handle;
}

  void
DrvCaenSY4527::ComDeinit(int handle)
{
  int ret = CAENHV_DeinitSystem(handle);
  if(ret != CAENHV_OK)
    DrvCaenHV_except::CAENWrapperRetStatus(handle,ret);
}


void
DrvCaenSY4527::AssertInit()
{
  INFO("DrvCaenSY4527::AssertInit()");
  //#warning CAEN ComInit
  fCaenCrateHandle=ComInit();
  AssertInitAllOwned();
  //ProcessUpdateListTemp();
}

  void
DrvCaenSY4527::Finalize()
{
  INFO("DrvCaenSY4527::Finalize()");
  this->VDeviceBase::Finalize();
  //#warning CAEN ComDeinit
  ComDeinit(fCaenCrateHandle );
  JoinThread();
}


void
DrvCaenSY4527::OnCycleLocal()
{
  int handle;
  char timestring[27];

  CAENHVRESULT  ret,ret1,ret2,ret3;
  int           sysHndl=-1;
  // char* address = "192.168.62.10";
  int link_type = 0;  // TCP/IP
   
  
  ret = CAENHV_InitSystem((CAENHV_SYSTEM_TYPE_t)2, link_type,(void*)fIPAddress.c_str(), fUsername.c_str(), fPassword.c_str(), &sysHndl);
  
  if( ret == CAENHV_OK ){
    // printf("\n CAEN system online - handle = %d \n\n",sysHndl);
  } else {
    DrvCaenHV_except::CAENWrapperRetStatus(sysHndl,ret,
					   "IP = "+fIPAddress+"  user = "+fUsername+"  pass = "+fPassword);
    printf("\n CAEN HV connect error - No connection \n");
    return;
  }
  
  // end of login //

  // get timestamp 
  time_t ticks;
  ticks = time(NULL);
  snprintf(timestring, 25, "%.25s", ctime(&ticks));
  printf("\n CAEN 4527_InitSystem: %s (num. %d) at %.24s \n", CAENHV_GetError(sysHndl), ret,timestring);

// Now ask SY4527 properties
  // char session[500];
  //CAENHV_GetSysProp(sysHndl,"Sessions"       , session);
  //printf(" %s \n",session);
  char modelname[80];
  CAENHV_GetSysProp(sysHndl,"ModelName"      , modelname);
  //printf("Model %s \n",modelname);
  char swrel[80];
  CAENHV_GetSysProp(sysHndl,"SwRelease"      , swrel);
  //printf("S/W rel  %s \n",swrel);
  //uint16_t signcfg;
  //CAENHV_GetSysProp(sysHndl,"GenSignCfg"     ,&signcfg);
  //printf(" %u \n",signcfg);
  //uint16_t frontpanin;
  //CAENHV_GetSysProp(sysHndl,"FrontPanIn"     ,&frontpanin);
  //printf("Fron Pan in %u \n",frontpanin);
  //uint16_t frontpanout;
  //CAENHV_GetSysProp(sysHndl,"FrontPanOut"    ,&frontpanout);
  //printf("Fron Pan out %u \n",frontpanout);
  //uint16_t tmp;
  //CAENHV_GetSysProp(sysHndl,"ResFlagCfg"     ,&tmp);
  //printf(" %s \n",tmp);
  //char hwpmsm[80];
  //CAENHV_GetSysProp(sysHndl,"HvPwSM"         , hwpmsm);
  // printf("H/W pw SM  %s \n",hwpmsm);
  char hvfanstat[80];
  CAENHV_GetSysProp(sysHndl,"HVFanStat"      , hvfanstat);
  //printf("HV Fan stat %s \n",hvfanstat);
  uint16_t hvfanspeed;
  CAENHV_GetSysProp(sysHndl,"HVFanSpeed"      , &hvfanspeed);
  //printf("HV Fan speed %u \n",hvfanspeed);
  char pwfanstat[80];
  CAENHV_GetSysProp(sysHndl,"PWFanStat"      , pwfanstat);
  //printf("PW Fan stat %s \n",pwfanstat);
  //uint16_t pwfanspeed;
  //CAENHV_GetSysProp(sysHndl,"PWFanSpeed"      , &pwfanspeed);
  //printf("PW Fan speed %u \n",pwfanspeed);
  //uint16_t clockfreq;
  //CAENHV_GetSysProp(sysHndl,"ClkFreq"                ,&clockfreq);
  //printf(" %u \n",clockfreq);
  //char tmp[80] ;
  //CAENHV_GetSysProp(sysHndl,"HVClkConf"      , tmp);
  //printf(" %s \n",tmp);
  char ipaddr[80];
  CAENHV_GetSysProp(sysHndl,"IPAddr"         , ipaddr);
  //printf("IP address %s \n",ipaddr);
  //char ipnetmask[80];
  //CAENHV_GetSysProp(sysHndl,"IPNetMsk"       , ipnetmask);
  //printf(" %s \n",ipnetmask);
  //char tmp[80];
  //CAENHV_GetSysProp(sysHndl,"IPGw"           , tmp);
  //printf(" %s \n",tmp);
  char pwvoltage[80];
  CAENHV_GetSysProp(sysHndl,"PWVoltage"      , pwvoltage);
  //printf("PW volatge %s \n",pwvoltage);
   char pwcurrent[80];
  CAENHV_GetSysProp(sysHndl,"PWCurrent"      , pwcurrent);
  //printf("PW current %s \n",pwcurrent);
  //uint16_t outputlevel ;
  //CAENHV_GetSysProp(sysHndl,"OutputLevel"    ,&outputlevel);
  // printf(" %u \n",outputlevel);
  //char name[80];
  //CAENHV_GetSysProp(sysHndl,"SymbolicName"   , name);
  //printf("Symboic name %s \n",name);
  //uint16_t tmp;
  //CAENHV_GetSysProp(sysHndl,"CmdQueueStatus" ,&tmp);
  //printf(" %s \n",tmp);
  char cpuload[80];
  CAENHV_GetSysProp(sysHndl,"CPULoad"        , cpuload);
  //printf("CPU load %s \n",cpuload);
  char memstat[80];
  CAENHV_GetSysProp(sysHndl,"MemoryStatus"   , memstat);
  //printf("Memory status %s \n",memstat);
 
  fflush(stdout);
  
  // end of channel name and VMon //
  
  // LOGOUT //
  
  ret = CAENHV_DeinitSystem(sysHndl);
  if(ret == CAENHV_OK) {
    printf("CAENHV_DeinitSystem: Connection closed (num. %d)\n", ret);
  } else {
    DrvCaenHV_except::CAENWrapperRetStatus(sysHndl,ret,
					   "IP = "+fIPAddress+"  user = "+fUsername);
    //printf("CAENHV_DeinitSystem: %s (num. %d)\n\n", CAENHV_GetError(sysHndl), ret);
  }
  // end of logout //

  // and send to the data folder for dcs
  
  FILE * pFile;
  // int n;
  std::string filename="data/CAENSY4527";
  pFile = fopen (filename.c_str(),"w");

  fprintf(pFile,"%.24s;%s;%s;%s\n",timestring,modelname,swrel,ipaddr);
  fprintf(pFile,"%u;%s;%s\n",hvfanspeed,hvfanstat,pwfanstat);
  fprintf(pFile,"%s;%s \n",pwvoltage,pwcurrent);
  fprintf(pFile,"%s;%s\n",cpuload,memstat);

  fclose(pFile);
 
  // and print to monitor fan speed and temp
  // and send value to file
  FILE * pFile3;
  std::string filename3="CAENSY4527";
  filename3+=".txt";
  pFile3 = fopen (filename3.c_str(),"w");
  // write values to file
  fprintf(pFile3,"PLOTID CAEN_HV_4527 \nPLOTNAME CAEN HV crate SY4527 status \nPLOTTYPE text \n");
  fprintf(pFile3,"DATA  [ [\"Timestamp\",\"%.24s\"]",timestring);
  fprintf(pFile3,",[\"Model \",\"%s\"]",modelname);
  fprintf(pFile3,",[\"S/W release \",\"\%s\"]",swrel);
  fprintf(pFile3,",[\"IP address \",\"\%s\"]",ipaddr);
  fprintf(pFile3,",[\"HV Fan status\",\%u]",hvfanspeed);
  fprintf(pFile3,",[\"HV Fan speeds\",\"\%s\"]",hvfanstat);
  fprintf(pFile3,",[\"PW fan speeds \",\"\%s\"]",pwfanstat);
  fprintf(pFile3,",[\"PW voltages  \",\"\%s\"]",pwvoltage);
  fprintf(pFile3,",[\"PW currents \",\"\%s\"]",pwcurrent);
  fprintf(pFile3,",[\"CPU load  \",\"\%s\"]",cpuload);
  fprintf(pFile3,",[\"Memory status \",\"\%s\"] ]\n",memstat);

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
  
  // and wait 120 secs for next readout
  sleep(120);

}


  void
DrvCaenSY4527::GetCrateMap()
{
  CAENHVRESULT  ret,ret1,ret2,ret3;
  int           sysHndl=-1;
  char* address = "192.168.62.10";
  int link_type = 0;  // TCP/IP

  ushort nrslots, *nrchlist, *sernumlist;
  char *modellist,*descrlist;
  unsigned char *firmwaremin,*firmwaremax;

  std::lock_guard<std::mutex> guard(fCaenCrateHandle_mutex);

  ret1 = CAENHV_GetCrateMap(fCaenCrateHandle,&nrslots,&nrchlist,&modellist,&descrlist,&sernumlist,&firmwaremin,&firmwaremax);     
  
  if(ret1!=CAENHV_OK) {
    printf("CAENHV_GetCrateMap call error %s (num. %d)\n\n", CAENHV_GetError(sysHndl), ret);
    DrvCaenHV_except::CAENWrapperRetStatus(fCaenCrateHandle,ret);
    
  } else {
    printf(" SY4527 crate map  - number of slots = %d \n",nrslots);
    
    char	*m = modellist, *d = descrlist;
    int i;

    for(i=0;i<nrslots;i++, m += strlen(m) + 1, d += strlen(d) + 1 ){

     if( *m == '\0' ) {
	printf("\n Board %2d: Not Present \n", i);
      } else {             

       std::cout<<" slot N. "<< i << std::endl;
       std::cout<<"nrchlist     "<<nrchlist         [i] <<" "<<std::endl
		<<"modellist    "<<m                    <<" "<<std::endl
		<<"descrlist    "<<d                    <<" "<<std::endl
		<<"sernumlist   "<<sernumlist       [i] <<" "<<std::endl
		<<"firmwaremin  "<<int(firmwaremin  [i])<<" "<<std::endl
		<<"firmwaremax " <<int(firmwaremax  [i])<<" "<<std::endl
		<<std::endl;
     }
    }

    //std::cerr<<"bef del "<<__FILE__<<__LINE__<<__func__<<std::endl;
    CAENHV_Free(nrchlist    );/// must be dealocated by the user
    CAENHV_Free(modellist   );/// must be dealocated by the user
    CAENHV_Free(descrlist   );/// must be dealocated by the user
    CAENHV_Free(sernumlist  );/// must be dealocated by the user
    CAENHV_Free(firmwaremin );/// must be dealocated by the user
    CAENHV_Free(firmwaremax );/// must be dealocated by the user
  }
}
