#include "DrvEcalT.h"   //in c file
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

#define SOCKETS_IP_ADDRESS			"169.168.62.120"
#define SOCKETS_PORT				5025
#define SOCKETS_BUFFER_SIZE			1024
#define SOCKETS_TIMEOUT				10

 
  DrvEcalT_except&
DrvEcalT_except::EcalTRetStatus(int ambhandler,int retstatus, const std::string& add)
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


  std::string str("EcalTWrapper returned error ("+std::to_string(retstatus)+") :  "+error);
  if(add.size()>0)str+="   "+add;

  switch (retstatus){
    case 1             : throw DrvEcalT_except::Except_SYSERR               (str) ;break;
    case 2             : throw DrvEcalT_except::Except_WRITEERR             (str) ;break;
    case 3             : throw DrvEcalT_except::Except_READERR              (str) ;break;
    case 4             : throw DrvEcalT_except::Except_TIMEERR              (str) ;break;
    case 5             : throw DrvEcalT_except::Except_DOWN                 (str) ;break;
    case 6             : throw DrvEcalT_except::Except_NOTPRES              (str) ;break;
  }
}

  int
DrvEcalT::ComInit()
{
  int handle=0;
  int MySocket,MyControlPort;
  char SocketsBuffer[1024];
  struct sockaddr_in MyAddress,MyControlAddress;
  unsigned int ControlPort;
  int status;
  int retval;

  const char *address=fIPAddress.c_str();
  // Create socket (allocate resources)
  if((MySocket=socket(
		      PF_INET, // IPv4
		      SOCK_STREAM, // TCP
		      0))==-1) {
    printf("Error: Unable to create socket (%i)...\n",errno);
    perror("sockets"); // Print error message based on errno
    exit(1);
    DrvEcalT_except::EcalTRetStatus(MySocket,1,"IP = "+fIPAddress+" ");
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
    DrvEcalT_except::EcalTRetStatus(MySocket,6,"IP = "+fIPAddress+" ");

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
DrvEcalT::ComDeinit(int handle)
{

  // soket always closed after readout
  
  //close(handle);
  // int ret = EcalT_DeinitSystem(handle);
  // if(ret != AMB_OK)
  // DrvEcalT_except::EcalTRetStatus(handle,ret);
}



  void
DrvEcalT::AssertInit()
{
  INFO("DrvEcalT::AssertInit()");
  //#warning CAEN ComInit
  fEcalTHandle=ComInit();
  AssertInitAllOwned();
  //ProcessUpdateListTemp();
}

void
DrvEcalT::Finalize()
{
  INFO("DrvEcalT::Finalize()");
  int bufsize;
  this->VDeviceBase::Finalize();
  //#warning CAEN ComDeinit
  ComDeinit(fEcalTHandle );
  JoinThread();
}

void
DrvEcalT::DebugDump()
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
DrvEcalT::OnCycleLocal()
{
  
  int handle;
  int MySocket,MyControlPort;
  char SocketsBuffer[1024];
  struct sockaddr_in MyAddress,MyControlAddress;
  unsigned int ControlPort;
  int status;
  int retval;

  const char *address=fIPAddress.c_str();
  
  // Create socket (allocate resources)
  if((MySocket=socket(
		      PF_INET, // IPv4
		      SOCK_STREAM, // TCP
		      0))==-1) {
    printf("Error: Unable to create socket (%i)...\n",errno);
    perror("sockets"); // Print error message based on errno
    DrvEcalT_except::EcalTRetStatus(MySocket,1,"IP = "+fIPAddress+" ");
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
    DrvEcalT_except::EcalTRetStatus(MySocket,6,"IP = "+fIPAddress+" ");

    //perror("sockets"); // Print error message based on errno
    // exit(1);
    return;
  }

  printf(" OnCycleLocal - Connected to server %s \n",address);

  // Minimize latency by setting TCP_NODELAY option
  SetNODELAY(MySocket);
	
  // Clear status and reset instrument
  WriteString(MySocket,"*CLS;*RST\n");

  // Get instrument's ID string
  WriteString(MySocket,"*IDN?\n");
  ReadString(MySocket,SocketsBuffer);
  //printf("Instrument ID: %s\n",SocketsBuffer);
	
  // Ask for control port
  WriteString(MySocket,"SYST:COMM:TCPIP:CONTROL?\n");
  if(ReadString(MySocket,SocketsBuffer)==0) {
      printf("Warning: No response from instrument (control port).\n");
      DrvEcalT_except::EcalTRetStatus(MySocket,2,"IP = "+fIPAddress+" ");
      close(MySocket);
      // exit(1);
      return;
}

  //printf(" Sockets buffer = %s \n",SocketsBuffer);
  sscanf(SocketsBuffer,"%u",&ControlPort);
  printf("Control Port: %u\n",ControlPort);
	
  // printf(" create socket for control port \n");
  // Create socket for control port
  if((MyControlPort=socket(PF_INET,SOCK_STREAM,0))==-1) {
      printf("Error: Unable to create control port socket (%i)...\n",errno);
       DrvEcalT_except::EcalTRetStatus(MySocket,3,"IP= "+fIPAddress+" ");
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
    DrvEcalT_except::EcalTRetStatus(MySocket,3,"IP= "+fIPAddress+" ");
    // perror("sockets"); // Print error message based on errno
    close(MySocket);
    // exit(1);
    return;
  }

  //printf(" doing a device clear \n");

  // Do a device clear
  DeviceClear(MyControlPort,SocketsBuffer);
       

  int iret;

  // printf(" set channel 1 as monitor channels for scan \n");
  // set channel 1 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@101);:TEMP:TRAN:TYPE RTD,(@101);:RTD:TYPE 85,(@101);OCOM Off,(@101)\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@101)\n");

  // printf(" set channel 4 as monitor channels for scan \n");
  // set channel 4 as monitor channel
  iret=WriteString(MySocket,":FUNC \"TEMP\",(@104);:TEMP:TRAN:TYPE RTD,(@104);:RTD:TYPE 85,(@104);OCOM Off,(@104)\n");
  iret=WriteString(MySocket,":TEMP:TRAN:RTD:RES 1000,(@104)\n");
	

  // printf(" doing a scan \n");
  // now do a scan
  iret=WriteString(MySocket,":ROUT:SCAN (@101,104)\n");

  iret=WriteString(MySocket,
		   ":TRIG:SOUR TIM;:TRIG:TIM 2;:TRIG:COUN 2\n");
  iret=WriteString(MySocket,":INIT\n");

  // wait for the scan to complete
  sleep(6);

  /*
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
  */

  // and read data
  //iret=WriteString(MySocket,'FETC?\n');
  iret=WriteString(MySocket,":DATA:PEEK? 10,0\n");
  iret=ReadString(MySocket,SocketsBuffer);
  if(iret==0){
      printf("Warning: No response from instrument (control port).\n");
      DrvEcalT_except::EcalTRetStatus(MySocket,2,"IP= "+fIPAddress+" ");
      // exit(1);
      return;
  } else {
    // printf(" Read socket buffer length =%d \n",iret);
    // printf(" SCAN DATA read = %s \n",SocketsBuffer);
  }

  // and decode the string
  char * token[200];
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
   // int n;
   std::string filename1="data/EcalT";
   pFile = fopen (filename1.c_str(),"w");

   //printf(" tokens found = %d \n",ntoken);
   int nreads=ntoken/9;
   int ik;
   for (ik=0;ik<nreads;ik++) {
     char *temp1;
     char *temp2;
     char *temp3;
     char tempc[15];
     double tempf[50];
     int mydates[3];
     int mytimes[2];
     double mytimes1;
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
      mytimes1=atof(token[istart+6]);
    
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
      sprintf(mystamp,"%d-%d-%d %d:%d:%f",mydates[0],mydates[1],mydates[2],mytimes[0],mytimes[1],mytimes1);
      printf(" timestamp = %s \n",mystamp);
      printf(" read %d - channel %d - T = %f - err = %d \n",ik,numchan[ik],tempf[ik],errflag);

        
      // write values to file
      fprintf(pFile,"%.24s;%d;%d;%f;%d \n",mystamp,ik,numchan[ik],tempf[ik],errflag);

      // printf("\n");

   }

   // and close written file
   fclose(pFile);
   
  }
    
  
  // and close sockets after everything done
  close(MyControlPort);
  close(MySocket);

}

int DrvEcalT::WriteString(int MySocket,string stringa)
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

int DrvEcalT::WaitForData(int MySocket)
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

int DrvEcalT::ReadString(int MySocket,char *buffer)
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
	if((actual=recv(MySocket,buffer,1024,0))==-1) {
		printf("Error: Unable to receice data (%i)...\n",errno);
		perror("sockets"); // Print error message based on errno
		exit(1);
	}
	else {
      	buffer[actual]=0;
	}
	return actual;
}

void DrvEcalT::DeviceClear(int MySocket,char *buffer)
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

void DrvEcalT::SetNODELAY(int MySocket)
{
	int StateNODELAY = 1; // Turn NODELAY on
	int ret;
	
	ret=setsockopt(MySocket, // Handle to socket connection
		IPPROTO_TCP, // Protocol level (TCP)
		TCP_NODELAY, // Option on this level (NODELAY)
		(void *)&StateNODELAY, // Pointer to option variable
		sizeof StateNODELAY); // Size of option variable
	
	if(ret==-1) {
		printf("Error: Unable to set NODELAY option (%i)...\n",errno);
		perror("sockets"); // Print error message based on errno
		exit(1);
	}
	return;
}

