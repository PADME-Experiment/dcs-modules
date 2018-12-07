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
#include <cmath>        // std::abs

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
  // string tok;
  char *tokens0[500];
  char *tokens1[500];
  int portNum = 11211; // port number
  char timestring[27];
  char *string0;
   #define CHUNK 3000 /* read max 3000 bytes at a time */
  char buf1[CHUNK];
  char buf0[CHUNK];
  struct sockaddr_in sa;
  char node[NI_MAXHOST];
  struct tm * timeinfo;

  bool debug=false;

  bool isExit=false;

  //for(int jjj=0; jjj< 1000; jjj++) {
	char *js= NULL;
	char *js0= NULL;
	int nread0=0;
	int nread=0;
  	int ichan =-1;
	jjj++;

  	// first sleep 20 sec to be sure readout timing is OK
	usleep(20000000);

	char * serv = new char[fIPAddress.size() + 1];
	std::copy(fIPAddress.begin(), fIPAddress.end(), serv);
	serv[fIPAddress.size()] = '\0'; // don't forget the terminating 0
		  
	string wget="wget -nv -O ";
	string http=" http://";
	string brdcfg="/brdcfg.cgx";
	string brdusr="/brdusr.cgx";
	string command_cfg;
	string command_usr;

	string filename_cfg="brdcfg.cgx";
	filename_cfg+=".";
	filename_cfg+=serv;
	string filename_usr="brdusr.cgx";
	filename_usr+=".";
	filename_usr+=serv;

	command_cfg+=wget;
	command_cfg+=filename_cfg;
	command_cfg+=http;
	command_cfg+=serv;
	command_cfg+=brdcfg;

	command_usr+=wget;
	command_usr+=filename_usr;
	command_usr+=http;
	command_usr+=serv;
	command_usr+=brdusr;

	//cout << " command_cfg " << command_cfg << endl; 
	//cout << " command_usr " << command_usr << endl; 

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

	// OLD - human readable format for timestamp
	//ticks = time(NULL);
	//snprintf(timestring, 25, "%.25s", ctime(&ticks));

	// new formatted timestring
	time (&ticks);
	timeinfo = localtime (&ticks);
	strftime (timestring,25,"%Y-%m-%d %T",timeinfo);

	
	// get json string from file
	// Open file for reading 
	fp = fopen(filename_cfg.c_str(), "r");
	
	if(fp) {
	  // printf(" Opened file brdcfg.cgx \n");
	  while ((nread0 = fread(buf0, 1, sizeof buf0, fp)) > 0) {
	    //printf(" nread = %u \n",nread);
	  }
	  // fwrite(buf0, 1, nread, stdout);
	  if (ferror(fp)) {
	    // deal with error 
	    fprintf(stderr, " error reading brdcfg.cgx  \n");
	    DrvHVSipm_except::HVSipmRetStatus(handle,3, "IP = "+fIPAddress+" ");
	    // printf(" error reading file \n");
	  }
	  // and close the file	  
	  fclose(fp);
	  // and remove the file
	  string command1="rm ";
	  command1+=filename_cfg;
	  system(command1.c_str());
	} else {
	  printf("Error opening file  brdcfg.cgx \n");
	  DrvHVSipm_except::HVSipmRetStatus(handle,3, "IP = "+fIPAddress+" ");
	  return;
	}
	
	
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
	      // separator is :
	      while( (tok=strsep(&tok0,":")) != NULL) {
		// printf( " %d tok0= %s \n",tokcount0,tok);
		/*
		//and strip quotes in tok
		// substitute quotes at start and end
		int iiok = strlen(tok);
		tok[0]="";
		tok[iiok]="";
		*/
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
	char *myipaddr;
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
	  if(strcmp(tokens0[itok],options0[1])  ==0 ) {
	    myipaddr=tokens0[itok+1];
	    itok++;
	  }
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
	fp = fopen(filename_usr.c_str(), "r");
	
	if(fp) {
	  // printf(" Opened file brdusr.cgx \n");
	  while ((nread = fread(buf1, 1, sizeof buf1, fp)) > 0) {
	    // printf(" nread = %u \n",nread);
	  }
	  // fwrite(buf1, 1, nread, stdout);
	  if (ferror(fp)) {
	    // deal with error 
	    fprintf(stderr, " error reading brdusr.cgx  \n");
	    DrvHVSipm_except::HVSipmRetStatus(handle,3, "IP = "+fIPAddress+" ");
	    // printf(" error reading file \n");
	    // return 1;
	  }
	  // and close the file	  
	  fclose(fp);
	  // and remove the file
	  string command2="rm ";
	  command2+=filename_usr;
	  system(command2.c_str());
	} else {
	  printf("Error opening file  brdusr.cgx \n");
	  DrvHVSipm_except::HVSipmRetStatus(handle,3, "IP = "+fIPAddress+" ");
	}

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

	  // here get the name and number of the detector
	  memset(&sa, 0, sizeof sa);
	  sa.sin_family = AF_INET;
	  
	  inet_pton(AF_INET, fIPAddress.c_str() , &sa.sin_addr);
	  
	  int res = getnameinfo((struct sockaddr*)&sa, sizeof(sa),
				node, sizeof(node),
				NULL, 0, NI_NAMEREQD);
	  
	  int length_node=strlen(node);
	  
	  char typeofveto[12]=" ";
	  char numofveto[4]=" ";
	  char position[4]="";
	  if (res) {
	    printf("error: %d\n", res);
	    printf("%s \n", gai_strerror(res));
	    
	    strcpy(typeofveto,"unknown");
	    strcpy(numofveto,"0");
	  }
	  else {
	    //printf("IP address %s - node=%s - length_node=%d \n", fIPAddress.c_str(),node,length_node);

	    //strncat(numofveto,node[6],2);
	    numofveto[0]=node[length_node-1];
	    numofveto[1]=node[length_node];
	    numofveto[2]='\0';
	    //printf(" type = %s  - number = %s \n",typeofveto,numofveto);

	    
	    if(length_node<15) {
	      //Copies second string
	      snprintf(typeofveto,length_node-5,"%s",node);              
	      typeofveto[length_node-5]='\0';
	    } else {
	      snprintf(typeofveto,8,"%s","hepveto");
	      typeofveto[8]='\0';
	      //if(strcmp(numofveto,"1")) snprintf(numofveto,4,"%s","top");
	      //if(strcmp(numofveto,"2")) snprintf(numofveto,4,"%s","bot");
	      //numofveto[4]='\0';
	    }

	  }
	   
	  int maxchan;
	  maxchan=ichan;
	  if(strcmp(typeofveto,"hepveto")==0 && strcmp(numofveto,"8")==0) {
	    maxchan=12;
	  }


	  if(debug) {
	    // comment printout
	    
	    printf(" Loop = %d - Timestamp = %25s  \n",jjj,timestring);
	    printf(" %.11s; %.7s; %.7s; %.9s; %.15s ;%.3s:%.3s:%.3s:%.3s:%.3s:%.3s;",name,snum,hwver,swver,myipaddr,mac1,mac2,mac3,mac4,mac5,mac6);
	    printf("\n id  chan status hvset  hvlvl  cardT  sipmI  sipmT errSt  errcnt lasterr\n");
	    for (ik=0;ik<maxchan;ik++) {
	      printf("%4d %4d %4d   %5.2f  %5.2f  %5.2f  %5.2f  %5.2f %5d %5d  %5d \n",
		     idval[ik],chanval[ik],cardsts[ik],hvreq[ik],hvlvl[ik],cardtemp[ik],
		     sipmcurr[ik],sipmtemp[ik],errsts[ik],errcnt[ik],lasterr[ik]);
	    }
	    printf(" \n\n");
	  }
	  
	  
	 
	  // and output to file
	  FILE * pFile;
	  std::string filename="data/HVSiPM_";
	  filename+=fIPAddress;
	  pFile = fopen (filename.c_str(),"w");
	  FILE * pFileh;
	  std::string filenameh="history/HVSiPM_";
	  filenameh+=fIPAddress;
	  pFileh = fopen (filenameh.c_str(),"a");

	  fprintf(pFile,"%.24s;%.11s;%.7s;%.7s;%.9s;%.15s;%.3s:%.3s:%.3s:%.3s:%.3s:%.3s;%d \n",timestring,name,snum,hwver,swver,myipaddr,mac1,mac2,mac3,mac4,mac5,mac6,maxchan);
	  fprintf(pFileh,"%.24s;%.15s;%d \n",timestring,myipaddr,maxchan);
	  for (ik=0;ik<maxchan;ik++) {
	    // print values for dcs
	    fprintf(pFile,"%4d;%4d;%4d;%5.2f;%5.2f;%5.2f;%5.2f;%5.2f;%5d;%5d;%5d \n",
		   idval[ik],chanval[ik],cardsts[ik],hvreq[ik],hvlvl[ik],cardtemp[ik],
		   sipmcurr[ik],sipmtemp[ik],errsts[ik],errcnt[ik],lasterr[ik]);
	    // and history file
	    fprintf(pFileh,"%4d;%4d;%4d;%5.2f;%5.2f;%5.2f;%5.2f;%5.2f;%5d;%5d;%5d \n",
		   idval[ik],chanval[ik],cardsts[ik],hvreq[ik],hvlvl[ik],cardtemp[ik],
		   sipmcurr[ik],sipmtemp[ik],errsts[ik],errcnt[ik],lasterr[ik]);
	  }

	  fclose(pFile);
	  fclose(pFileh);

	    
	  // check valid values for HV,temp and no errors
	  // and send alarm if needed
	  int ioff_channels=0;
	  FILE * pFile2;
	  std::string filename2="alarm/sipm_";
	  filename2.append(typeofveto);
	  filename2.append(numofveto);
	  filename2+="_alarm.txt";
	  pFile2 = fopen (filename2.c_str(),"w");
	  fprintf(pFile2," <span foreground=\"red\" font=\"32\">DCS SIPM  %s %s  Alarm \n",typeofveto,numofveto);
	  
	  
	  // prepare for alarms
	  FILE * pFile4;
	  std::string filename4="monitor/sipm_alarm";
	  filename4.append(typeofveto);
	  filename4.append(numofveto);
	  filename4+=".txt";
	  pFile4 = fopen (filename4.c_str(),"w");
	  fprintf(pFile4,"PLOTID SIPM_ALARM_%s_%s \n",typeofveto,numofveto);
	  fprintf(pFile4,"PLOTNAME SiPM %s_%s %.24s \nPLOTTYPE activetext\n",typeofveto,numofveto,timestring);
	  fprintf(pFile4,"DATA [ ");

	  
	  // 2.7 to avoid alarm of pveto 6 channels off
	  float vlimit=2.7;
	  float tlimit=50.;
	  for (ik=0;ik<maxchan;ik++) {
	    int ialarm=0;
	    // mask alarms for hepveto 8, last 4 channels
	    //if(strcmp(typeofveto,"hepveto")==0 && strcmp(numofveto,"8")==0) {
	    //if(ik>11) break;
	    //}
	    // alarm for voltage
	    float absdiff=abs(hvreq[ik]-hvlvl[ik]);
	    if(absdiff>vlimit) {
	      printf("Alarm SIPM %s - %s - channel %d - HV=%5.2f  \n ",typeofveto,numofveto,ik,hvlvl[ik]);
	      fprintf(pFile2," <i> check channel %d - HV=%5.2f  \n</i> ",ik,hvlvl[ik]);
	      ialarm++;
	    }
	    if(sipmtemp[ik]>tlimit) {
	      printf("Alarm SIPM %s - %s - channel %d - Temp=%5.2f  \n ",typeofveto,numofveto,ik,sipmtemp[ik]);
	      fprintf(pFile2," <i> check channel %d - Temp=%5.2f  \n</i> ",ik,sipmtemp[ik]);
	      ialarm++;
	    }
	    if(errsts[ik]!=0) {
	      //if((errsts[ik]!=0) && (errcnt[ik]>1) ) {
	      printf("Alarm SIPM %s - %s - channel %d - HV=%5.2f Current=%5.2f Temp=%5.2f Error=%d \n",typeofveto,numofveto,ik,hvlvl[ik],sipmcurr[ik],sipmtemp[ik],errsts[ik]);
	      fprintf(pFile2,"<i> check channel %d - HV=%5.2f Current=%5.2f Temp=%5.2f Error=%d \n</i>",ik,hvlvl[ik],sipmcurr[ik],sipmtemp[ik],errsts[ik]);
	      ialarm++;
	    }

	    // prepare data for alarm monitor
	    if(ialarm>0) {
	      ioff_channels++;
	      if (ioff_channels>0) {
		fprintf(pFile4,",");
	      }
	      fprintf(pFile4,"{\"title \",\"Chan %s HV \",\"current\":{\"value\":\"%5.2f\",\"col\":\"#FF0000\"}}",ik,hvlvl[ik]);
	      fprintf(pFile4,"{\"title \",\"current \",\"current\":{\"value\":\"%5.2f\",\"col\":\"#FF0000\"}}",sipmcurr[ik]);
	      fprintf(pFile4,"{\"title \",\"Temp \",\"current\":{\"value\":\"%.5.2f\",\"col\":\"#FF0000\"}}",sipmtemp[ik]);
	      fprintf(pFile4,"{\"title \",\"Status \",\"current\":{\"value\":\"%d\",\"col\":\"#FF0000\"}}",errsts[ik]);
	    }

	  }  // endif for channels

	  // And close alarm files
	  fprintf(pFile2," </span> ");
	  fclose(pFile2);

	  fprintf(pFile4," ]\n");
	  fclose(pFile4);

	  /*
	  // and copy alarm file to monitor@l0padme3
	  std::string scp4="scp -q ";
	  scp4+=filename4;
	  scp4+=" monitor@l0padme3:PadmeMonitor/watchdir/. ";
	  // cout << " scp command " << scp2 << endl; 
	  char * writable4 = new char[scp4.size() + 1];
	  std::copy(scp4.begin(), scp4.end(), writable4);
	  writable4[scp4.size()] = '\0'; // don't forget the terminating 0
	  // scp to monitor@l0padme3
	  system(writable4);
	  delete [] writable4;
	  */

	  // and send alarm if there are channels off
	  if(ioff_channels>0) {
	    std::string command2="sendmessage_sipm  ";
	    command2+=filename2;
	    char * writable2 = new char[command2.size() + 1];
	    std::copy(command2.begin(), command2.end(), writable2);
	    writable2[command2.size()] = '\0'; // don't forget the terminating 0
	    system(writable2);
	    // and delete 
	    delete [] writable2;

	  }
	    	  
	  
	  // and write plots for monitoring
	  FILE * pFile3;
	  std::string filename3="monitor/SiPMMon_";
	  filename3.append(typeofveto);
	  filename3.append(numofveto);
	  filename3+=".txt";
	  pFile3 = fopen (filename3.c_str(),"w");

	  char typeofveto_up[length_node-5];
	  int nchar=length_node-5;
	  int ic=0;
	  for (ic=0;ic<nchar;ic++) {
	    typeofveto_up[ic]=toupper(typeofveto[ic]);
	  }
	    
	  // write values to file
	  int numchan= maxchan;
	  fprintf(pFile3,"PLOTID %s_NIM_MOD_%s_HV \n",typeofveto_up,numofveto);
	  fprintf(pFile3,"PLOTNAME HV_%s_%s %s \nCHANNELS %d\nRANGE_X 0 %d\nTITLE_Y Voltage (V)\nPLOTTYPE histo1d\n",typeofveto_up,numofveto,timestring,numchan,numchan);
	  fprintf(pFile3,"DATA  [ [");
	  for (ik=0;ik<maxchan-1;ik++) {
	    fprintf(pFile3,"%5.2f,",hvlvl[ik]);
	  }
	  fprintf(pFile3,"%5.2f ]]\n",hvlvl[maxchan-1]);

	  fprintf(pFile3,"PLOTID %s_NIM_MOD_%s_CURRENT \n",typeofveto_up,numofveto);
	  fprintf(pFile3,"PLOTNAME SiPM_Current_%s_%s \nCHANNELS %d\nRANGE_X 0 %d\nTITLE_Y Current (uA) \nPLOTTYPE histo1d\n",typeofveto_up,numofveto,numchan,numchan);
	  fprintf(pFile3,"DATA  [ [");
	  for (ik=0;ik<maxchan-1;ik++) {
	    fprintf(pFile3,"%5.2f,",sipmcurr[ik]);
	  }
	  fprintf(pFile3,"%5.2f ]]\n",sipmcurr[maxchan-1]);

	  fprintf(pFile3,"PLOTID %s_NIM_MOD_%s_TEMP \n",typeofveto_up,numofveto);
	  fprintf(pFile3,"PLOTNAME SiPM_Temp_%s_%s \nCHANNELS %d\nRANGE_X 0 %d\nTITLE_Y Temp (C) \n PLOTTYPE histo1d\n",typeofveto_up,numofveto,numchan,numchan);
	  fprintf(pFile3,"DATA  [ [");
	  for (ik=0;ik<maxchan-1;ik++) {
	    fprintf(pFile3,"%5.2f,",sipmtemp[ik]);
	  }
	  fprintf(pFile3,"%5.2f ]]\n",sipmtemp[maxchan-1]);

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
	  delete [] writable2;
	 
	  
	} else {
	  //printf( " lenbuf = 0 - exiting \n");
	  fprintf(stderr, "Couldn't get json \n");
	  DrvHVSipm_except::HVSipmRetStatus(handle,3, "IP = "+fIPAddress+" ");
	}
	
	//}  // end of for loop

 
}

