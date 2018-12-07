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
  bool alarm=true;
  bool debug=false;

 
   if( argc == 2 ) {
     printf("The argument supplied is %s\n", argv[1]);
     // default is alarm on - if 0 alarm=false
     if(argv[1]==0) alarm=false;
   }
   else if( argc > 2 ) {
      printf("Too many arguments supplied.\n");
      return 1;
   }
   else {
      printf("One argument expected.\n");
      return 1;
   }

  int line_number;
  int nchan_alarm_ecal=0;
  int nchan_alarm_sac=0;
  int nchan_alarm_cosm=0;
  int nchan_alarm_sipm=0;

  line_number=0;
  std::ifstream myfile("nchan_summ_alarm_ECALHV.txt");
  if (myfile.is_open()) {
    while ( getline (myfile,line) )    {
      std::cout << line << std::endl;
      linec = strdup(line.c_str());
      if(line_number==0) nchan_alarm_ecal=atoi(linec);
      //  if(line_number==1) nchan_alarm_sac=atoi(line);
      line_number++;
    }
    myfile.close();
  }  else {
    printf("Unable to open file \n"); 
  }


  line_number=0;
  std::ifstream myfile1("nchan_summ_alarm_SACHV.txt");
  if (myfile1.is_open()) {
    while ( getline (myfile1,line) )    {
      std::cout << line << std::endl;
      linec = strdup(line.c_str());
      if(line_number==0) nchan_alarm_sac=atoi(linec);
      line_number++;
    }
    myfile1.close();
  }  else {
    printf("Unable to open file \n"); 
  }

  line_number=0;
  std::ifstream myfile2("nchan_summ_alarm_cosmic.txt");
  if (myfile2.is_open()) {
    while ( getline (myfile2,line) )    {
      std::cout << line << std::endl;
      linec = strdup(line.c_str());
      if(line_number==0) nchan_alarm_cosm=atoi(linec);
      //  if(line_number==1) nchan_alarm_sac=atoi(line);
      line_number++;
    }
    myfile2.close();
  }  else {
    printf("Unable to open file \n"); 
  }

  line_number=0;
  std::ifstream myfile3("nchan_summ_alarm_hvsipm.txt");
  if (myfile3.is_open()) {
    while ( getline (myfile3,line) )    {
      std::cout << line << std::endl;
      linec = strdup(line.c_str());
      if(line_number==0) nchan_alarm_sipm=atoi(linec);
      //  if(line_number==1) nchan_alarm_sac=atoi(line);
      line_number++;
    }
    myfile3.close();
  }  else {
    printf("Unable to open file \n"); 
  }

  // and now send to monitor


  time_t ticks;
  struct tm * timeinfo;
  char timestamp[25];


  // new formatted timestring
  time (&ticks);
  timeinfo = localtime (&ticks);
  strftime (timestamp,25,"%Y-%m-%d %T",timeinfo);

 
 

  // and write data for monitoring alarm
  FILE * pFile5;
  std::string filename5="monitor/summ_alarm_ECALHV";
  filename5+=".txt";
  pFile5 = fopen (filename5.c_str(),"w");
  

  // write to alarm monitor
  // write values to file
  //status
  fprintf(pFile5,"PLOTID SUMM_HV_ALARM \n");
  fprintf(pFile5,"PLOTNAME HV Alarms %.24s\nPLOTTYPE activetext\n",timestamp);
  fprintf(pFile5,"DATA  [ {\"title\":\"ECAL alarm channels\",\"current\":{\"value\":\"%d\",\"col\":\"#00CC00\"},\"alarm\":{\"min\":\"-1\",\"max\":\"0\",\"col\":\"#FF0000\"}},{\"title\":\"SAC alarm channels\",\"current\":{\"value\":\"%d\",\"col\":\"#00CC00\"},\"alarm\":{\"min\":\"-1\",\"max\":\"0\",\"col\":\"#FF0000\"}},{\"title\":\"Cosmic alarm channels\",\"current\":{\"value\":\"%d\",\"col\":\"#00CC00\"},\"alarm\":{\"min\":\"-1\",\"max\":\"0\",\"col\":\"#FF0000\"}},{\"title\":\"Veto SiPM alarm channels\",\"current\":{\"value\":\"%d\",\"col\":\"#00CC00\"},\"alarm\":{\"min\":\"-1\",\"max\":\"0\",\"col\":\"#FF0000\"}} ]\n",nchan_alarm_ecal,nchan_alarm_sac,nchan_alarm_cosm,nchan_alarm_sipm);
  fclose(pFile5);

     

  // if alarm exist send it to monitor
  std::string scp5="scp -q ";
  scp5+=filename5;
  scp5+=" monitor@l0padme3:PadmeMonitor/watchdir/. ";
  // cout << " scp command " << scp2 << endl; 
  char * writable5 = new char[scp5.size() + 1];
  std::copy(scp5.begin(), scp5.end(), writable5);
  writable5[scp5.size()] = '\0'; // don't forget the terminating 0
  // scp to monitor@l0padme3
  system(writable5);
     

  return 0;
}
