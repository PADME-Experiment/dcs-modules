#include "DrvBTFBeam.h"   //in c file
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
#include <stdio.h>
#include <string.h>

using namespace std;

 
  DrvBTFBeam_except&
DrvBTFBeam_except::BTFBeamRetStatus(int ambhandler,int retstatus, const std::string& add)
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


  std::string str("BTFBeamWrapper returned error ("+std::to_string(retstatus)+") :  "+error);
  if(add.size()>0)str+="   "+add;

  switch (retstatus){
    case 1             : throw DrvBTFBeam_except::Except_SYSERR               (str) ;break;
    case 2             : throw DrvBTFBeam_except::Except_WRITEERR             (str) ;break;
    case 3             : throw DrvBTFBeam_except::Except_READERR              (str) ;break;
    case 4             : throw DrvBTFBeam_except::Except_TIMEERR              (str) ;break;
    case 5             : throw DrvBTFBeam_except::Except_DOWN                 (str) ;break;
    case 6             : throw DrvBTFBeam_except::Except_NOTPRES              (str) ;break;
  }
}

  int
DrvBTFBeam::ComInit()
{
  /* -------------- INITIALIZING VARIABLES -------------- */
 int handle; // socket file descriptors
 int portNum = 11211; // port number (same that server)
 int bufsize = 1024; // buffer size
 char buffer[1024]; // buffer to transmit
 //char ip[] = "141.108.7.201"; // Server IP */
 memcached_server_st *servers = NULL;

 cout << "\n- Starting client..." << endl;
// memcached_servers_parse (char *server_strings);
// memcached_st *memc;
memcached_return rc;
//char *serv;


  char *retrieved_value;
  size_t value_length;
  uint32_t flags;

  char * serv = new char[fIPAddress.size() + 1];
  std::copy(fIPAddress.begin(), fIPAddress.end(), serv);
  serv[fIPAddress.size()] = '\0'; // don't forget the terminating 0

  int port;
  if(fPort.size()!=0) {
    port = std::atoi (fPort.c_str());
  } else {
    port = portNum;
  }

    memc = memcached_create(NULL);
    servers = memcached_server_list_append(servers,serv, port, &rc);
    rc = memcached_server_push(memc, servers);

    if (rc == MEMCACHED_SUCCESS)
      printf("Added server %s successfully\n",serv);
    else {
      fprintf(stderr, "Couldn't add server: %s\n", memcached_strerror(memc, rc));
      DrvBTFBeam_except::BTFBeamRetStatus(handle,6, "IP = "+fIPAddress+" ");
    }

    return handle;    
}

  void
DrvBTFBeam::ComDeinit(int hadle)
{
  // close(handle);
  // int ret = BTFBeam_DeinitSystem(handle);
  // if(ret != AMB_OK)
  // DrvBTFBeam_except::BTFBeamRetStatus(handle,ret);
}



  void
DrvBTFBeam::AssertInit()
{
  INFO("DrvBTFBeam::AssertInit()");
  //#warning CAEN ComInit
  fBTFBeamHandle=ComInit();
  AssertInitAllOwned();
  //ProcessUpdateListTemp();
}

void
DrvBTFBeam::Finalize()
{
  INFO("DrvBTFBeam::Finalize()");
  int bufsize;
  this->VDeviceBase::Finalize();
  //#warning CAEN ComDeinit
  ComDeinit(fBTFBeamHandle );
  JoinThread();
}

void
DrvBTFBeam::DebugDump()
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
DrvBTFBeam::OnCycleLocal()
{
  int handle;
  // int portNum = 11211; // port number (same that server)
  char *key = "keyvalue";
  char *retrieved_value;
  size_t value_length;
  uint32_t flags;
  // memcached_server_st *servers = NULL;
  //memcached_servers_parse (char *server_strings);
  // memcached_st *memc;
  memcached_return rc;
  //char *serv;

  /*   done at cominit
  char * serv = new char[fIPAddress.size() + 1];
  std::copy(fIPAddress.begin(), fIPAddress.end(), serv);
  serv[fIPAddress.size()] = '\0'; // don't forget the terminating 0

  int port;
  if(fPort.size()!=0) {
    port = std::atoi (fPort.c_str());
  } else {
    port = portNum;
  }

  memc = memcached_create(NULL);
  servers = memcached_server_list_append(servers,serv, port, &rc);
  rc = memcached_server_push(memc, servers);
  
  if (rc == MEMCACHED_SUCCESS) {
      // printf("Added server %s successfully\n",serv);
  } else {
      fprintf(stderr, "Couldn't add server: %s\n", memcached_strerror(memc, rc));
      DrvBTFBeam_except::BTFBeamRetStatus(handle,6, "IP = "+fIPAddress+" ");
  }
  */

  // here get value from memcache server
  retrieved_value = memcached_get(memc, key, strlen(key), &value_length, &flags, &rc);
    //printf("Yay!\n");

  if (rc == MEMCACHED_SUCCESS) {
    // fprintf(stderr, "Key retrieved successfully - ik=%d \n",ik);
    // printf("The key '%s' on server '%s' returned value '%s'.\n", key,serv,retrieved_value);
      int length=value_length;
      
      // printf ("Splitting string DAQ into tokens:\n");
      // get only last 4 values of all string
      char *DAQ_string;
 
      DAQ_string=strdup(retrieved_value);
      free(retrieved_value);
      
      // get timestamp 
      time_t ticks;
      ticks = time(NULL);
      char timestring[27];
      snprintf(timestring, 25, "%.25s", ctime(&ticks));
      // printf(" Timestamp = %25s  \n",timestring);

      /*
      cout <<  " memcache key " << key << " server says ";
      cout << DAQ_string << " - length =  " << length << endl;
      */
      
      char * pch1[20];
      char *found;
      int itoken1=0;
      while ((found = strsep(&DAQ_string," ")) != NULL ) {
 	pch1[itoken1]=found;
	itoken1++;
      }
      // cout << "memcache key pieces = " << itoken1 << endl;

      /*
      for(int ik=0;ik<itoken1;ik++) {
	printf(" pch1[%d] = %s \n",ik,pch1[ik]);
      }
      */
      
      char send_string[20];
      sprintf(send_string, "%s %s %s %s %s %s ",timestring,pch1[itoken1-5],pch1[itoken1-4],pch1[itoken1-3],pch1[itoken1-2],pch1[itoken1-1]);
      int lenght1=strlen(send_string);
      
      cout << " memcache key " << key << " sent_value = "<< send_string << " - length =  " << lenght1 << endl;

      // wait 2 secs for next read
      usleep(2000000);

  } else {
    fprintf(stderr, "Couldn't retrieve key %s: %s\n", key,memcached_strerror(memc, rc));
    DrvBTFBeam_except::BTFBeamRetStatus(handle,3, "IP = "+fIPAddress+" ");
     
  }  
}

