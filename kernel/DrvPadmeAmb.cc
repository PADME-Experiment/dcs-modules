#include "DrvPadmeAmb.h"   //in c file
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

  DrvPadmeAmb_except&
DrvPadmeAmb_except::PadmeAmbRetStatus(int ambhandler,int retstatus, const std::string& add)
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


  std::string str("PadmeAmbWrapper returned error ("+std::to_string(retstatus)+") :  "+error);
  if(add.size()>0)str+="   "+add;

  switch (retstatus){
    case 1             : throw DrvPadmeAmb_except::Except_SYSERR               (str) ;break;
    case 2             : throw DrvPadmeAmb_except::Except_WRITEERR             (str) ;break;
    case 3             : throw DrvPadmeAmb_except::Except_READERR              (str) ;break;
    case 4             : throw DrvPadmeAmb_except::Except_TIMEERR              (str) ;break;
    case 5             : throw DrvPadmeAmb_except::Except_DOWN                 (str) ;break;
    case 6             : throw DrvPadmeAmb_except::Except_NOTPRES              (str) ;break;
  }
}

  int
DrvPadmeAmb::ComInit()
{
  /* -------------- INITIALIZING VARIABLES -------------- */
 int handle; // socket file descriptors
 int portNum = 11211; // port number (same that server)
 int bufsize = 1024; // buffer size
 char buffer[1024]; // buffer to transmit
 //char ip[] = "141.108.7.201"; // Server IP */
 memcached_server_st *servers = NULL;

 cout << "\n- Starting client..." << endl;
//memcached_servers_parse (char *server_strings);
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
      DrvPadmeAmb_except::PadmeAmbRetStatus(handle,6, "IP = "+fIPAddress+" ");
    }
 
  return handle;
}

  void
DrvPadmeAmb::ComDeinit(int handle)
{
  // close(handle);
  // int ret = PadmeAmb_DeinitSystem(handle);
  // if(ret != AMB_OK)
  // DrvPadmeAmb_except::PadmeAmbRetStatus(handle,ret);
}



  void
DrvPadmeAmb::AssertInit()
{
  INFO("DrvPadmeAmb::AssertInit()");
  //#warning CAEN ComInit
  fPadmeAmbHandle=ComInit();
  AssertInitAllOwned();
  //ProcessUpdateListTemp();
}

void
DrvPadmeAmb::Finalize()
{
  INFO("DrvPadmeAmb::Finalize()");
  
  int bufsize;
  this->VDeviceBase::Finalize();
  //#warning CAEN ComDeinit
  ComDeinit(fPadmeAmbHandle);
  JoinThread();
}

void
DrvPadmeAmb::DebugDump()
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
DrvPadmeAmb::OnCycleLocal()
{
  int handle;
  // int portNum = 11211; // port number (same that server)
  char *key = "PadmeAmb";
  char *retrieved_value;
  size_t value_length;
  uint32_t flags;
  // memcached_server_st *servers = NULL;
  // memcached_servers_parse (char *server_strings);
  // memcached_st *memc;
  memcached_return rc;
  //char *serv;
  
 
  /*   already done at  cominit
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
    DrvPadmeAmb_except::PadmeAmbRetStatus(handle,6, "IP = "+fIPAddress+" ");
  }
  */

  // sleep 10 seconds between readout
  usleep(10000000);

  // here get value from memcache server
  retrieved_value = memcached_get(memc, key, strlen(key), &value_length, &flags, &rc);
  //printf("Yay!\n");
  
  if (rc == MEMCACHED_SUCCESS) {
    // fprintf(stderr, "Key retrieved successfully - ik=%d \n",ik);
    // printf("The key '%s' on server '%s' returned value '%s'.\n", key,serv,retrieved_value);
    cout <<  " PADME Amb memcache key " << key << " server says " ;
    int length=value_length;
    cout << retrieved_value << " - length =  " << length << endl;
    free(retrieved_value);
      
  } else {
    fprintf(stderr, "Couldn't retrieve key %s: %s\n", key,memcached_strerror(memc, rc));
    DrvPadmeAmb_except::PadmeAmbRetStatus(handle,3, "IP = "+fIPAddress+" ");
    
  }  
    
}

