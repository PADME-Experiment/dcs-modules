#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>     /* atoi */
#include <math.h>       /* round, floor, ceil, trunc */
#include <time.h>
#include "CAENHVWrapper.h"
#include <ctype.h>
#include <errno.h>

int main() {

  std::string line;
  char *linec;
  char *tok;
  int inumber[700];
  int slot[700];
  int nch[700];
  int x[700];
  int y[700];
  int inumber1[700];
  int slot1[700];
  int nch1[700];
  float hv[700];
  float current[700];
  float status[700];
  CAENHVRESULT  ret,ret1,ret2,ret3;
  int           sysHndl=-1;
  char* address = "192.168.62.10";
  int link_type = 0;  // TCP/IP
  

  int line_number=0;
  int input_slot,input_chan;
 

askslot:
  // How to get a string/sentence with spaces
  printf("Please enter a slot to set up : \n>");
  std::cin >> input_slot;
  printf("You entered slot  %d\n",input_slot);
  // and verify slot number OK
  if((input_slot<0) || (input_slot>13)) {
    printf(" invalid slot number %d \n",input_slot);
    goto askslot;
  }

askchan:
  // How to get a string/sentence with spaces
  printf("Please enter a channel to set up : \n>");
  std::cin >> input_chan;
  printf("You entered channel  %d \n",input_chan);
   // and verify slot number OK
  if((input_chan<0) || (input_chan>47)) {
    printf(" invalid channel number %d \n",input_chan);
    goto askchan;
  }
    
  
  ret = CAENHV_InitSystem((CAENHV_SYSTEM_TYPE_t)2, link_type,(void*)address, "admin", "admin", &sysHndl);
  
  if( ret == CAENHV_OK ){
    printf("\n CAEN system online - handle = %d \n\n",sysHndl);
  } else {
    printf("\n CAEN HV connect error - No connection \n");
    return 1;
  }
  
  // end of login //


  // SET V0 TO INPUT VALUE //
  unsigned short slotnum, NrOfCh, numch, *listaCh, Ch; //*ChList;
  char ParName3[30] = "Pw";
  char ParName1[5] = "VMon";  
  unsigned int ON = 1;
  float HV;
   
  NrOfCh=1;
  int usize = NrOfCh*sizeof(unsigned short);
  listaCh = (unsigned short*) malloc(usize);

  slotnum=input_slot;
  numch=input_chan;
  listaCh[0] = numch;

  printf(" setting ON slot %d chan %d \n",slotnum,listaCh[0]);
  ret = CAENHV_SetChParam(sysHndl, slotnum, ParName3, NrOfCh, listaCh, &ON);
  printf(", Channel %s - CAENHV_SetChParam: %s (num. %d)\n\n", ParName3,CAENHV_GetError(sysHndl), ret);
  printf(" ... waiting for channel to ramp up .....\n");
   // sleep 10 secs to get channel safely up
   sleep(15);


  // and Get VMon
  ret1 = CAENHV_GetChParam(sysHndl, slotnum, ParName1, NrOfCh, listaCh, &HV);
  printf(", Channel %s - CAENHV_GetChParam: %s (num. %d)\n\n", ParName1,CAENHV_GetError(sysHndl), ret1);
  // anyhow print value
  printf(", slot: %d, channel: %d - HV = %10.2f  \n", slotnum,numch, HV);
 
  // and free allocated listaCh
  free(listaCh);

  // LOGOUT //
  
  ret = CAENHV_DeinitSystem(sysHndl);
  if(ret == CAENHV_OK) printf("CAENHV_DeinitSystem: Connection closed (num. %d)\n\n", ret);
  else printf("CAENHV_DeinitSystem: %s (num. %d)\n\n", CAENHV_GetError(sysHndl), ret);
  
  // end of logout //



  return 0;
}
