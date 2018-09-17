#include "DrvHVSipm.h"   //in c file
#include"fwk/utlCommaDashListToVector.h"
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
#include <stdio.h>
#include <string.h>

using namespace std;
int jjj=0;

  DrvHVSipm_except&
DrvHVSipm_except::HVSipmRetStatus(int ambhandler,int retstatus, const std::string& add)
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


  std::string str("HVSipmWrapper returned error ("+std::to_string(retstatus)+") :  "+error);
  if(add.size()>0)str+="   "+add;

  switch (retstatus){
    case 1             : throw DrvHVSipm_except::Except_SYSERR               (str) ;break;
    case 2             : throw DrvHVSipm_except::Except_WRITEERR             (str) ;break;
    case 3             : throw DrvHVSipm_except::Except_READERR              (str) ;break;
    case 4             : throw DrvHVSipm_except::Except_TIMEERR              (str) ;break;
    case 5             : throw DrvHVSipm_except::Except_DOWN                 (str) ;break;
    case 6             : throw DrvHVSipm_except::Except_NOTPRES              (str) ;break;
  }
}

  int
DrvHVSipm::ComInit()
{
  // this is the version reading the json string
 
  return 0;
}

  void
DrvHVSipm::ComDeinit(int handle)
{
  // close(handle);
  // int ret = HVSipm_DeinitSystem(handle);
  // if(ret != AMB_OK)
  // DrvHVSipm_except::HVSipmRetStatus(handle,ret);
}



  void
DrvHVSipm::AssertInit()
{
  INFO("DrvHVSipm::AssertInit()");
  //#warning CAEN ComInit
  fHVSipmHandle=ComInit();
  AssertInitAllOwned();
  //ProcessUpdateListTemp();
}

void
DrvHVSipm::Finalize()
{
  INFO("DrvHVSipm::Finalize()");
  
  int bufsize;
  this->VDeviceBase::Finalize();
  //#warning CAEN ComDeinit
  ComDeinit(fHVSipmHandle);
  JoinThread();
}

void
DrvHVSipm::DebugDump()
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
DrvHVSipm::OnCycleLocal()
{
  int handle;
  FILE *fp;
  int i;
  int r;
  int lenbuf;
  int lenbuf0;
  int tokcount;
  int tokcount0;
  int tokcount1;
  char *tok0;
  char *tok1;
  char *tok;
  char *tokens0[500];
  char *tokens1[500];
  int portNum = 11211; // port number
  char timestring[27];
  char *string0;

  #define bufSize 3000 // buffer size
  char buffer[bufSize]; // buffer to transmit
  char buffer1[bufSize]; // buffer to transmit

  #define CHUNK 3000 /* read max 3000 bytes at a time */
  char buf1[CHUNK];
  char buf0[CHUNK];
  
  size_t value_length;
  uint32_t flags;
  unsigned int mccount;


  bool isExit=false;

  //for(int jjj=0; jjj< 1000; jjj++) {
	char *js= NULL;
	char *js0= NULL;
	int nread0=0;
	int nread=0;
  	int ichan =-1;
	jjj++;

  	// first sleep 2 sec to be sure readout timing is OK
	// usleep(2000000);

	char * serv = new char[fIPAddress.size() + 1];
	std::copy(fIPAddress.begin(), fIPAddress.end(), serv);
	serv[fIPAddress.size()] = '\0'; // don't forget the terminating 0

	string wget_str="wget http://";
	string brdcfg="/brdcfg.cgx";
	string brdusr="/brdusr.cgx";
	string command_cfg;
	string command_usr;


	command_cfg+=wget_str;
	command_cfg+=serv;
	command_usr+=wget_str;
	command_usr+=serv;
	command_cfg+=brdcfg;
	command_usr+=brdusr;
	cout << " command_cfg " << command_cfg << endl; 
	cout << " command_usr " << command_usr << endl; 

	char * writable1 = new char[command_cfg.size() + 1];
	std::copy(command_cfg.begin(), command_cfg.end(), writable1);
	writable1[command_cfg.size()] = '\0'; // don't forget the terminating 0
	// GET THE json string from NIM module
 	//system("wget http://192.168.62.50/brdusr.cgx");
	system(writable1);

	char * writable2 = new char[command_usr.size() + 1];
	std::copy(command_usr.begin(), command_usr.end(), writable2);
	writable2[command_usr.size()] = '\0'; // don't forget the terminating 0
	// GET THE json string from NIM module
 	//system("wget http://192.168.62.50/brdcfg.cgx");
	system(writable2);

	delete [] writable1;
	delete [] writable2;
	delete [] serv;

	// get timestamp 
	time_t ticks;
	ticks = time(NULL);
	snprintf(timestring, 25, "%.25s", ctime(&ticks));

	
	// get json string from file
	// Open file for reading 
	fp = fopen("brdcfg.cgx", "r");
	
	if(fp) {
	  // printf(" Opened file brdcfg.cgx \n");
	  while ((nread0 = fread(buf0, 1, sizeof buf0, fp)) > 0) {
	    //printf(" nread = %u \n",nread);
	  }
	  // fwrite(buf0, 1, nread, stdout);
	  if (ferror(fp)) {
	    // deal with error 
	    fprintf(stderr, " error opening brdcfg.cgx  \n");
	    DrvHVSipm_except::HVSipmRetStatus(handle,3, "IP = "+fIPAddress+" ");
	    // printf(" error reading file \n");
	  }
	  // and close the file	  
	  fclose(fp);
	}
	// and remove the file
	system("rm brdcfg.cgx");
	
	
	// get json string from call to nim module
	lenbuf0=strlen(buf0);
	// printf(" lenbuf0 =%d \n",lenbuf0);
	// memset(js0, '\0', lenbuf0);
	
	// copy string here
	js0=strndup(buf0,lenbuf0);
	
	lenbuf0=strlen(js0);
	// complete the string with 0
	// buf0[lenbuf-1]=0;
	// printf(" js0 length = %d \n",lenbuf0);
	
	if(lenbuf0>0) {
	  
	  r=strlen(js0);
	  
	  tokcount=0;
	  tokcount0=0;
	  // printf(" r= %d \n",r);
	  while( (tok0=strsep(&js0,"{[,]}")) != NULL) {
	    if(strcmp(tok0,"") != 0 ) {
	      // printf(" %d token =  %s \n",tokcount,tok0);
	      while( (tok=strsep(&tok0,":")) != NULL) {
		// printf( " %d tok0= %s \n",tokcount0,tok);
		tokens0[tokcount0]=tok;
		tokcount0++;
	      }
	      tokcount++;
	    }
	  }
	  // reset counter to get special character at end
	  //tokcount0=tokcount0-2;
	  
	  // printf(" TOKCOUNT0 = %d \n",tokcount0);
	  
	} else {
	  //printf(" error reading brdcfg.cgx \n");
	  //printf( " lenbuf = 0 - exiting \n");
	  fprintf(stderr, " error reading brdcfg.cgx  \n");
	  DrvHVSipm_except::HVSipmRetStatus(handle,3, "IP = "+fIPAddress+" ");
	}  // endif lenbuf0
	
	/*
	  int iloop=0;	
	  for(iloop=0;iloop<tokcount0;iloop++) {
	  printf(" iloop = %d   token0 = %s \n",iloop,tokens0[iloop]);	
	  }
	*/
	
	//  {"id":"","ipAdr":"192.168.0.100","macAdr":"62:A4:17:80:FF:0D","name":"PADME-NIM","sn":"FG001","hwVer":"1.0","swVer":"1.0.168"}
	char* options0[7] = { "\"id\"", "\"ipAdr\"", "\"macAdr\"","\"name\"",
				  "\"sn\"","\"hwVer\"", "\"swVer\""};
	
	char *macadr0[17];
	char *name;
	char *snum;
	char *hwver;
	char *swver;
	char* macaddr;
	char* mac1;
	char* mac2;
	char* mac3;
	char* mac4;
	char* mac5;
	char* mac6;

	// here parse the string
	int itok;
	for (itok=0;itok<tokcount0;itok++) {
	  // printf(" token %d = %s \n",itok,tokens[itok]);
	  if(strcmp(tokens0[itok],options0[2])  ==0 ) {
	    mac1=tokens0[itok+1];
	    mac2=tokens0[itok+2];
	    mac3=tokens0[itok+3];
	    mac4=tokens0[itok+4];		  
	    mac5=tokens0[itok+5];
	    mac6=tokens0[itok+6];		  
	    itok=itok+6;
	  }
	  if(strcmp(tokens0[itok],options0[3])  ==0 ) {
	    name=tokens0[itok+1];
	    itok++;
	  }
	  if(strcmp(tokens0[itok],options0[4])  ==0 ) {
	    snum=tokens0[itok+1];
	    itok++;
	  }
	  if(strcmp(tokens0[itok],options0[5])  ==0 ) {
	    hwver=tokens0[itok+1];
	    itok++;
	  }
	  if(strcmp(tokens0[itok],options0[6])  ==0 ) {
	    swver=tokens0[itok+1];
	    itok++;
	  }
	}

       
	// set the string for memcache
	// int n1=sprintf(string0," %.11s; %.7s; %.7s; %.9s; %.3s:%.3s:%.3s:%.3s:%.3s:%.3s;",name,snum,hwver,swver,mac1,mac2,mac3,mac4,mac5,mac6);
       

	// get json string from file
	// Open file for reading 
	fp = fopen("brdusr.cgx", "r");
	
	if(fp) {
	  // printf(" Opened file brdusr.cgx \n");
	  while ((nread = fread(buf1, 1, sizeof buf1, fp)) > 0) {
	    // printf(" nread = %u \n",nread);
	  }
	  // fwrite(buf1, 1, nread, stdout);
	  if (ferror(fp)) {
	    // deal with error 
	    fprintf(stderr, " error opening brdusr.cgx  \n");
	    DrvHVSipm_except::HVSipmRetStatus(handle,3, "IP = "+fIPAddress+" ");
	    // printf(" error reading file \n");
	    // return 1;
	  }
	  // and close the file	  
	  fclose(fp);
	}
	// and remove the file
	system("rm brdusr.cgx");

	// get json string from call to nim module
	lenbuf=strlen(buf1);
	// printf(" lenbuf =%d \n",lenbuf);
	// memset(js, '\0', lenbuf);
	
	// copy string here
	js=strndup(buf1,lenbuf);
	
	lenbuf=strlen(js);
	// complete the string with 0
	// buf1[lenbuf-1]=0;
	// printf(" js length = %d \n",lenbuf);
	if(lenbuf>0) {
	  
	  r=strlen(js);
	  
	  tokcount=0;
	  tokcount1=0;
	  // printf(" r= %d \n",r);
	  while( (tok1=strsep(&js,"{[,]}")) != NULL) {
	    if(strcmp(tok1,"") != 0 ) {
	      // printf(" %d token =  %s \n",tokcount,tok1);
	      
	      while( (tok=strsep(&tok1,":")) != NULL) {
		// printf( " %d tok1= %s \n",tokcount1,tok);
		tokens1[tokcount1]=tok;
		tokcount1++;
	      }
	      tokcount++;
	    }
	  }
	  // reset counter to get special character at end
	  tokcount1=tokcount1-2;
	  
	  //printf(" TOKCOUNT = %d \n",tokcount1);

	  int idval[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	  int chanval[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	  int cardsts[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	  float hvreq[16]={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
	  float hvlvl[16]={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
	  float cardtemp[16]={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
	  float sipmcurr[16]={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
	  float sipmtemp[16]={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
	  float cardsupplyv[16]={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
	  int errsts[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	  int errcnt[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	  int lasterr[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	  
	  char* options[12] = { "\"id\"", "\"ch\"", "\"cardSts\"","\"hvReq\"",
				"\"hvLvl\"","\"cardTemp\"",
				"\"apdCurrent\"","\"apdTemp\"","\"cardSsupplyV\"",
				"\"errSts\"","\"errCnt\"","\"lastErr\""};
	  
	  // here parse the string
	  int itok;
	  for (itok=0;itok<tokcount1;itok++) {
	    // printf(" token %d = %s \n",itok,tokens[itok]);
	    if(strcmp(tokens1[itok],options[0])  ==0 ) {
	      ichan++;
	      idval[ichan]=ichan;
	      itok++;
	    }
	    if(strcmp(tokens1[itok],options[1])  ==0 ) {
	      chanval[ichan]=atoi(tokens1[itok+1]);
	      itok++;
	    }
	    if(strcmp(tokens1[itok],options[2])  ==0 ) {
	      cardsts[ichan]=atoi(tokens1[itok+1]);
	      itok++;
	    }
	    if(strcmp(tokens1[itok],options[3])  ==0 ) {
	      hvreq[ichan]=atof(tokens1[itok+1]);
	      itok++;
	    }
	    if(strcmp(tokens1[itok],options[4])  ==0 ) {
	      hvlvl[ichan]=atof(tokens1[itok+1]);
	      itok++;
	    }
	    if(strcmp(tokens1[itok],options[5])  ==0 ) {
	      cardtemp[ichan]=atof(tokens1[itok+1]);
	      itok++;
	    }
	    if(strcmp(tokens1[itok],options[6])  ==0 ) {
	      sipmcurr[ichan]=atof(tokens1[itok+1]);
	      itok++;
	    }
	    if(strcmp(tokens1[itok],options[7])  ==0 ) {
	      sipmtemp[ichan]=atof(tokens1[itok+1]);
	      itok++;
	    }
	    
	    if(strcmp(tokens1[itok],options[8])  ==0 ) {
	      cardsupplyv[ichan]=atof(tokens1[itok+1]);
	      itok++;
	    }
	    if(strcmp(tokens1[itok],options[9])  ==0 ) {
	      errsts[ichan]=atoi(tokens1[itok+1]);
	      itok++;
	    }
	    if(strcmp(tokens1[itok],options[10])  ==0 ) {
	      errcnt[ichan]=atoi(tokens1[itok+1]);
	      itok++;
	    }
	    if(strcmp(tokens1[itok],options[11])  ==0 ) {
	      lasterr[ichan]=atoi(tokens1[itok+1]);
	      itok++;
	      
	    }
	  }
	  
	  // add 1 to ichan
	  ichan++;
	  // printf(" ichan = %d \n",ichan);
	  int ik;
	  char string[60];

	  /*
	  // initialize buffer1 with timestring
	  int n2=sprintf(buffer1,"%.25s;",timestring);
	  // and add string0 to buffer1
	  strcat(buffer1,string0);	  
	  printf(" buffer1 = %s  \n",buffer1);
	  //buffer1[0]='\0';
	  */

	  printf(" Loop = %d - Timestamp = %25s  \n",jjj,timestring);
	  printf(" %.11s; %.7s; %.7s; %.9s; %.3s:%.3s:%.3s:%.3s:%.3s:%.3s;",name,snum,hwver,swver,mac1,mac2,mac3,mac4,mac5,mac6);
	  printf("\n id  chan status hvset  hvlvl  cardT  sipmI  sipmT errSt  errcnt lasterr\n");
	  for (ik=0;ik<ichan;ik++) {
	    printf("%4d %4d %4d   %5.2f  %5.2f  %5.2f  %5.2f  %5.2f %5d %5d  %5d \n",
		   idval[ik],chanval[ik],cardsts[ik],hvreq[ik],hvlvl[ik],cardtemp[ik],
		   sipmcurr[ik],sipmtemp[ik],errsts[ik],errcnt[ik],lasterr[ik]);
	  

	    /*
	    // set the string for memcache
	    sprintf(string,"%3d;%3d;%3d;%5.2f;%5.2f;%5.2f;%5.2f;%5.2f;%4d;%4d;%4d",
		    idval[ik],chanval[ik],cardsts[ik],hvreq[ik],hvlvl[ik],cardtemp[ik],
		    sipmcurr[ik],sipmtemp[ik],errsts[ik],errcnt[ik],lasterr[ik]);
	    // and add it to buffer1
	    strcat(buffer1,string);
	    */
	  }
	  printf(" \n\n");
	  
	  /*     Drop memcache part
	  
	  // check final string
	  int lenbuf1=strlen(buffer1);
	  printf(" memcache buffer lenght = %d \n",lenbuf1);
	  // printf(" buffer 1 = %s \n",buffer1);
	  
	  // and send buffer to memcache server
	  bool set_value=memcached_set(memc,key,strlen(key),buffer1,strlen(buffer1),999999,0);
	  printf("set memcache  %s!\n", memcached_strerror(memc, rc));
	  
	  
	  //and read again the key
	  retrieved_value = memcached_get(memc, key, strlen(key), &value_length, &flags, &rc);
	  printf(" get memcache  %s!\n", memcached_strerror(memc, rc));
	  
	  */
	  
	} else {
	  //printf( " lenbuf = 0 - exiting \n");
	  fprintf(stderr, "Couldn't get json \n");
	  DrvHVSipm_except::HVSipmRetStatus(handle,3, "IP = "+fIPAddress+" ");
	}
	
	//}  // end of for loop
 
}

