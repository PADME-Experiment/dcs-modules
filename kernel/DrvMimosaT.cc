#include "DrvMimosaT.h"   //in c file
#include"fwk/utlCommaDashListToVector.h"
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <bits/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */
#include <libmemcached/memcached.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

#define SOCKETS_IP_ADDRESS			"169.168.62.132"
#define SOCKETS_PORT				5025
#define SOCKETS_BUFFER_SIZE			3000
#define SOCKETS_TIMEOUT				10

 
  DrvMimosaT_except&
DrvMimosaT_except::MimosaTRetStatus(int ambhandler,int retstatus, const std::string& add)
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


  std::string str("MimosaTWrapper returned error ("+std::to_string(retstatus)+") :  "+error);
  if(add.size()>0)str+="   "+add;

  switch (retstatus){
    case 1             : throw DrvMimosaT_except::Except_SYSERR               (str) ;break;
    case 2             : throw DrvMimosaT_except::Except_WRITEERR             (str) ;break;
    case 3             : throw DrvMimosaT_except::Except_READERR              (str) ;break;
    case 4             : throw DrvMimosaT_except::Except_TIMEERR              (str) ;break;
    case 5             : throw DrvMimosaT_except::Except_DOWN                 (str) ;break;
    case 6             : throw DrvMimosaT_except::Except_NOTPRES              (str) ;break;
  }
}

  int
DrvMimosaT::ComInit()
{
  int handle=0;
  int MySocket,MyControlPort;
  char SocketsBuffer[SOCKETS_BUFFER_SIZE];
  struct sockaddr_in MyAddress,MyControlAddress;
  unsigned int ControlPort;
  int status;
  int retval;
  bool debug=false;

  const char *address=fIPAddress.c_str();
  // Create socket (allocate resources)
  if((MySocket=socket(
		      PF_INET, // IPv4
		      SOCK_STREAM, // TCP
		      0))==-1) {
    printf("Error: Unable to create socket (%i)...\n",errno);
    perror("sockets"); // Print error message based on errno
    exit(1);
    DrvMimosaT_except::MimosaTRetStatus(MySocket,1,"IP = "+fIPAddress+" ");
  }
	
  // Establish TCP connection
  memset(&MyAddress,0,sizeof(struct sockaddr_in)); // Set structure to zero
  MyAddress.sin_family=PF_INET; // IPv4
  MyAddress.sin_port=htons(SOCKETS_PORT); // Port number (in network order)
  MyAddress.sin_addr.s_addr=
    inet_addr(address); // IP address (in network order)
  if(connect(MySocket,(struct sockaddr *)&MyAddress,
	     sizeof(struct sockaddr_in))==-1) {
    printf("Error: Unable to establish connection to socket (%i)...\n",
	   errno);
    DrvMimosaT_except::MimosaTRetStatus(MySocket,6,"IP = "+fIPAddress+" ");

    // perror("sockets"); // Print error message based on errno
    // exit(1);
  }

  printf(" ComInit - Connected to server %s \n",address);

  // and closae the sockets
  close(MySocket);

  handle = MySocket;
  return handle;    
}

  void
DrvMimosaT::ComDeinit(int handle)
{

}



  void
DrvMimosaT::AssertInit()
{
  INFO("DrvMimosaT::AssertInit()");
  //#warning CAEN ComInit
  fMimosaTHandle=ComInit();
  AssertInitAllOwned();
  //ProcessUpdateListTemp();
}

void
DrvMimosaT::Finalize()
{
  INFO("DrvMimosaT::Finalize()");
  int bufsize;
  this->VDeviceBase::Finalize();
  //#warning CAEN ComDeinit
  ComDeinit(fMimosaTHandle );
  JoinThread();
}

void
DrvMimosaT::DebugDump()
{
  
   
}

void
DrvMimosaT::OnCycleLocal()
{
  
  int handle;
  int MySocket,MyControlPort;
  char SocketsBuffer[SOCKETS_BUFFER_SIZE];
  struct sockaddr_in MyAddress,MyControlAddress;
  unsigned int ControlPort;
  int status;
  int retval;
  bool debug=false;

  const char *address=fIPAddress.c_str();
  
  // Create socket (allocate resources)
  if((MySocket=socket(
		      PF_INET, // IPv4
		      SOCK_STREAM, // TCP
		      0))==-1) {
    printf("Error: Unable to create socket (%i)...\n",errno);
    perror("sockets"); // Print error message based on errno
    DrvMimosaT_except::MimosaTRetStatus(MySocket,1,"IP = "+fIPAddress+" ");
    // exit(1);
    return;
  }
	
  // Establish TCP connection
  memset(&MyAddress,0,sizeof(struct sockaddr_in)); // Set structure to zero
  MyAddress.sin_family=PF_INET; // IPv4
  MyAddress.sin_port=htons(SOCKETS_PORT); // Port number (in network order)
  MyAddress.sin_addr.s_addr=
    inet_addr(address); // IP address (in network order)
  if(connect(MySocket,(struct sockaddr *)&MyAddress,
	     sizeof(struct sockaddr_in))==-1) {
    printf("Error: Unable to establish connection to socket (%i)...\n",
	   errno);
    DrvMimosaT_except::MimosaTRetStatus(MySocket,6,"IP = "+fIPAddress+" ");

    //perror("sockets"); // Print error message based on errno
    // exit(1);
    return;
  }

  if(debug) printf(" OnCycleLocal - Connected to server %s \n",address);

  // Clear status and reset instrument
  WriteString(MySocket,"*CLS\n");

  // Get instrument's ID string
  WriteString(MySocket,"*IDN?\n");
  ReadString(MySocket,SocketsBuffer);
  //printf("Instrument ID: %s\n",SocketsBuffer);
	
  // Ask for control port
  WriteString(MySocket,"SYST:COMM:TCPIP:CONTROL?\n");
  if(ReadString(MySocket,SocketsBuffer)==0) {
      printf("Warning: No response from instrument (control port).\n");
      DrvMimosaT_except::MimosaTRetStatus(MySocket,2,"IP = "+fIPAddress+" ");
      close(MySocket);
      // exit(1);
      return;
}

  //printf(" Sockets buffer = %s \n",SocketsBuffer);
  sscanf(SocketsBuffer,"%u",&ControlPort);
  if(debug) printf("Control Port: %u\n",ControlPort);
	
  // printf(" create socket for control port \n");
  // Create socket for control port
  if((MyControlPort=socket(PF_INET,SOCK_STREAM,0))==-1) {
      printf("Error: Unable to create control port socket (%i)...\n",errno);
       DrvMimosaT_except::MimosaTRetStatus(MySocket,3,"IP= "+fIPAddress+" ");
       //perror("sockets"); // Print error message based on errno
       close(MySocket);
       return;
       // exit(1);
  }

  //printf(" establishing connection to control port \n");

  // Establish TCP connection to control port
  memset(&MyControlAddress,0,sizeof(struct sockaddr_in)); // Set structure to zero
  MyControlAddress.sin_family=PF_INET; // IPv4
  MyControlAddress.sin_port=htons(ControlPort); // Port number (in network order)
  MyControlAddress.sin_addr.s_addr=inet_addr(address); // IP address (in network order)
  if(connect(MyControlPort,(struct sockaddr *)&MyControlAddress,
	     sizeof(struct sockaddr_in))==-1) {
    printf("Error: Unable to establish connection to control port (%i)...\n",
	   errno);
    DrvMimosaT_except::MimosaTRetStatus(MySocket,3,"IP= "+fIPAddress+" ");
    // perror("sockets"); // Print error message based on errno
    close(MySocket);
    // exit(1);
    return;
  }

  //printf(" doing a device clear \n");

  // Do a device clear
  DeviceClear(MyControlPort,SocketsBuffer);
       

  int iret;

  // read first Mimosa channels

  // set channel 1 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@301);:TEMP:TRAN:TYPE TC,(@301);TEMP:TRAN:TC:TYPE K,(@301);\n");
  iret=WriteString(MySocket,"RJUN:TYPE INT,(@301);:TEMP:NPLC 1,(@301);:TEMP:TRAN:TC:CHECK ON,(@301);:ZERO:AUTO ON,(@301);:UNIT:TEMP C,(@301);\n");
  //printf(" Iret set 301 =%d \n",iret);
   
  
  // set channel 2 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@302);:TEMP:TRAN:TYPE TC,(@302);:TEMP:TRAN:TC:TYPE K,(@302);\n");
  iret=WriteString(MySocket,"RJUN:TYPE INT,(@302);:TEMP:NPLC 1,(@302);:TEMP:TRAN:TC:CHECK ON,(@302);:ZERO:AUTO ON,(@302);:UNIT:TEMP C,(@302)\n");
  //printf(" Iret set 301 =%d \n",iret);

  // printf(" doing a scan \n");
  // now do a scan
  iret=WriteString(MySocket,":ROUT:SCAN (@301,302)\n");

  iret=WriteString(MySocket,
		   ":TRIG:SOUR TIM;:TRIG:TIM 2;:TRIG:COUN 1\n");
  iret=WriteString(MySocket,":INIT\n");

  // wait for the scan to complete
  sleep(2);

  
  time_t rawtime;
  struct tm * timeinfo;
  char timestamp[25];
  int lengthstr;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  // new formatted timestring
  strftime (timestamp,25,"%Y-%m-%d %T",timeinfo);
  lengthstr=strlen(timestamp);
  //printf(" timestamp length = %d \n",lengthstr);
  timestamp[lengthstr]='\0';
 

  // and read data
  //iret=WriteString(MySocket,'FETC?\n');
  iret=WriteString(MySocket,":DATA:PEEK? 2,0\n");
  iret=ReadString(MySocket,SocketsBuffer);
  if(iret==0){
      printf("Warning: No response from instrument (control port).\n");
      DrvMimosaT_except::MimosaTRetStatus(MySocket,2,"IP= "+fIPAddress+" ");
      // exit(1);
      return;
  } else {
    if(debug) {
      printf(" Read socket buffer length =%d \n",iret);
      printf(" SCAN DATA read = %s \n",SocketsBuffer);
    }
  }


  // and decode the string
  char * token[800];
  char *tok;
  int ntoken;
  ntoken=0;
  char *socket1;
  socket1=strdup(SocketsBuffer);
  
  // and decode string
  while((tok=strsep(&socket1,",")) != NULL) {
    token[ntoken]=tok;
    //printf(" %d - %s \n",ntoken,tok);
    ntoken++;
  }

  if(ntoken>8) {
 
   // and write to file
   FILE * pFile;
   FILE * pFile_app;
   // int n;
   std::string filename1="data/MimosaT";
   std::string filename_app="history/MimosaT";
   pFile = fopen (filename1.c_str(),"w");
   pFile_app = fopen (filename_app.c_str(),"a");

   // and send value to Monitor
   FILE * pFile3;
   std::string filename3="monitor/MimosaT.txt";
   pFile3 = fopen (filename3.c_str(),"w");

   //printf(" tokens found = %d \n",ntoken);
   int nreads=ntoken/9;
   int ik;

   // write values to file
   fprintf(pFile3,"PLOTID Mimosa_Temp \nPLOTNAME Mimosa Temperature probes \nPLOTTYPE activetext \n");
   fprintf(pFile3,"DATA  [ ");

   fprintf(pFile,"%.24s;%d\n",timestamp,nreads);
   fprintf(pFile_app,"%.24s;%d\n",timestamp,nreads);
   
   double tempf[50];
   int numchan[50];
   char mystamp[25];
      
   for (ik=0;ik<nreads;ik++) {
     char *temp1;
     char *temp2;
     char *temp3;
     char tempc[15];
     int mydates[3];
     int mytimes[2];
     //double mytimes1;
     int mytimes1;
     int errflag;
     int istart=ik*9;
 
     //printf(" ik=%d istart=%d \n",ik,istart);
     temp1="";
     temp1=token[istart];
     //int length=strlen(temp);
     //printf(" length =%d \n",length);
     //temp[length]='\0';
     strncpy(tempc,temp1,15);
     //printf(" tempc = %s \n",tempc);
     tempf[ik]=atof(tempc);
     // now decode time
     mydates[0]=atoi(token[istart+3]);
     mydates[1]=atoi(token[istart+2]);
     mydates[2]=atoi(token[istart+1]);
     mytimes[0]=atoi(token[istart+4]);
     mytimes[1]=atoi(token[istart+5]);
     mytimes1=int(atof(token[istart+6]));
     
     temp2="";
     temp2=token[istart+7];
     // int length=strlen(temp2);
     // printf(" length =%d \n",length);
     //printf(" temp2 = %s \n",temp2);
     numchan[ik]=atoi(temp2);
     
     temp3="";
     temp3=token[istart+8];
     errflag=atoi(temp3);
     // printf("\n");
     // printf(" time = %d-%d-%d %d:%d:%f \n",mydates[0],mydates[1],mydates[2],mytimes[0],mytimes[1],mytimes1);
     //     sprintf(mystamp,"%d-%d-%d %d:%d:%d",mydates[2],mydates[1],mydates[0],mytimes[0],mytimes[1],mytimes1);

     if(debug) {
       //printf(" timestamp = %s \n",mystamp);
       printf(" timestamp = %s \n",timestamp);
       printf(" read %d - channel %d - T = %f - err = %d \n",ik,numchan[ik],tempf[ik],errflag);
     }
     
     
     // write values to file
     fprintf(pFile,"%d;%d;%f;%d \n",ik,numchan[ik],tempf[ik],errflag);
     fprintf(pFile_app,"%d;%d;%f;%d \n",ik,numchan[ik],tempf[ik],errflag);
     
     // print only on last value of the 2 monitor
     if(numchan[ik]==302) { 
       fprintf(pFile3,",");
     }

     if(numchan[ik]==301 || numchan[ik]==302) {
       // and write to monitor only mimosa
       fprintf(pFile3,"{\"title\":\"Channel %d Timestamp\",\"current\":{\"value\":\"%.24s\"}}",numchan[ik],timestamp); 
       fprintf(pFile3,",{\"title\":\"Temperature - C\",\"current\":{\"value\":\"\%.2f\",\"col\":\"#00CC00\"}}",tempf[ik]);
       fprintf(pFile3,",{\"title\":\"Error flag\",\"current\":{\"value\":\"\%d\",\"col\":\"#00CC00\"}}",errflag);
       // printf("\n");
     }
   }
   
   // close buffer for monitor
   fprintf(pFile3," ]\n");

   // and close written file
   fclose(pFile);
   fclose(pFile_app);
   fclose(pFile3);

   // and copy file to monitor@l0padme3
   std::string scp2="scp -q ";
   scp2+=filename3;
   scp2+= " monitor@l0padme3:PadmeMonitor/watchdir/. ";
   // cout << " scp command " << scp2 << endl; 
   char * writable2 = new char[scp2.size() + 1];
   std::copy(scp2.begin(), scp2.end(), writable2);
   writable2[scp2.size()] = '\0'; // don't forget the terminating 0
   // scp to monitor@l0padme3
   system(writable2);

   
  } // endif ntokens
    
  // Do a device clear
  DeviceClear(MyControlPort,SocketsBuffer);


  // read seconmd ECAL  channels
  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@101);:TEMP:TRAN:TYPE RTD,(@101);:RTD:TYPE 85,(@101);OCOM Off,(@101);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@101);\n");

  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@102);:TEMP:TRAN:TYPE RTD,(@102);:RTD:TYPE 85,(@102);OCOM Off,(@102);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@102);\n");

  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@103);:TEMP:TRAN:TYPE RTD,(@103);:RTD:TYPE 85,(@103);OCOM Off,(@103);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@103);\n");

  // set channel 104 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@104);:TEMP:TRAN:TYPE RTD,(@104);:RTD:TYPE 85,(@104);OCOM Off,(@104);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@104);\n");

  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@105);:TEMP:TRAN:TYPE RTD,(@105);:RTD:TYPE 85,(@105);OCOM Off,(@105);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@105);\n");
	
  // add other 2 channels at 101,104
  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@106);:TEMP:TRAN:TYPE RTD,(@106);:RTD:TYPE 85,(@106);OCOM Off,(@106);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@106);\n");
  // add other 2 channels at 101,104

  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@107);:TEMP:TRAN:TYPE RTD,(@107);:RTD:TYPE 85,(@107);OCOM Off,(@107);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@107);\n");

  // add other 2 channels at 101,104
  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@108);:TEMP:TRAN:TYPE RTD,(@108);:RTD:TYPE 85,(@108);OCOM Off,(@108);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@108);\n");

  // add other 2 channels at 101,104
  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@109);:TEMP:TRAN:TYPE RTD,(@109);:RTD:TYPE 85,(@109);OCOM Off,(@109);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@109);\n");

  // add other 2 channels at 101,104
  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@110);:TEMP:TRAN:TYPE RTD,(@110);:RTD:TYPE 85,(@110);OCOM Off,(@110);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@110);\n");


  // add other 2 channels at 101,104
  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@111);:TEMP:TRAN:TYPE RTD,(@111);:RTD:TYPE 85,(@111);OCOM Off,(@111);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@111);\n");

  // add other 2 channels at 101,104
  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@112);:TEMP:TRAN:TYPE RTD,(@112);:RTD:TYPE 85,(@112);OCOM Off,(@112);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@112);\n");

  // add other 2 channels at 101,104
  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@113);:TEMP:TRAN:TYPE RTD,(@113);:RTD:TYPE 85,(@113);OCOM Off,(@113);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@113);\n");

  // set channel 104 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@114);:TEMP:TRAN:TYPE RTD,(@114);:RTD:TYPE 85,(@114);OCOM Off,(@114);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@114);\n");

  // add other 2 channels at 101,104
  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@115);:TEMP:TRAN:TYPE RTD,(@115);:RTD:TYPE 85,(@115);OCOM Off,(@115);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@115);\n");
	
  // add other 2 channels at 101,104
  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@116);:TEMP:TRAN:TYPE RTD,(@116);:RTD:TYPE 85,(@116);OCOM Off,(@116);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@116);\n");
  // add other 2 channels at 101,104

  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@117);:TEMP:TRAN:TYPE RTD,(@117);:RTD:TYPE 85,(@117);OCOM Off,(@117);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@117);\n");

  // add other 2 channels at 101,104
  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@118);:TEMP:TRAN:TYPE RTD,(@118);:RTD:TYPE 85,(@118);OCOM Off,(@118);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@118);\n");


  // printf(" doing a scan \n");
  // now do a scan
  iret=WriteString(MySocket,":ROUT:SCAN (@101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118)\n");

  iret=WriteString(MySocket,
		   ":TRIG:SOUR TIM;:TRIG:TIM 5;:TRIG:COUN 1\n");
  iret=WriteString(MySocket,":INIT\n");

  // wait for the scan to complete
  sleep(10);

  
  // and read data
  //iret=WriteString(MySocket,'FETC?\n');
  iret=WriteString(MySocket,":DATA:PEEK? 20,0\n");
  iret=ReadString(MySocket,SocketsBuffer);
  if(iret==0){
      printf("Warning: No response from instrument (control port).\n");
      DrvMimosaT_except::MimosaTRetStatus(MySocket,2,"IP= "+fIPAddress+" ");
      // exit(1);
      return;
  } else {
    if(debug) {
      printf(" Read socket buffer length =%d \n",iret);
      printf(" SCAN DATA read = %s \n",SocketsBuffer);
    }
  }


  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  // new formatted timestring
  strftime (timestamp,25,"%Y-%m-%d %T",timeinfo);
  lengthstr=strlen(timestamp);
  //printf(" timestamp length = %d \n",lengthstr);
  timestamp[lengthstr]='\0';

  ntoken=0;
  socket1=strdup(SocketsBuffer);
  
  // and decode string
  while((tok=strsep(&socket1,",")) != NULL) {
    token[ntoken]=tok;
    //printf(" %d - %s \n",ntoken,tok);
    ntoken++;
  }

  if(ntoken>8) {
 
   // and write to file
   FILE * pFile;
   FILE * pFile_app;
   // int n;
   std::string filename1="data/EcalTL";
   std::string filename_app="history/EcalTL";
   pFile = fopen (filename1.c_str(),"w");
   pFile_app = fopen (filename_app.c_str(),"a");

   // and send value to Monitor
   FILE * pFile3L1;
   FILE * pFile3L2;

   std::string filename3L1="monitor/EcalTL1.txt";
   pFile3L1 = fopen (filename3L1.c_str(),"w");
   std::string filename3L2="monitor/EcalTL2.txt";
   pFile3L2 = fopen (filename3L2.c_str(),"w");

   //printf(" tokens found = %d \n",ntoken);
   int nreads=ntoken/9;
   int ik;


   // write values to file
   fprintf(pFile3L1,"PLOTID ECAL_Temp_L1\nPLOTNAME Ecal Temperature Left 1  \nPLOTTYPE activetext \n");
   fprintf(pFile3L1,"DATA  [ {\"title\":\"Timestamp\",\"current\":{\"value\":\"%.24s\"}}",timestamp);
   // write values to file
   fprintf(pFile3L2,"PLOTID ECAL_Temp_L2 \nPLOTNAME Ecal Temperature Left 2 \nPLOTTYPE activetext \n");
   fprintf(pFile3L2,"DATA  [ {\"title\":\"Timestamp\",\"current\":{\"value\":\"%.24s\"}}",timestamp);
   fprintf(pFile,"%.24s;%d\n",timestamp,nreads);
   fprintf(pFile_app,"%.24s;%d\n",timestamp,nreads);
   
   for (ik=0;ik<nreads;ik++) {
     char *temp1;
     char *temp2;
     char *temp3;
     char tempc[15];
     double tempf[50];
     int mydates[3];
     int mytimes[2];
     //double mytimes1;
     int mytimes1;
     int numchan[50];
     int errflag;
     char mystamp[25];
     int istart=ik*9;
 
     //printf(" ik=%d istart=%d \n",ik,istart);
     temp1="";
     temp1=token[istart];
     //int length=strlen(temp);
     //printf(" length =%d \n",length);
     //temp[length]='\0';
     strncpy(tempc,temp1,15);
     //printf(" tempc = %s \n",tempc);
     tempf[ik]=atof(tempc);
     // now decode time
     mydates[0]=atoi(token[istart+3]);
     mydates[1]=atoi(token[istart+2]);
     mydates[2]=atoi(token[istart+1]);
     mytimes[0]=atoi(token[istart+4]);
     mytimes[1]=atoi(token[istart+5]);
     mytimes1=int(atof(token[istart+6]));
     
     temp2="";
     temp2=token[istart+7];
     // int length=strlen(temp2);
     // printf(" length =%d \n",length);
     //printf(" temp2 = %s \n",temp2);
     numchan[ik]=atoi(temp2);
     
     temp3="";
     temp3=token[istart+8];
     errflag=atoi(temp3);
     // printf("\n");
     // printf(" time = %d-%d-%d %d:%d:%f \n",mydates[0],mydates[1],mydates[2],mytimes[0],mytimes[1],mytimes1);
     //sprintf(mystamp,"%d-%d-%d %d:%d:%d",mydates[2],mydates[1],mydates[0],mytimes[0],mytimes[1],mytimes1);

     if(debug) {
       printf(" timestamp = %s \n",timestamp);
       printf(" read %d - channel %d - T = %f - err = %d \n",ik,numchan[ik],tempf[ik],errflag);
     }
     
     
     // write values to file
     fprintf(pFile,"%d;%d;%.2f;%d \n",ik,numchan[ik],tempf[ik],errflag);
     fprintf(pFile_app,"%d;%d;%.2f;%d \n",ik,numchan[ik],tempf[ik],errflag);
     
     // and write to monitor all ECAL Temp
     if(numchan[ik]>100 && numchan[ik]<111) {
       fprintf(pFile3L1,",{\"title\":\"Channel %d T - C\",\"current\":{\"value\":\"\%.2f\",\"col\":\"#00CC00\"}}",numchan[ik],tempf[ik]);
       fprintf(pFile3L1,",{\"title\":\"Error flag\",\"current\":{\"value\":\"\%d\",\"col\":\"#00CC00\"}}",errflag);
     }
     if(numchan[ik]>110 && numchan[ik]<121) {
       fprintf(pFile3L2,",{\"title\":\"Channel %d T - C\",\"current\":{\"value\":\"\%.2f\",\"col\":\"#00CC00\"}}",numchan[ik],tempf[ik]);
       fprintf(pFile3L2,",{\"title\":\"Error flag\",\"current\":{\"value\":\"\%d\",\"col\":\"#00CC00\"}}",errflag);
     }
     // printf("\n");    
   }
   
   // close buffer for monitor
   fprintf(pFile3L1," ]\n");
   fprintf(pFile3L2," ]\n");

   // and close written file
   fclose(pFile);
   fclose(pFile_app);
   fclose(pFile3L1);
   fclose(pFile3L2);
 
  
   // and copy file to monitor@l0padme3
   std::string scp2="scp -q ";
   scp2+=filename3L1;
   scp2+= " monitor@l0padme3:PadmeMonitor/watchdir/. ";
   // cout << " scp command " << scp2 << endl; 
   char * writable2 = new char[scp2.size() + 1];
   std::copy(scp2.begin(), scp2.end(), writable2);
   writable2[scp2.size()] = '\0'; // don't forget the terminating 0
   // scp to monitor@l0padme3
   system(writable2);

   // and copy file to monitor@l0padme3
   std::string scp3="scp -q ";
   scp3+=filename3L2;
   scp3+= " monitor@l0padme3:PadmeMonitor/watchdir/. ";
   // cout << " scp command " << scp2 << endl; 
   char * writable3 = new char[scp3.size() + 1];
   std::copy(scp3.begin(), scp3.end(), writable3);
   writable3[scp3.size()] = '\0'; // don't forget the terminating 0
   // scp to monitor@l0padme3
   system(writable3);
  
   
  } // endif ntokens
    
  // Do a device clear
  DeviceClear(MyControlPort,SocketsBuffer);
  
  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@201);:TEMP:TRAN:TYPE RTD,(@201);:RTD:TYPE 85,(@201);OCOM Off,(@201);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@201);\n");

  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@202);:TEMP:TRAN:TYPE RTD,(@202);:RTD:TYPE 85,(@202);OCOM Off,(@202);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@202);\n");

  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@203);:TEMP:TRAN:TYPE RTD,(@203);:RTD:TYPE 85,(@203);OCOM Off,(@203);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@203);\n");

  // set channel 104 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@204);:TEMP:TRAN:TYPE RTD,(@204);:RTD:TYPE 85,(@204);OCOM Off,(@204);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@204);\n");

  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@205);:TEMP:TRAN:TYPE RTD,(@205);:RTD:TYPE 85,(@205);OCOM Off,(@205);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@205);\n");
	
  // add other 2 channels at 101,104
  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@206);:TEMP:TRAN:TYPE RTD,(@206);:RTD:TYPE 85,(@206);OCOM Off,(@206);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@206);\n");
  // add other 2 channels at 101,104

  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@207);:TEMP:TRAN:TYPE RTD,(@207);:RTD:TYPE 85,(@207);OCOM Off,(@207);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@207);\n");

  // add other 2 channels at 101,104
  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@208);:TEMP:TRAN:TYPE RTD,(@208);:RTD:TYPE 85,(@208);OCOM Off,(@208);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@208);\n");

  // add other 2 channels at 101,104
  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@209);:TEMP:TRAN:TYPE RTD,(@209);:RTD:TYPE 85,(@209);OCOM Off,(@209);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@209);\n");

  // add other 2 channels at 101,104
  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@210);:TEMP:TRAN:TYPE RTD,(@210);:RTD:TYPE 85,(@210);OCOM Off,(@210);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@210);\n");


  // add other 2 channels at 101,104
  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@211);:TEMP:TRAN:TYPE RTD,(@211);:RTD:TYPE 85,(@211);OCOM Off,(@211);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@211);\n");

  // add other 2 channels at 101,104
  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@212);:TEMP:TRAN:TYPE RTD,(@212);:RTD:TYPE 85,(@212);OCOM Off,(@212);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@212);\n");

  // add other 2 channels at 101,104
  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@213);:TEMP:TRAN:TYPE RTD,(@213);:RTD:TYPE 85,(@213);OCOM Off,(@213);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@213);\n");

  // set channel 104 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@214);:TEMP:TRAN:TYPE RTD,(@214);:RTD:TYPE 85,(@214);OCOM Off,(@214);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@214);\n");

  // add other 2 channels at 101,104
  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@215);:TEMP:TRAN:TYPE RTD,(@215);:RTD:TYPE 85,(@215);OCOM Off,(@215);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@215);\n");
	
  // add other 2 channels at 101,104
  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@216);:TEMP:TRAN:TYPE RTD,(@216);:RTD:TYPE 85,(@216);OCOM Off,(@216);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@216);\n");
  // add other 2 channels at 101,104

  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@217);:TEMP:TRAN:TYPE RTD,(@217);:RTD:TYPE 85,(@217);OCOM Off,(@217);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@217);\n");

  // add other 2 channels at 101,104
  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@218);:TEMP:TRAN:TYPE RTD,(@218);:RTD:TYPE 85,(@218);OCOM Off,(@218);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@218);\n");

  // add other 2 channels at 101,104
  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@219);:TEMP:TRAN:TYPE RTD,(@219);:RTD:TYPE 85,(@219);OCOM Off,(@219);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@219);\n");
  // add other 2 channels at 101,104

  // set channel 101 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@220);:TEMP:TRAN:TYPE RTD,(@220);:RTD:TYPE 85,(@220);OCOM Off,(@220);\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@220);\n");

  // printf(" doing a scan \n");
  // now do a scan
  iret=WriteString(MySocket,":ROUT:SCAN (@201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220)\n");

  iret=WriteString(MySocket,
		   ":TRIG:SOUR TIM;:TRIG:TIM 5;:TRIG:COUN 1\n");
  iret=WriteString(MySocket,":INIT\n");

  // wait for the scan to complete
  sleep(10);

  
  // and read data
  //iret=WriteString(MySocket,'FETC?\n');
  iret=WriteString(MySocket,":DATA:PEEK? 20,0\n");
  iret=ReadString(MySocket,SocketsBuffer);
  if(iret==0){
      printf("Warning: No response from instrument (control port).\n");
      DrvMimosaT_except::MimosaTRetStatus(MySocket,2,"IP= "+fIPAddress+" ");
      // exit(1);
      return;
  } else {
    if(debug) {
      printf(" Read socket buffer length =%d \n",iret);
      printf(" SCAN DATA read = %s \n",SocketsBuffer);
    }
  }

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  // new formatted timestring
  strftime (timestamp,25,"%Y-%m-%d %T",timeinfo); 
  lengthstr=strlen(timestamp);
  //printf(" timestamp length = %d \n",lengthstr);
  timestamp[lengthstr]='\0';

  ntoken=0;
  socket1=strdup(SocketsBuffer);
  
  // and decode string
  while((tok=strsep(&socket1,",")) != NULL) {
    token[ntoken]=tok;
    //printf(" %d - %s \n",ntoken,tok);
    ntoken++;
  }

  if(ntoken>8) {
 
   FILE * pFile;
   FILE * pFile_app;
   // and write to file
   // int n;
   std::string filename1="data/EcalTR";
   std::string filename_app="history/EcalTR";
   pFile = fopen (filename1.c_str(),"w");
   pFile_app = fopen (filename_app.c_str(),"a");

   // and send value to Monitor
   FILE * pFile3R1;
   FILE * pFile3R2;

   std::string filename3R1="monitor/EcalTR1.txt";
   pFile3R1 = fopen (filename3R1.c_str(),"w");
   std::string filename3R2="monitor/EcalTR2.txt";
   pFile3R2 = fopen (filename3R2.c_str(),"w");

   //printf(" tokens found = %d \n",ntoken);
   int nreads=ntoken/9;
   int ik=0;
  

   fprintf(pFile3R1,"PLOTID ECAL_Temp_R1 \nPLOTNAME Ecal Temperature Right 1 \nPLOTTYPE activetext \n");
   fprintf(pFile3R1,"DATA  [ {\"title\":\"Timestamp\",\"current\":{\"value\":\"%.24s\"}}",timestamp);
   fprintf(pFile3R2,"PLOTID ECAL_Temp_R2 \nPLOTNAME Ecal Temperature Right 2 \nPLOTTYPE activetext \n");
   fprintf(pFile3R2,"DATA  [ {\"title\":\"Timestamp\",\"current\":{\"value\":\"%.24s\"}}",timestamp);

   fprintf(pFile,"%.24s;%d\n",timestamp,nreads);
   fprintf(pFile_app,"%.24s;%d\n",timestamp,nreads);
   
   for (ik=0;ik<nreads;ik++) {
     char *temp1;
     char *temp2;
     char *temp3;
     char tempc[15];
     double tempf[50];
     int mydates[3];
     int mytimes[2];
     //double mytimes1;
     int mytimes1;
     int numchan[50];
     int errflag;
     char mystamp[25];
     int istart=ik*9;
 
     //printf(" ik=%d istart=%d \n",ik,istart);
     temp1="";
     temp1=token[istart];
     //int length=strlen(temp);
     //printf(" length =%d \n",length);
     //temp[length]='\0';
     strncpy(tempc,temp1,15);
     //printf(" tempc = %s \n",tempc);
     tempf[ik]=atof(tempc);
     // now decode time
     mydates[0]=atoi(token[istart+3]);
     mydates[1]=atoi(token[istart+2]);
     mydates[2]=atoi(token[istart+1]);
     mytimes[0]=atoi(token[istart+4]);
     mytimes[1]=atoi(token[istart+5]);
     mytimes1=int(atof(token[istart+6]));
     
     temp2="";
     temp2=token[istart+7];
     // int length=strlen(temp2);
     // printf(" length =%d \n",length);
     //printf(" temp2 = %s \n",temp2);
     numchan[ik]=atoi(temp2);
     
     temp3="";
     temp3=token[istart+8];
     errflag=atoi(temp3);
     // printf("\n");
     // printf(" time = %d-%d-%d %d:%d:%f \n",mydates[0],mydates[1],mydates[2],mytimes[0],mytimes[1],mytimes1);
     //sprintf(mystamp,"%d-%d-%d %d:%d:%d",mydates[2],mydates[1],mydates[0],mytimes[0],mytimes[1],mytimes1);

     if(debug) {
       printf(" timestamp = %s \n",timestamp);
       printf(" read %d - channel %d - T = %f - err = %d \n",ik,numchan[ik],tempf[ik],errflag);
     }
     
     
     // write values to file
     fprintf(pFile,"%d;%d;%.2f;%d \n",ik,numchan[ik],tempf[ik],errflag);
     fprintf(pFile_app,"%d;%d;%.2f;%d \n",ik,numchan[ik],tempf[ik],errflag);
     
     if(numchan[ik]>200 && numchan[ik]<211) {
       fprintf(pFile3R1,",{\"title\":\"Channel %d T - C\",\"current\":{\"value\":\"\%.2f\",\"col\":\"#00CC00\"}}",numchan[ik],tempf[ik]);
       fprintf(pFile3R1,",{\"title\":\"Error flag\",\"current\":{\"value\":\"\%d\",\"col\":\"#00CC00\"}}",errflag);
     }
     if(numchan[ik]>210 && numchan[ik]<221) {
       fprintf(pFile3R2,",{\"title\":\"Channel %d T - C\",\"current\":{\"value\":\"\%.2f\",\"col\":\"#00CC00\"}}",numchan[ik],tempf[ik]);
       fprintf(pFile3R2,",{\"title\":\"Error flag\",\"current\":{\"value\":\"\%d\",\"col\":\"#00CC00\"}}",errflag);
     }
     // printf("\n");
    
   }
   
   // close buffer for monitor
   fprintf(pFile3R1," ]\n");
   fprintf(pFile3R2," ]\n");

   // and close written file
   fclose(pFile);
   fclose(pFile_app);
   fclose(pFile3R1);
   fclose(pFile3R2);

  
   // and copy file to monitor@l0padme3
   std::string scp2="scp -q ";
   scp2+=filename3R1;
   scp2+= " monitor@l0padme3:PadmeMonitor/watchdir/. ";
   // cout << " scp command " << scp2 << endl; 
   char * writable2 = new char[scp2.size() + 1];
   std::copy(scp2.begin(), scp2.end(), writable2);
   writable2[scp2.size()] = '\0'; // don't forget the terminating 0
   // scp to monitor@l0padme3
   system(writable2);

   // and copy file to monitor@l0padme3
   std::string scp3="scp -q ";
   scp3+=filename3R2;
   scp3+= " monitor@l0padme3:PadmeMonitor/watchdir/. ";
   // cout << " scp command " << scp2 << endl; 
   char * writable3 = new char[scp3.size() + 1];
   std::copy(scp3.begin(), scp3.end(), writable3);
   writable3[scp3.size()] = '\0'; // don't forget the terminating 0
   // scp to monitor@l0padme3
   system(writable3);
  
   
  } // endif ntokens
  


  // and close sockets after everything done
  close(MyControlPort);
  close(MySocket);

  // and sleep for 60 sec
  sleep(40);

}

int DrvMimosaT::WriteString(int MySocket,string stringa)
{
	int retval;
	//printf(" sent buffer %s  size = %d \n",string,length);
	const char *stringac=stringa.c_str();
	int length=strlen(stringac);
	retval=send(MySocket,stringac,length,0);
	if(retval==-1) {
		printf("Error: Unable to send message (%i)...\n",errno);
		perror("sockets"); // Print error message based on errno
		exit(1);
	}
	return retval;
}

int DrvMimosaT::WaitForData(int MySocket)
{
	fd_set MyFDSet;
	struct timeval tv;
	int retval;
	
	// Wait for data to become available
	FD_ZERO(&MyFDSet); // Initialize fd_set structure
	FD_SET(MySocket,&MyFDSet); // Add socket to "watch list"
	tv.tv_sec=SOCKETS_TIMEOUT; tv.tv_usec=0; // Set timeout
	retval=select(MySocket+1,&MyFDSet,NULL,NULL,&tv); // Wait for change
	
	// Interpret return value
	if(retval==-1) {
		printf("Error: Problem with select (%i)...\n",errno);
		perror("sockets"); // Print error message based on errno
		exit(1);
	}
	return retval; // 0 = timeout, 1 = socket status has changed
}

int DrvMimosaT::ReadString(int MySocket,char *buffer)
{
	int actual;
	
	// Wait for data to become available
	if(WaitForData(MySocket)==0)
	{
		// Timeout
		printf("Warning: Timeout...\n");
		return 0;
	}
	
	// Read data
	if((actual=recv(MySocket,buffer,SOCKETS_BUFFER_SIZE,0))==-1) {
		printf("Error: Unable to receice data (%i)...\n",errno);
		perror("sockets"); // Print error message based on errno
		exit(1);
	}
	else {
      	buffer[actual]=0;
	}
	return actual;
}

void DrvMimosaT::DeviceClear(int MySocket,char *buffer)
{
	WriteString(MySocket,"DCL\n");
	if(ReadString(MySocket,buffer)==0)
		return;
	if(strcmp(buffer,"DCL\n")==0) {
	  // printf("DCL\\n received back from instrument...\n");
	} else {
		printf("Warning: DCL response: %s\n",buffer);
	}
	return;
}

