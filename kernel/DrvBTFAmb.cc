#include "DrvBTFAmb.h"   //in c file
#include"fwk/utlCommaDashListToVector.h"
// #include <caenlib/CAENHVWrapper.h>
// #include "extern/CAENHVWrapper/CAENHVWrapper.h"
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */
#include <libmemcached/memcached.h>
#include <ctype.h>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <iomanip>

using namespace std;

 
  DrvBTFAmb_except&
DrvBTFAmb_except::BTFAmbRetStatus(int ambhandler,int retstatus, const std::string& add)
{
 std::string error=(
  retstatus==0     ? "Command wrapper correctly executed"                                            :
  retstatus==1     ? "Error of operatived system"                                                    :
  retstatus==2     ? "Write error in communication channel"                                          :
  retstatus==3     ? "Read error in communication channel"                                           :
  retstatus==4     ? "Time out in server communication"                                              :
  retstatus==5     ? "Command Front End application is down"                                         :
  retstatus==6     ? "Communication with system not yet connected by a Login command"                :
  "Unknown");


  std::string str("BTFAmbWrapper returned error ("+std::to_string(retstatus)+") :  "+error);
  if(add.size()>0)str+="   "+add;

  switch (retstatus){
    case 1             : throw DrvBTFAmb_except::Except_SYSERR               (str) ;break;
    case 2             : throw DrvBTFAmb_except::Except_WRITEERR             (str) ;break;
    case 3             : throw DrvBTFAmb_except::Except_READERR              (str) ;break;
    case 4             : throw DrvBTFAmb_except::Except_TIMEERR              (str) ;break;
    case 5             : throw DrvBTFAmb_except::Except_DOWN                 (str) ;break;
    case 6             : throw DrvBTFAmb_except::Except_NOTPRES              (str) ;break;
  }
}

  int
DrvBTFAmb::ComInit()
{
  int handle=0;
  return handle;    
}

  void
DrvBTFAmb::ComDeinit(int hadle)
{
  // close(handle);
  // int ret = BTFAmb_DeinitSystem(handle);
  // if(ret != AMB_OK)
  // DrvBTFAmb_except::BTFAmbRetStatus(handle,ret);
}



  void
DrvBTFAmb::AssertInit()
{
  INFO("DrvBTFAmb::AssertInit()");
  //#warning CAEN ComInit
  fBTFAmbHandle=ComInit();
  AssertInitAllOwned();
  //ProcessUpdateListTemp();
}

void
DrvBTFAmb::Finalize()
{
  INFO("DrvBTFAmb::Finalize()");
  int bufsize;
  this->VDeviceBase::Finalize();
  //#warning CAEN ComDeinit
  ComDeinit(fBTFAmbHandle );
  JoinThread();
}

void
DrvBTFAmb::DebugDump()
{
  
  std::string fTemp  ("23.0");
  std::string fRHumi ("45.0");
  std::string timestamp;


  time_t rawtime;
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  //printf ( "The current date/time is: %s", asctime (timeinfo) );
  timestamp=asctime(timeinfo);
  

  std::stringstream ss;
  ss.str(std::string());ss.clear();ss<<"fTemperature    "<<fTemp <<" @- "<<timestamp;INFO(ss.str());
  ss.str(std::string());ss.clear();ss<<"fRel Humidity   "<<fRHumi<<" @- "<<timestamp;INFO(ss.str());
   
}

void
DrvBTFAmb::OnCycleLocal()
{

  int handle;


  // sleep 30 seconds between cycles
  sleep(30);

  string command = "GET \"http://";
  string ipaddress=fIPAddress;
  string rest = "/epocasi.html\" | tail -3 > file.out";
  
  command+=ipaddress;
  command+=rest;

  const char * commandc=command.c_str();
  
  // execute command
  system(commandc);

  //delete commandc;
  
  int iline=0;					\
  float var[3];
  string line;
  string filename="file.out";
  ifstream myfile (filename);

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
  
  if(!myfile) {
    // cout << "Cannot open input file.\n";
    fprintf(stderr, "Couldn't open file : %s\n", filename.c_str());
    DrvBTFAmb_except::BTFAmbRetStatus(handle,3, "IP = "+fIPAddress+" ");
    // return 1;
  }
  
  int i=0;
  
  while(myfile) {
    getline(myfile,line);  // delim defaults to '\n'
    if(i<3) {
      // cout << "i = " << i << " " << line << endl;
      var[i]=stof(line);
    }
    i++;
  }
  
  myfile.close();
  
  printf(" BTFAmb - Starting client on %s \n",fIPAddress.c_str());  
  printf(" command is %s \n",commandc);
  printf("Timestamp = %s \n",timestamp);
  for(int ii=0;ii<3;ii++) {
    printf(" var %d = %f \n",ii,var[ii]);
  }
  
  system("rm file.out");
  
}

