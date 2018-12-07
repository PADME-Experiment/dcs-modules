#include "DrvCaenHV.h"   //in c file
#include"fwk/utlCommaDashListToVector.h"
//#include <caenlib/CAENHVWrapper.h>
#include "extern/CAENHVWrapper/CAENHVWrapper.h"
#include<iostream>
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */
#include <stdio.h>
#include <string.h>
#include <ctype.h>


  DrvCaenHV_except&
DrvCaenHV_except::CAENWrapperRetStatus(int caenhandler,int retstatus, const std::string& add)
{ std::string a(CAENHV_GetError(caenhandler));
std::string error=(
retstatus==0     ? "Command wrapper correctly executed"                                            :
retstatus==1     ? "Error of operatived system"                                                    :
retstatus==2     ? "Write error in communication channel"                                          :
retstatus==3     ? "Read error in communication channel"                                           :
retstatus==4     ? "Time out in server communication"                                              :
retstatus==5     ? "Command Front End application is down"                                         :
retstatus==6     ? "Communication with system not yet connected by a Login command"                :
retstatus==7     ? "Communication with a not present board/slot"                                   :
retstatus==8     ? "Communication with RS232 not yet implemented"                                  :
retstatus==9     ? "User memory not sufficient"                                                    :
retstatus==10    ? "Value out of range"                                                            :
retstatus==11    ? "Execute command not yet implemented"                                           :
retstatus==12    ? "Get Property not yet implemented"                                              :
retstatus==13    ? "Set Property not yet implemented"                                              :
retstatus==14    ? "Property not found"                                                            :
retstatus==15    ? "Execute command not found"                                                     :
retstatus==16    ? "No System property"                                                            :
retstatus==17    ? "No get property"                                                               :
retstatus==18    ? "No set property"                                                               :
retstatus==19    ? "No execute command"                                                            :
retstatus==20    ? "Device configuration changed"                                                  :
retstatus==21    ? "Property of param not found"                                                   :
retstatus==22    ? "Param not found"                                                               :
retstatus==23    ? "No data present"                                                               :
retstatus==24    ? "Device already open"                                                           :
retstatus==25    ? "To Many devices opened"                                                        :
retstatus==26    ? "Function Parameter not valid"                                                  :
retstatus==27    ? "Function not available for the connected device"                               :
retstatus==0x1001? "Device already connected"                                                      :
retstatus==0x1002? "Device not connected"                                                          :
retstatus==0x1003? "Operating system error"                                                        :
retstatus==0x1004? "Login failed"                                                                  :
retstatus==0x1005? "Logout failed"                                                                 :
retstatus==0x1006? "Link type not supported"                                                       :
retstatus==0x1007? "Login failed for username/password ( SY4527 / SY5527 )"                        :
"Unknown");


  std::string str("CAENHVWrapper returned error ("+std::to_string(retstatus)+") : '" + a + "'  "+error);
  if(add.size()>0)str+="   "+add;

  switch (retstatus){
    case CAENHV_SYSERR               : throw DrvCaenHV_except::Except_SYSERR               (str) ;break;
    case CAENHV_WRITEERR             : throw DrvCaenHV_except::Except_WRITEERR             (str) ;break;
    case CAENHV_READERR              : throw DrvCaenHV_except::Except_READERR              (str) ;break;
    case CAENHV_TIMEERR              : throw DrvCaenHV_except::Except_TIMEERR              (str) ;break;
    case CAENHV_DOWN                 : throw DrvCaenHV_except::Except_DOWN                 (str) ;break;
    case CAENHV_NOTPRES              : throw DrvCaenHV_except::Except_NOTPRES              (str) ;break;
    case CAENHV_SLOTNOTPRES          : throw DrvCaenHV_except::Except_SLOTNOTPRES          (str) ;break;
    case CAENHV_NOSERIAL             : throw DrvCaenHV_except::Except_NOSERIAL             (str) ;break;
    case CAENHV_MEMORYFAULT          : throw DrvCaenHV_except::Except_MEMORYFAULT          (str) ;break;
    case CAENHV_OUTOFRANGE           : throw DrvCaenHV_except::Except_OUTOFRANGE           (str) ;break;
    case CAENHV_EXECCOMNOTIMPL       : throw DrvCaenHV_except::Except_EXECCOMNOTIMPL       (str) ;break;
    case CAENHV_GETPROPNOTIMPL       : throw DrvCaenHV_except::Except_GETPROPNOTIMPL       (str) ;break;
    case CAENHV_SETPROPNOTIMPL       : throw DrvCaenHV_except::Except_SETPROPNOTIMPL       (str) ;break;
    case CAENHV_PROPNOTFOUND         : throw DrvCaenHV_except::Except_PROPNOTFOUND         (str) ;break;
    case CAENHV_EXECNOTFOUND         : throw DrvCaenHV_except::Except_EXECNOTFOUND         (str) ;break;
    case CAENHV_NOTSYSPROP           : throw DrvCaenHV_except::Except_NOTSYSPROP           (str) ;break;
    case CAENHV_NOTGETPROP           : throw DrvCaenHV_except::Except_NOTGETPROP           (str) ;break;
    case CAENHV_NOTSETPROP           : throw DrvCaenHV_except::Except_NOTSETPROP           (str) ;break;
    case CAENHV_NOTEXECOMM           : throw DrvCaenHV_except::Except_NOTEXECOMM           (str) ;break;
    case CAENHV_SYSCONFCHANGE        : throw DrvCaenHV_except::Except_SYSCONFCHANGE        (str) ;break;
    case CAENHV_PARAMPROPNOTFOUND    : throw DrvCaenHV_except::Except_PARAMPROPNOTFOUND    (str) ;break;
    case CAENHV_PARAMNOTFOUND        : throw DrvCaenHV_except::Except_PARAMNOTFOUND        (str) ;break;
    case CAENHV_NODATA               : throw DrvCaenHV_except::Except_NODATA               (str) ;break;
    case CAENHV_DEVALREADYOPEN       : throw DrvCaenHV_except::Except_DEVALREADYOPEN       (str) ;break;
    case CAENHV_TOOMANYDEVICEOPEN    : throw DrvCaenHV_except::Except_TOOMANYDEVICEOPEN    (str) ;break;
    case CAENHV_INVALIDPARAMETER     : throw DrvCaenHV_except::Except_INVALIDPARAMETER     (str) ;break;
    case CAENHV_FUNCTIONNOTAVAILABLE : throw DrvCaenHV_except::Except_FUNCTIONNOTAVAILABLE (str) ;break;
    case CAENHV_SOCKETERROR          : throw DrvCaenHV_except::Except_SOCKETERROR          (str) ;break;
    case CAENHV_COMMUNICATIONERROR   : throw DrvCaenHV_except::Except_COMMUNICATIONERROR   (str) ;break;
    case CAENHV_NOTYETIMPLEMENTED    : throw DrvCaenHV_except::Except_NOTYETIMPLEMENTED    (str) ;break;
    case CAENHV_CONNECTED            : throw DrvCaenHV_except::Except_CONNECTED            (str) ;break;
    case CAENHV_NOTCONNECTED         : throw DrvCaenHV_except::Except_NOTCONNECTED         (str) ;break;
    case CAENHV_OS                   : throw DrvCaenHV_except::Except_OS                   (str) ;break;
    case CAENHV_LOGINFAILED          : throw DrvCaenHV_except::Except_LOGINFAILED          (str) ;break;
    case CAENHV_LOGOUTFAILED         : throw DrvCaenHV_except::Except_LOGOUTFAILED         (str) ;break;
    case CAENHV_LINKNOTSUPPORTED     : throw DrvCaenHV_except::Except_LINKNOTSUPPORTED     (str) ;break;
    case CAENHV_USERPASSFAILED       : throw DrvCaenHV_except::Except_USERPASSFAILED       (str) ;break;
  }
}



  int
DrvCaenHV::ComInit()
{
  CAENHVRESULT ret;
  int handle;
  handle=0;

  /*
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
  */

  return handle;
}


  void
DrvCaenHV::ComDeinit(int handle)
{
  int ret = CAENHV_DeinitSystem(handle);
  if(ret != CAENHV_OK)
    DrvCaenHV_except::CAENWrapperRetStatus(handle,ret);
}


  void
DrvCaenHV::AssertInit()
{
  INFO("DrvCaenHV::AssertInit()");
  //#warning CAEN ComInit
  fCaenCrateHandle=ComInit();
  AssertInitAllOwned();
  //ProcessUpdateListTemp();
}
  void
DrvCaenHV::Finalize()
{
  INFO("DrvCaenHV::Finalize()");
  this->VDeviceBase::Finalize();
  //#warning CAEN ComDeinit
  ComDeinit(fCaenCrateHandle );
  JoinThread();
}


void
DrvCaenHV::OnCycleLocal()
{
  int handle;
  char timestring[27];
  bool debug=false;

  CAENHVRESULT  ret,ret1,ret2,ret3;
  int           sysHndl=-1;
  // char* address = "192.168.62.10";
  int link_type = 0;  // TCP/IP
   
  // printf("\n\n **************** reading channels ********** \n");
  // LOGIN //
  
  ret = CAENHV_InitSystem((CAENHV_SYSTEM_TYPE_t)2, link_type,(void*)fIPAddress.c_str(), fUsername.c_str(), fPassword.c_str(), &sysHndl);
  
  if( ret == CAENHV_OK ){
    if(debug) printf("\n CAEN system online - handle = %d \n\n",sysHndl);
  } else {
    DrvCaenHV_except::CAENWrapperRetStatus(sysHndl,ret,
					   "IP = "+fIPAddress+"  user = "+fUsername+"  pass = "+fPassword);
    printf("\n CAEN HV connect error - No connection \n");
    return;
  }
  
  // end of login //

  // get timestamp 
  time_t ticks;
  // OLD - Human readable format
  //ticks = time(NULL);
  //snprintf(timestring, 25, "%.25s", ctime(&ticks));

  // new formatted timestring
  time (&ticks);
  struct tm * timeinfo;
  timeinfo = localtime (&ticks);
  strftime (timestring,25,"%Y-%m-%d %T",timeinfo);

  if(debug) printf("\n CAENHV_InitSystem: %s (num. %d) at %.24s \n", CAENHV_GetError(sysHndl), ret,timestring);


  // first read all SY4527 relevant parameters

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
 
  //fflush(stdout);
  
  // end of channel name and VMon //
  
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
  std::string filename3="monitor/CAENSY4527";
  filename3+=".txt";
  pFile3 = fopen (filename3.c_str(),"w");
  // write values to file
  fprintf(pFile3,"PLOTID CAEN_HV_4527 \nPLOTNAME CAEN HV crate SY4527 status \nPLOTTYPE text \n");
  fprintf(pFile3,"DATA  [ [\"Timestamp\",\"%.24s\"]",timestring);
  //fprintf(pFile3,",[\"Model \",\"%s\"]",modelname);
  //fprintf(pFile3,",[\"S/W release \",\"\%s\"]",swrel);
  fprintf(pFile3,",[\"IP address \",\"\%s\"]",ipaddr);
  fprintf(pFile3,",[\"HV Fan status\",\%u]",hvfanspeed);
  fprintf(pFile3,",[\"HV Fan speeds\",\"\%s\"]",hvfanstat);
  fprintf(pFile3,",[\"PW fan speeds \",\"\%s\"]",pwfanstat);
  fprintf(pFile3,",[\"PW voltages  \",\"\%s\"]",pwvoltage);
  fprintf(pFile3,",[\"PW currents \",\"\%s\"]",pwcurrent);
  //fprintf(pFile3,",[\"CPU load  \",\"\%s\"]",cpuload);
  //fprintf(pFile3,",[\"Memory status \",\"\%s\"]",memstat);
  fprintf(pFile3," ]\n",memstat);

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


  // and now read voltages, current, status for all ecal/SAC channels

  unsigned short slot;
  unsigned short NrOfCh=48;
  const unsigned short TotalEcal=620;
  const unsigned short TotalSAC=30;
  const unsigned short TotalCosm=4;
  unsigned short nch,Ch ;  //*ChList;
  unsigned short *listaCh;
  char  listNameCh[NrOfCh][MAX_CH_NAME];
  char ParName1[7] = "VMon";  
  char ParName2[7] = "IMon";  
  char ParName3[7] = "Status";  
  float *fParValList1;
  float *fParValList2;
  float *fParValList3;

 /*
  char *SACNameCh[TotalSAC];
  char *EcalNameCh[TotalEcal];
  char *CosmNameCh[TotalCosm];
   float SACList1[TotalSAC];
  float EcalList1[TotalEcal];
  float CosmList1[TotalCosm];
  float SACList2[TotalSAC];
  float EcalList2[TotalEcal];
  float CosmList2[TotalCosm];
  float SACList3[TotalSAC];
  float EcalList3[TotalEcal];
  float CosmList3[TotalCosm];
 */

  int total_number_ecal_channels=0;
  int total_number_sac_channels=0;
  int total_number_cosmic_channels=0;
  int islot;

  
  FILE * pFile1;
  FILE * pFile1h;
  std::string filename1="data/CAENHV_ECAL";
  std::string filename1h="history/CAENHV_ECAL";
  pFile1 = fopen (filename1.c_str(),"w");
  pFile1h = fopen (filename1h.c_str(),"a");
  
  FILE * pFile2;
  std::string filename2="data/CAENHV_SAC";
  pFile2 = fopen (filename2.c_str(),"w");
  FILE * pFile2h;
  std::string filename2h="history/CAENHV_SAC";
  pFile2h = fopen (filename2h.c_str(),"a");

  FILE * pFile4;
  std::string filename4="data/CAENCosmic";
  pFile4 = fopen (filename4.c_str(),"w");
  FILE * pFile4h;
  std::string filename4h="history/CAENCosmics";
  pFile4h = fopen (filename4h.c_str(),"a");
  
  // And store timestring
  fprintf(pFile1,"%.24s;%s;616\n",timestring,ipaddr);
  fprintf(pFile2,"%.24s;%s;25\n",timestring,ipaddr);
  fprintf(pFile4,"%.24s;%s;4\n",timestring,ipaddr);
  fprintf(pFile1h,"%.24s;%s;616\n",timestring,ipaddr);
  fprintf(pFile2h,"%.24s;%s;25\n",timestring,ipaddr);
  fprintf(pFile4h,"%.24s;%s;4\n",timestring,ipaddr);

  // loop only over first 14 slots
  for (islot=0;islot<15;islot++) {

    unsigned short maxch=NrOfCh;
    // for last slot for cosmics 12 channels board and only first 4 channels
    if(islot>13) {
      NrOfCh=12;
      maxch=4;
    }

    slot = islot;
    int usize = NrOfCh*sizeof(unsigned short);
    int fsize = NrOfCh*sizeof(float);
    listaCh = (unsigned short*) malloc(usize);
    fParValList1 = (float*) malloc(fsize);
    fParValList2 = (float*) malloc(fsize);
    fParValList3 = (float*) malloc(fsize);

    // loop on all slots
    //printf("CAEN SY4527 - Slot number %d ",slot);

     for(nch=0;nch<NrOfCh;nch++){
      // at the moment Ch and nch are the same
      Ch = nch;
      listaCh[nch] = Ch;
    }
    
    
    // ChList = malloc(ChNum * sizeof(unsigned short));
    //int chansize=NrOfCh*MAX_CH_NAME;
    //listNameCh = (char*) malloc(chansize);

    // get the channel name list
    ret = CAENHV_GetChName(sysHndl, slot, NrOfCh, listaCh, listNameCh);
    if(ret != CAENHV_OK){   
      //free(listNameCh);
      if(debug) printf("CAENHV_GetChName: %s (num. %d)\n\n", CAENHV_GetError(sysHndl), ret);
      DrvCaenHV_except::CAENWrapperRetStatus(sysHndl,ret,
					     "IP = "+fIPAddress+"  user = "+fUsername+"  pass = "+fPassword);
      return; 
    }else{
      

      int numofchannels=0;

      // Get VMon
      ret1 = CAENHV_GetChParam(sysHndl, slot, ParName1, NrOfCh, listaCh, fParValList1);
      if(ret1 != CAENHV_OK){
	if(debug)printf(", Channel %s - CAENHV_GetChParam: %s (num. %d)\n\n", ParName1,CAENHV_GetError(sysHndl), ret1);
 	DrvCaenHV_except::CAENWrapperRetStatus(sysHndl,ret1,
					       "IP = "+fIPAddress+"  user = "+fUsername);
     }

      // Get IMon
      ret2 = CAENHV_GetChParam(sysHndl, slot, ParName2, NrOfCh, listaCh, fParValList2);
      if(ret2 != CAENHV_OK){
	if(debug)printf(", Channel %s - CAENHV_GetChParam: %s (num. %d)\n\n", ParName2,CAENHV_GetError(sysHndl), ret2);
	DrvCaenHV_except::CAENWrapperRetStatus(sysHndl,ret2,
					       "IP = "+fIPAddress+"  user = "+fUsername);
      }

      // Get Status
      ret3 = CAENHV_GetChParam(sysHndl, slot, ParName3, NrOfCh, listaCh, fParValList3);
      if(ret3 != CAENHV_OK){
	if(debug) //printf(", Channel %s - CAENHV_GetChParam: %s (num. %d)\n\n", ParName3,CAENHV_GetError(sysHndl), ret3);
	DrvCaenHV_except::CAENWrapperRetStatus(sysHndl,ret3,
					       "IP = "+fIPAddress+"  user = "+fUsername);
      }

      for(nch = 0; nch < maxch; nch++) {

	if(debug) {
	  // print value
	  printf("Chan n. %3d: %10s", listaCh[nch], listNameCh[nch]);
	  printf(", slot: %2d, %s: %10.2f   %s: %10.2f   %s: %10.2f \n", slot, ParName1, fParValList1[nch],ParName2,fParValList2[nch],ParName3,fParValList3[nch]);
	}

        //char *namechan=listNameCh[nch];
	
	// exclude channels with CHANN 
	if (strncmp (listNameCh[nch],"CHANNEL",6) == 0) {
	  // do nothing
	  //printf(" Slot %d - channel %d - found channel named %s - exclude from monitor channels \n",slot,nch,listNameCh[nch]);
	} else if (strncmp (listNameCh[nch],"SAC",3) == 0) {
	  // get SAC channels
	  //printf("Slot %d - channel %d - SAC channel %s\n",slot,nch,listNameCh[nch]);
	  
	  // write values to file
	  fprintf(pFile2,"%d;%d;%.12s;%6.2f;%6.2f;%6.2f\n",slot,nch,listNameCh[nch],fParValList1[nch],fParValList2[nch],fParValList3[nch]);	  
	  fprintf(pFile2h,"%d;%d;%.12s;%6.2f;%6.2f;%6.2f\n",slot,nch,listNameCh[nch],fParValList1[nch],fParValList2[nch],fParValList3[nch]);	  

	  // and add counter for all SAC channels
	  total_number_sac_channels++;
	} else if (strncmp (listNameCh[nch],"cosmic",6) == 0) {
	  // get cosmic channels
	  //printf("Slot %d - channel %d - cosmic channel %s\n",slot,nch,listNameCh[nch])
	  // write values to file
	  fprintf(pFile4,"%d;%d;%.12s;%6.2f;%6.2f;%6.2f\n",slot,nch,listNameCh[nch],fParValList1[nch],fParValList2[nch],fParValList3[nch]);	  
	  fprintf(pFile4h,"%d;%d;%.12s;%6.2f;%6.2f;%6.2f\n",slot,nch,listNameCh[nch],fParValList1[nch],fParValList2[nch],fParValList3[nch]);	  

	  // and add counter for all SAC channels
	  total_number_cosmic_channels++;
	} else {
	  // get Ecal channels
	  numofchannels++;

	  // write values to file
	  fprintf(pFile1,"%d;%d;%.12s;%6.2f;%6.2f;%6.2f\n",slot,nch,listNameCh[nch],fParValList1[nch],fParValList2[nch],fParValList3[nch]);
	  fprintf(pFile1h,"%d;%d;%.12s;%6.2f;%6.2f;%6.2f\n",slot,nch,listNameCh[nch],fParValList1[nch],fParValList2[nch],fParValList3[nch]);

	  // and increase ecal counter
	  total_number_ecal_channels++;

	}
	
	
      } // end for loop

      //printf("- number of channels read = %d \n",numofchannels);
    
      // total_number_ecal_channels += numofchannels; 



    }  //endif listach
    

    
    // end of channel name and VMon //
    //free(listNameCh);
    free(listaCh);
    free(fParValList1);
    free(fParValList2);
    free(fParValList3);

  } // endif islot

  // LOGOUT 
  
  ret = CAENHV_DeinitSystem(sysHndl);
  if(ret == CAENHV_OK) {
    printf("CAENHV_DeinitSystem: Connection closed (num. %d)\n", ret);
  } else {
    if(debug) printf("CAENHV_DeinitSystem: %s (num. %d)\n\n", CAENHV_GetError(sysHndl), ret);
    DrvCaenHV_except::CAENWrapperRetStatus(sysHndl,ret,
					   "IP = "+fIPAddress+"  user = "+fUsername);
  }
  // end of logout 

  if(debug) {
    printf("Total number of ecal channels = %d \n",total_number_ecal_channels);
    printf("Total number of SAC channels = %d \n",total_number_sac_channels);
    printf("Total number of cosmic channels = %d \n",total_number_cosmic_channels);
  }

  // and close output files
  fclose(pFile1);
  fclose(pFile2);
  fclose(pFile4);
  fclose(pFile1h);
  fclose(pFile2h);
  fclose(pFile4h);
  

  // fflush(stdout);

  // and now run the program to read the ecal values and produce monitoring files

  char alarm[3]=" 1";
  if(fAlarm!=""){
    std::copy(fAlarm.begin(), fAlarm.end(), alarm);
    alarm[fAlarm.size()] = '\0'; // don't forget the terminating 0
  }
  std::string command="./readfile_";
  std::string det_ecal="ecal";
  std::string det_sac="sac";
  std::string moncomm="./mon_summ_hvalarm.exe ";
  std::string exe=".exe";
  std::string blank=" ";
  std::string det_cosmics="cosmics";
  std::string cout=".out";
  std::string command1=command+det_ecal+exe+blank+alarm+"  >"+command+det_ecal+cout;
  std::string command2=command+det_sac+exe+blank+alarm+"  >"+command+det_sac+cout;
  std::string command3=command+det_cosmics+exe+blank+alarm+"  >"+command+det_cosmics+cout;
  std::string command4=moncomm+blank+alarm;
  command1[command1.size()]= '\0';
  command2[command2.size()]= '\0';
  command3[command3.size()]= '\0';
  if(debug) {
    printf(" Alarm state is %s \n",alarm);
    printf(" command1 = %s \n",command1.c_str());
    printf(" command2 = %s \n",command2.c_str());
    printf(" command3 = %s \n",command3.c_str());
    printf(" command4 = %s \n",command4.c_str());
  }

  system(command1.c_str());
  system(command2.c_str());
  system(command3.c_str());
  system(command4.c_str());

  
  // and wait 60 secs for next readout
  sleep(60);
 
 
}
