#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>     /* atoi */
#include <math.h>       /* round, floor, ceil, trunc */
#include <time.h>

int main (int argc, char *argv[]) {

  std::string line;
  char *linec;
  char *tok;
  char *inumber1[10];
  int slot1[10];
  int nch1[10];
  float hv[10];
  float current[10];
  float status[10];
  char* timestamp;
  bool alarm=true;
  bool debug=false;

  int line_number=0;
  std::ifstream input;  
  
  if( argc == 2 ) {
    printf("The argument supplied is %s\n", argv[1]);
    // default is alarm on - if 0 alarm=false
    if(strcmp(argv[1],"0")==9) alarm=false;
  }
  else if( argc > 2 ) {
    printf("Too many arguments supplied.\n");
    return 1;
  }
  else {
    printf("One argument expected.\n");
    return 1;
  }

  // now read the actual data file
  int line1_number=0;
  std::ifstream input1;  

  input1.open ("data/CAENCosmic");

  while(std::getline(input1, line)) {
    if(line1_number==0) {
     int tokcount1=0;     
     // cast string to char
     linec = strdup(line.c_str());
     while( (tok=strsep(&linec,";")) != NULL) {
       if(debug) printf(" tok %d = %s \n",tokcount1,tok);
	if(tokcount1==0) timestamp=tok;
	tokcount1++;
     }
    }
    if(line1_number>0) {
      int ntoken1=0;
      // cast string to char
      linec = strdup(line.c_str());

      //std::cout << "linec = " << linec << std::endl;
      int tokcount1=0;
      int ik=line1_number-1;
      while( (tok=strsep(&linec,";")) != NULL) {
	if(tokcount1==0) slot1[ik]=atoi(tok);
	if(tokcount1==1) {
	  // strip 0 if present
	  //if(tok[0]=='0') tok[0]=' ';
	  nch1[ik]=atoi(tok);
	}
	if(tokcount1==2) inumber1[ik]=tok;
	if(tokcount1==3) hv[ik]=atof(tok);
	if(tokcount1==4) current[ik]=atof(tok);
	if(tokcount1==5) status[ik]=atof(tok);
	tokcount1++;
	//printf(" line %d - token %d = %s \n",line_number,tokcount,tok);
      }
      //printf(" line1 %d , inum=%s , slot=%d, nch=%d , hv=%f, current=%f \n",
      //	     ik,inumber1[ik],slot1[ik],nch1[ik],hv[ik],current[ik]);
    }
    line1_number++;
    // std::cout<<line<<'\n';
  }
  printf(" Data file  is long %d lines \n",line1_number);
  printf(" Cosmics HV  values read =  %d \n",line1_number-1);


  // and close the input file
  input1.close();


  if(debug) {
  
    printf("Timestamp= %.24s \n",timestamp);
    // and now print resulting matrix
 
   for (int ichan=0;ichan<line1_number-1;ichan++) {
     printf(" channel %s - HV=%.2f - I =%.2f - status=%.2f \n",inumber1[ichan],
	    hv[ichan],current[ichan],status[ichan]);
    }
 
  }

  /*
  time_t ticks;
  struct tm * timeinfo;
  char timestamp[25];

  // old human readable timestamp
  // time ( &rawtime );
  //timeinfo = localtime ( &rawtime );
  //timestamp=asctime(timeinfo);

  // new formatted timestring
  time (&ticks);
  timeinfo = localtime (&ticks);
  strftime (timestamp,25,"%Y-%m-%d %T",timeinfo);
  */

  int ioff_channels=0;

  if(alarm) {
    FILE * pFile2;
    std::string filename2="alarm/cosmic_alarm.txt";
    pFile2 = fopen (filename2.c_str(),"w");
    fprintf(pFile2," <span foreground=\"red\" font=\"32\">DCS Alarm \n Cosmics HV Alarm\n at %.24s \n",timestamp);

    // check alarm only for first 2 channels
    printf("Cosmics  - Checking channels for alarms \n");
    // check strange values for HV and current
    for (int ix=0;ix<2;ix++) {

      if(hv[ix]<50. || current[ix]<200. ) {
	printf(" ALARM : Cosmic channel %s is at HV %5.2f , current %5.2f \n slot %d - channel %d \n",inumber1[ix],hv[ix],current[ix],slot1[ix],nch1[ix]); 
	fprintf(pFile2," <i> check channel %s - HV=%5.2f - slot %d, channel %d  \n</i> ",inumber1[ix],hv[ix],slot1[ix],nch1[ix]);
	ioff_channels++;
      }
    
    }

    // printf("\n");
    // And close alarm file
    fprintf(pFile2," </span> ");
    fclose(pFile2);

    // and send alarm if there are off channels
    if(ioff_channels>0) {
      system("sendmessage_cosmics");
    }

  } else {
    printf(" ALARMS disabled ! \n");
  }

  printf(" Cosmics alarm channels = %d \n",ioff_channels);

  // AND PRODUCE THE DATA FILE FOR THE MONITORING

  // and write data for monitoring
  FILE * pFile3;
  std::string filename3="monitor/CosmicsHV";
  filename3+=".txt";
  pFile3 = fopen (filename3.c_str(),"w");


  // write values to file
  fprintf(pFile3,"PLOTID Cosmics_HV \nPLOTNAME HV for Cosmics scintillators \nPLOTTYPE activetext \n");
  fprintf(pFile3,"DATA  [ {\"title\":\"Timestamp\",\"current\":{\"value\":\"%.24s\"} },",timestamp);
  
  
  // check only first 4 channels
  // ---  check only first 2 channels for the moment
  for(int nch = 0; nch < 2; nch++) {
   
    // and write file for Monitor
    if(nch>0) { 
      fprintf(pFile3,",");
    }
    // and write to monitor
    fprintf(pFile3,"{\"title\":\" %s status\",\"current\":{\"value\":\"%.2f\",\"col\":\"#00CC00\"} } ",inumber1[nch],status[nch]); 
    fprintf(pFile3,",{\"title\":\"Voltage \",\"current\":{\"value\":\"%.2f\",\"col\":\"#00CC00\"},\"alarm\":{\"min\":\"1500.\",\"max\":\"2200.\"} }",hv[nch]);
    fprintf(pFile3,",{\"title\":\"Current\",\"current\":{\"value\":\"%.2f\",\"col\":\"#00CC00\"},\"alarm\":{\"min\":\"1000.\",\"max\":\"1600.\"} }",current[nch]);
    // printf("\n");
    
  }  // end for loop	

  // close buffer for monitor
  fprintf(pFile3," ]\n");
  // and close output files
  fclose(pFile3);

  // and copy file to monitor@l0padme3
  std::string scp2="scp -q "+filename3+" monitor@l0padme3:PadmeMonitor/watchdir/. ";
  // cout << " scp command " << scp2 << endl; 
  char * writable2 = new char[scp2.size() + 1];
  std::copy(scp2.begin(), scp2.end(), writable2);
  writable2[scp2.size()] = '\0'; // don't forget the terminating 0
  // scp to monitor@l0padme3
  system(writable2);

  // and write data for monitoring alarm
  FILE * pFile5;
  std::string filename5="nchan_summ_alarm_cosmics.txt";
  pFile5 = fopen (filename5.c_str(),"w");

  // anad print to file how many alarm channels
  fprintf(pFile5," %d\n",ioff_channels);
  fclose(pFile5);


  return 0;
}
