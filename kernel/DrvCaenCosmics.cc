#include "DrvCaenCosmics.h"   //in c file
#include"fwk/utlCommaDashListToVector.h"
//#include <caenlib/CAENHVWrapper.h>
#include "extern/CAENHVWrapper/CAENHVWrapper.h"
#include<iostream>
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */
#include <stdio.h>
#include <string.h>
#include <ctype.h>


  DrvCaenCosmics_except&
DrvCaenCosmics_except::CAENWrapperRetStatus(int caenhandler,int retstatus, const std::string& add)
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
    case CAENHV_SYSERR               : throw DrvCaenCosmics_except::Except_SYSERR               (str) ;break;
    case CAENHV_WRITEERR             : throw DrvCaenCosmics_except::Except_WRITEERR             (str) ;break;
    case CAENHV_READERR              : throw DrvCaenCosmics_except::Except_READERR              (str) ;break;
    case CAENHV_TIMEERR              : throw DrvCaenCosmics_except::Except_TIMEERR              (str) ;break;
    case CAENHV_DOWN                 : throw DrvCaenCosmics_except::Except_DOWN                 (str) ;break;
    case CAENHV_NOTPRES              : throw DrvCaenCosmics_except::Except_NOTPRES              (str) ;break;
    case CAENHV_SLOTNOTPRES          : throw DrvCaenCosmics_except::Except_SLOTNOTPRES          (str) ;break;
    case CAENHV_NOSERIAL             : throw DrvCaenCosmics_except::Except_NOSERIAL             (str) ;break;
    case CAENHV_MEMORYFAULT          : throw DrvCaenCosmics_except::Except_MEMORYFAULT          (str) ;break;
    case CAENHV_OUTOFRANGE           : throw DrvCaenCosmics_except::Except_OUTOFRANGE           (str) ;break;
    case CAENHV_EXECCOMNOTIMPL       : throw DrvCaenCosmics_except::Except_EXECCOMNOTIMPL       (str) ;break;
    case CAENHV_GETPROPNOTIMPL       : throw DrvCaenCosmics_except::Except_GETPROPNOTIMPL       (str) ;break;
    case CAENHV_SETPROPNOTIMPL       : throw DrvCaenCosmics_except::Except_SETPROPNOTIMPL       (str) ;break;
    case CAENHV_PROPNOTFOUND         : throw DrvCaenCosmics_except::Except_PROPNOTFOUND         (str) ;break;
    case CAENHV_EXECNOTFOUND         : throw DrvCaenCosmics_except::Except_EXECNOTFOUND         (str) ;break;
    case CAENHV_NOTSYSPROP           : throw DrvCaenCosmics_except::Except_NOTSYSPROP           (str) ;break;
    case CAENHV_NOTGETPROP           : throw DrvCaenCosmics_except::Except_NOTGETPROP           (str) ;break;
    case CAENHV_NOTSETPROP           : throw DrvCaenCosmics_except::Except_NOTSETPROP           (str) ;break;
    case CAENHV_NOTEXECOMM           : throw DrvCaenCosmics_except::Except_NOTEXECOMM           (str) ;break;
    case CAENHV_SYSCONFCHANGE        : throw DrvCaenCosmics_except::Except_SYSCONFCHANGE        (str) ;break;
    case CAENHV_PARAMPROPNOTFOUND    : throw DrvCaenCosmics_except::Except_PARAMPROPNOTFOUND    (str) ;break;
    case CAENHV_PARAMNOTFOUND        : throw DrvCaenCosmics_except::Except_PARAMNOTFOUND        (str) ;break;
    case CAENHV_NODATA               : throw DrvCaenCosmics_except::Except_NODATA               (str) ;break;
    case CAENHV_DEVALREADYOPEN       : throw DrvCaenCosmics_except::Except_DEVALREADYOPEN       (str) ;break;
    case CAENHV_TOOMANYDEVICEOPEN    : throw DrvCaenCosmics_except::Except_TOOMANYDEVICEOPEN    (str) ;break;
    case CAENHV_INVALIDPARAMETER     : throw DrvCaenCosmics_except::Except_INVALIDPARAMETER     (str) ;break;
    case CAENHV_FUNCTIONNOTAVAILABLE : throw DrvCaenCosmics_except::Except_FUNCTIONNOTAVAILABLE (str) ;break;
    case CAENHV_SOCKETERROR          : throw DrvCaenCosmics_except::Except_SOCKETERROR          (str) ;break;
    case CAENHV_COMMUNICATIONERROR   : throw DrvCaenCosmics_except::Except_COMMUNICATIONERROR   (str) ;break;
    case CAENHV_NOTYETIMPLEMENTED    : throw DrvCaenCosmics_except::Except_NOTYETIMPLEMENTED    (str) ;break;
    case CAENHV_CONNECTED            : throw DrvCaenCosmics_except::Except_CONNECTED            (str) ;break;
    case CAENHV_NOTCONNECTED         : throw DrvCaenCosmics_except::Except_NOTCONNECTED         (str) ;break;
    case CAENHV_OS                   : throw DrvCaenCosmics_except::Except_OS                   (str) ;break;
    case CAENHV_LOGINFAILED          : throw DrvCaenCosmics_except::Except_LOGINFAILED          (str) ;break;
    case CAENHV_LOGOUTFAILED         : throw DrvCaenCosmics_except::Except_LOGOUTFAILED         (str) ;break;
    case CAENHV_LINKNOTSUPPORTED     : throw DrvCaenCosmics_except::Except_LINKNOTSUPPORTED     (str) ;break;
    case CAENHV_USERPASSFAILED       : throw DrvCaenCosmics_except::Except_USERPASSFAILED       (str) ;break;
  }
}



  int
DrvCaenCosmics::ComInit()
{
  CAENHVRESULT ret;
  int handle;
  CAENHV_SYSTEM_TYPE_t sys_type = (CAENHV_SYSTEM_TYPE_t) SY4527; // 0: SY1527, 2: SY4527
  int link_type = LINKTYPE_TCPIP;
  ret = CAENHV_InitSystem(sys_type, link_type, (void*)fIPAddress.c_str(), fUsername.c_str(), fPassword.c_str(), &handle);
  if(ret != CAENHV_OK){
    DrvCaenCosmics_except::CAENWrapperRetStatus(handle,ret,
        "IP = "+fIPAddress+"  user = "+fUsername+"  pass = "+fPassword);
  }
  
  // and close the handle
  ret = CAENHV_DeinitSystem(handle);
  if(ret != CAENHV_OK)
    DrvCaenCosmics_except::CAENWrapperRetStatus(handle,ret);
  return handle;
}

  void
DrvCaenCosmics::ComDeinit(int handle)
{
  int ret = CAENHV_DeinitSystem(handle);
  if(ret != CAENHV_OK)
    DrvCaenCosmics_except::CAENWrapperRetStatus(handle,ret);
}


  void
DrvCaenCosmics::AssertInit()
{
  INFO("DrvCaenCosmics::AssertInit()");
  //#warning CAEN ComInit
  fCaenCrateHandle=ComInit();
  AssertInitAllOwned();
  //ProcessUpdateListTemp();
}
  void
DrvCaenCosmics::Finalize()
{
  INFO("DrvCaenCosmics::Finalize()");
  this->VDeviceBase::Finalize();
  //#warning CAEN ComDeinit
  ComDeinit(fCaenCrateHandle );
  JoinThread();
}


void
DrvCaenCosmics::OnCycleLocal()
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
    DrvCaenCosmics_except::CAENWrapperRetStatus(sysHndl,ret,
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


  // and now read voltages, current, status for all Cosmics channels

  unsigned short slot;
  const unsigned short NrOfCh=4;
  const unsigned short TotalCosm=4;
  unsigned short nch,Ch ;  //*ChList;
  unsigned short *listaCh;
  char  listNameCh[NrOfCh][MAX_CH_NAME];
  char ParName1[7] = "VMon";  
  float *fParValList1;
  char ParName2[7] = "IMon";  
  float *fParValList2;
  char ParName3[7] = "Status";  
  float *fParValList3;
  
  int total_number_cosmic_channels=0;
  int islot;
 
   
  // loop only on slot 14
  islot=14;

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
    // error in CAEN readout
    //free(listNameCh);
    if(debug) printf("CAENHV_GetChName: %s (num. %d)\n\n", CAENHV_GetError(sysHndl), ret);
    DrvCaenCosmics_except::CAENWrapperRetStatus(sysHndl,ret,
						"IP = "+fIPAddress+"  user = "+fUsername+"  pass = "+fPassword);
    return; 
  }else{


    int numofchannels=0;

    // Get VMon
    ret1 = CAENHV_GetChParam(sysHndl, slot, ParName1, NrOfCh, listaCh, fParValList1);
    if(ret1 != CAENHV_OK){
      if(debug)printf(", Channel %s - CAENHV_GetChParam: %s (num. %d)\n\n", ParName1,CAENHV_GetError(sysHndl), ret1);
      DrvCaenCosmics_except::CAENWrapperRetStatus(sysHndl,ret1,
						  "IP = "+fIPAddress+"  user = "+fUsername);
    }

    // Get IMon
    ret2 = CAENHV_GetChParam(sysHndl, slot, ParName2, NrOfCh, listaCh, fParValList2);
    if(ret2 != CAENHV_OK){
      if(debug)printf(", Channel %s - CAENHV_GetChParam: %s (num. %d)\n\n", ParName2,CAENHV_GetError(sysHndl), ret2);
      DrvCaenCosmics_except::CAENWrapperRetStatus(sysHndl,ret2,
						  "IP = "+fIPAddress+"  user = "+fUsername);
    }

    // Get Status
    ret3 = CAENHV_GetChParam(sysHndl, slot, ParName3, NrOfCh, listaCh, fParValList3);
    if(ret3 != CAENHV_OK){
      if(debug) //printf(", Channel %s - CAENHV_GetChParam: %s (num. %d)\n\n", ParName3,CAENHV_GetError(sysHndl), ret3);
	DrvCaenCosmics_except::CAENWrapperRetStatus(sysHndl,ret3,
						    "IP = "+fIPAddress+"  user = "+fUsername);
    }

    FILE * pFile1;
    std::string filename1="data/CAENCosmics";
    pFile1 = fopen (filename1.c_str(),"w");
 
    // And store timestring
    fprintf(pFile1,"%.24s;%s\n",timestring,fIPAddress.c_str());

    // and send value to Monitor
    FILE * pFile3;
    std::string filename3="monitor/CosmicsHV.txt";
    pFile3 = fopen (filename3.c_str(),"w");

    // write values to file
    fprintf(pFile3,"PLOTID Cosmics_HV \nPLOTNAME HV for Cosmics scintillators \nPLOTTYPE text \n");
    fprintf(pFile3,"DATA  [ [\"Timestamp\",\"%.24s\"],",timestring);
 

    // check only first 4 channels
    for(nch = 0; nch < NrOfCh; nch++) {
	
      // get Ecal channels
      numofchannels++;
	
      // write values to file
      fprintf(pFile1,"%d;%d;%.12s;%6.2f;%6.2f;%6.2f\n",slot,nch,listNameCh[nch],fParValList1[nch],fParValList2[nch],fParValList3[nch]);

      // and increase ecal counter
      total_number_cosmic_channels++;

      // and write file for Monitor
     if(nch>0) { 
       fprintf(pFile3,",");
     }
     // and write to monitor
     fprintf(pFile3,"[\"Channel\",\"%s\"]",listNameCh[nch]); 
     fprintf(pFile3,",[\"Voltage \",\%.2f]",fParValList1[nch]);
     fprintf(pFile3,",[\"Current \",\%.2f]",fParValList2[nch]);
     fprintf(pFile3,",[\"Status \",\%.2f]",fParValList3[nch]);
     // printf("\n");

    }  // end for loop	

    // close buffer for monitor
    fprintf(pFile3," ]");
    // and close output files
    fclose(pFile1);
    fclose(pFile3);

    // and copy file to monitor@l0padme3
    std::string scp2="scp -q "+filename3+" monitor@l0padme3:PadmeMonitor/watchdir/. ";
    // cout << " scp command " << scp2 << endl; 
    char * writable2 = new char[scp2.size() + 1];
    std::copy(scp2.begin(), scp2.end(), writable2);
    writable2[scp2.size()] = '\0'; // don't forget the terminating 0
    // scp to monitor@l0padme3
    system(writable2);

  } // endif listach
    
  
  if(debug) {
     printf("Total number of Cosmics channels read = %d \n",total_number_cosmic_channels);
     int nch1;
     for(nch1 = 0; nch1 <total_number_cosmic_channels ; nch1++) {
       printf("Chan n. %3d: %10s", listaCh[nch1], listNameCh[nch1]);
       printf(", %s: %10.2f   %s: %10.2f   %s: %10.2f \n", ParName1, fParValList1[nch1],ParName2,fParValList2[nch1],ParName3,fParValList3[nch1]);
     }
     
  }
  
  // end of channel name and VMon //
  //free(listNameCh);
  free(listaCh);
  free(fParValList1);
  free(fParValList2);
  free(fParValList3);



  // LOGOUT //
  
  ret = CAENHV_DeinitSystem(sysHndl);
  if(ret == CAENHV_OK) {
    printf("CAENHV_DeinitSystem: Connection closed (num. %d)\n", ret);
  } else {
    if(debug) printf("CAENHV_DeinitSystem: %s (num. %d)\n\n", CAENHV_GetError(sysHndl), ret);
    DrvCaenCosmics_except::CAENWrapperRetStatus(sysHndl,ret,
					   "IP = "+fIPAddress+"  user = "+fUsername);
  }
  // end of logout //

  fflush(stdout);

  
  // and wait 60 secs for next readout
  sleep(60);
 
 
}
