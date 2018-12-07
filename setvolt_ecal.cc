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
  std::ifstream input;  

  input.open ("CalorimeterMap15pC.txt");
 

  while(std::getline(input, line)) {
    if(line_number>0) {
      int ntoken=0;
      // cast string to char
      linec = strdup(line.c_str());

      //std::cout << "linec = " << linec << std::endl;
      int tokcount=0;
      int ik=line_number-1;
      while( (tok=strsep(&linec,";")) != NULL) {
	if(tokcount==0) inumber[ik]=atoi(tok);
	if(tokcount==1) x[ik]=atoi(tok);
	if(tokcount==2) y[ik]=atoi(tok);
	if(tokcount==3) slot[ik]=atoi(tok);
	if(tokcount==4) {
	  // strip 0 if present
	  //if(tok[0]=='0') tok[0]=' ';
	  nch[ik]=atoi(tok);
	}
	if(tokcount==7) hv[ik]=atof(tok);
	tokcount++;
	//printf(" line %d - token %d = %s \n",line_number,tokcount,tok);
      }
      printf(" line %d , inum=%d , x=%d , y=%d, slot=%d, nch=%d hv=%5.2f \n",
      	     ik,inumber[ik],x[ik],y[ik],slot[ik],nch[ik],hv[ik]);
    }
    line_number++;
    
    // std::cout<<line<<'\n';
  }

  printf(" Map file  is long %d lines \n",line_number);
  printf(" Ecal values read =  %d \n",line_number-1);
 
    
 
  printf("\n\n **************** setting HV to  channels ********** \n");
  // LOGIN //
  
  ret = CAENHV_InitSystem((CAENHV_SYSTEM_TYPE_t)2, link_type,(void*)address, "admin", "admin", &sysHndl);
  
  if( ret == CAENHV_OK ){
    printf("\n CAEN system online - handle = %d \n\n",sysHndl);
  } else {
    printf("\n CAEN HV connect error - No connection \n");
    return 1;
  }
  
  // end of login //

  for(int ic=0;ic<line_number-1;ic++) {

    // SET V0 TO INPUT VALUE //
    unsigned short slotnum, NrOfCh, numch, *listaCh, Ch; //*ChList;
    char ParName2[30] = "V0Set";
    float HV;

    NrOfCh=1;
    int usize = NrOfCh*sizeof(unsigned short);
    listaCh = (unsigned short*) malloc(usize);

    slotnum=slot[ic];
    numch=nch[ic];
    HV=hv[ic];
    listaCh[0] = numch;

    printf(" setting hv of slot %d chan %u to %5.2f \n",slotnum,listaCh[0],HV);

    ret = CAENHV_SetChParam(sysHndl, slotnum, ParName2, NrOfCh, listaCh, &HV);
    
    printf("\nCAENHV_SetChParam: %s (num. %d)\n\n", CAENHV_GetError(sysHndl), ret);

    free(listaCh);

  }

  // LOGOUT //
  
  ret = CAENHV_DeinitSystem(sysHndl);
  if(ret == CAENHV_OK) printf("CAENHV_DeinitSystem: Connection closed (num. %d)\n\n", ret);
  else printf("CAENHV_DeinitSystem: %s (num. %d)\n\n", CAENHV_GetError(sysHndl), ret);
  
  // end of logout //


  // and close the input file
  input.close();


  return 0;
}
