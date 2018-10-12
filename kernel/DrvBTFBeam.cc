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
 //const int bufsize = 4096; // buffer size
 //char buffer[bufsize]; // buffer to transmit
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
  char *key1 = "DAQ_BTF";
  //char *key1 = "DAQ_BTF";
  char *key3 = "VUG_PADME";
  char *key2 = "BTFDATA_PADME";
  char *retrieved_value1;
  char *retrieved_value2;
  char *retrieved_value3;
  size_t value_length1;
  size_t value_length2;
  size_t value_length3;
  uint32_t flags;
   memcached_return rc;
  //char *serv;

   // get timestamp 
   time_t ticks;
   ticks = time(NULL);
   char timestring[27];
   snprintf(timestring, 25, "%.25s", ctime(&ticks));
   // printf(" Timestamp = %25s  \n",timestring);


   /*  
   // key 1
  // here get value from memcache server
  retrieved_value1 = memcached_get(memc, key1, strlen(key1), &value_length1, &flags, &rc);
    //printf("Yay!\n");

  if (rc == MEMCACHED_SUCCESS) {
    // fprintf(stderr, "Key retrieved successfully - ik=%d \n",ik);
    //printf("The key '%s' on server '%s' returned value '%s'.\n", key,serv,retrieved_value);
      int length=value_length1;
      
      // printf ("Splitting string DAQ into tokens:\n");
      // get only last 4 values of all string
      char *DAQ_string;
 
      DAQ_string=strdup(retrieved_value1);
      free(retrieved_value1);
      
       
      //cout <<  " memcache key " << key << " server says ";
      //cout << DAQ_string << " - length =  " << length << endl;
            
      char * pch1[4000];
      char *found;
      int itoken1=0;
      while ((found = strsep(&DAQ_string," ")) != NULL ) {
 	pch1[itoken1]=found;
	itoken1++;
      }
      //cout << "memcache key pieces = " << itoken1 << endl;

      
      //for(int ik=0;ik<itoken1;ik++) {
      //printf(" pch1[%d] = %s \n",ik,pch1[ik]);
      //}
     
      
      char send_string[20];
      sprintf(send_string, "%.24s;%s;%s;%s;%s;%s ",timestring,pch1[itoken1-5],pch1[itoken1-4],pch1[itoken1-3],pch1[itoken1-2],pch1[itoken1-1]);
      int lenght1=strlen(send_string);
      
      cout << " memcache key1 " << key1 << " sent_value = "<< send_string << " - length =  " << lenght1 << endl;

      // and send value to file
      FILE * pFile;
      // int n;
      std::string filename1="data/BTF_DAQ";
      pFile = fopen (filename1.c_str(),"w");
      // write values to file
      fprintf(pFile," %s ",send_string);
      fclose(pFile);
   
  } else {
    fprintf(stderr, "Couldn't retrieve key %s: %s\n", key1,memcached_strerror(memc, rc));
    DrvBTFBeam_except::BTFBeamRetStatus(handle,3, "IP = "+fIPAddress+" ");
     
  }  

*/

  // key 2
  // here get value from memcache server
  retrieved_value2 = memcached_get(memc, key2, strlen(key2), &value_length2, &flags, &rc);
    //printf("Yay!\n");

  if (rc == MEMCACHED_SUCCESS) {
    // fprintf(stderr, "Key retrieved successfully  \n");
    // printf("The key '%s' returned value '%s'.\n", key2,retrieved_value2);
    int length=value_length2;
      
    // printf ("Splitting string DAQ into tokens:\n");
    // get only last 4 values of all string
    char *BTF_string;
 
    BTF_string=strdup(retrieved_value2);
    free(retrieved_value2);
    
    //cout <<  " memcache key " << key2 << " server says ";
    //cout << BTF_string << " - length =  " << length << endl;
    
    char * pch2[20];
    char *found;
    int itoken2=0;
    while ((found = strsep(&BTF_string,",")) != NULL ) {
 	pch2[itoken2]=found;
	itoken2++;
    }
    //cout << "memcache key2 pieces = " << itoken2 << endl;
    
    /*
    for(int ik=0;ik<itoken2;ik++) {
      printf(" pch2[%d] = %s \n",ik,pch2[ik]);
    }
    */

    // convert labview timestamp to normal timestamp
    double timechar=atof(pch2[1]);
    // and subtract seconds from 0:0 1/1/1970 to 0:00 1/1/1904
    double  time2 = timechar -  2082844800.;
    // printf(" new time is %f \n",time2);
    char array[100];
    sprintf(array, "%f", time2);
    // printf(" array is %s \n",array);
  
    // convert first data to normal time stamp
    time_t timedata = atoi(array); // convert to time_t, ignores msec
    char *dateconv;
    dateconv=asctime(localtime(&timedata));

    char send_string[70];

    // convert pch2[10] to string and strip last funny chars
    string pch2_10;
    //sprintf(pch2_10,"%s",pch2[10] );
    pch2_10=pch2[10];
    if (pch2_10.size () > 0)  pch2_10.resize (pch2_10.size () - 4);

      
    // reinterpret timestamp from unix timestamp
    //time_t timeval=ctime(pch2[1]);
    //printf("Converted Time %s",timeval);
    sprintf(send_string, "%.24s;%s;%s;%s;%s;%s;%s;%s;%s;%s \n",dateconv,pch2[2],pch2[3],pch2[4],pch2[5],pch2[6],pch2[7],pch2[8],pch2[9],pch2_10.c_str());
    int lenght2=strlen(send_string);
      
    cout << " memcache key2 " << key2 << " sent_value = "<< send_string << " - length =  " << lenght2 << endl;

      // and send value to file
      FILE * pFile;
      std::string filename1="data/BTFDATA_Padme";
      pFile = fopen (filename1.c_str(),"w");
      // write values to file
      fprintf(pFile," %s ",send_string);
      fclose(pFile);

     // and send value to file
      FILE * pFile2;
      std::string filename2="BTFDATA.txt";
      pFile2 = fopen (filename2.c_str(),"w");

      char *parttype;
      if(pch2[4]=="0") parttype="e-";
      if(pch2[4]=="1") parttype="e+";
      

      // write values to file
      fprintf(pFile2,"PLOTID DCS_BTF_Beam \nPLOTNAME BTF Data for Padme \nPLOTTYPE text \n");
      fprintf(pFile2,"DATA [ [\"Timestamp\",\"%.24s\"]",dateconv);
      fprintf(pFile2,",[\"Producer status\",%s]",pch2[2]);
      fprintf(pFile2,",[\"DHPTB101 status\",%s]",pch2[3]);
      fprintf(pFile2,",[\"BTF part type\",%s]",pch2[4]);
      fprintf(pFile2,",[\"Padme Magnet current\",%s] ]\n",pch2_10.c_str());
      fprintf(pFile2,"PLOTID DHSTB001_BTF_Beam \nPLOTNAME DHSTB001 Data for Padme \nPLOTTYPE text \n");
      fprintf(pFile2,"DATA [ [\"Timestamp\",\"%.24s\"]",dateconv);
      fprintf(pFile2,",[\"DHSTB001 status\",%s]",pch2[5]);
      fprintf(pFile2,",[\"DHSTB001 Energy\",%s] ]\n",pch2[8]);
      fprintf(pFile2,"PLOTID DHSTB002_BTF_Beam \nPLOTNAME DHSTB002 Data for Padme \nPLOTTYPE text \n");
      fprintf(pFile2,"DATA [ [\"Timestamp\",\"%.24s\"]",dateconv);
      fprintf(pFile2,",[\"DHSTB002 status\",%s]",pch2[6]);
      fprintf(pFile2,",[\"DHSTB002 Energy\",%s] ]\n",pch2[9]);
      fclose(pFile2);

      // and copy file to monitor@l0padme3
      string scp="scp -q BTFDATA.txt monitor@l0padme3:PadmeMonitor/watchdir/. ";
      // cout << " scp command " << scp << endl; 
      char * writable1 = new char[scp.size() + 1];
      std::copy(scp.begin(), scp.end(), writable1);
      writable1[scp.size()] = '\0'; // don't forget the terminating 0
      // scp to monitor@l0padme3
      system(writable1);


  } else {
    fprintf(stderr, "Couldn't retrieve key %s: %s\n", key3,memcached_strerror(memc, rc));
    DrvBTFBeam_except::BTFBeamRetStatus(handle,3, "IP = "+fIPAddress+" ");
     
  }  
     
  // key 3
  // here get value from memcache server
  retrieved_value3 = memcached_get(memc, key3, strlen(key3), &value_length3, &flags, &rc);
    //printf("Yay!\n");

  if (rc == MEMCACHED_SUCCESS) {
    // fprintf(stderr, "Key retrieved successfully  \n");
    // printf("The key '%s' returned value '%s'.\n", key3,retrieved_value3);
    int length=value_length3;
      
    // printf ("Splitting string DAQ into tokens:\n");
    // get only last 4 values of all string
    char *VUG_string;
 
    VUG_string=strdup(retrieved_value3);
    free(retrieved_value3);
    
    //cout <<  " memcache key " << key2 << " server says ";
    //cout << VUG_string << " - length =  " << length << endl;
    
    char * pch3[4000];
    char *found;
    int itoken3=0;
    while ((found = strsep(&VUG_string,",")) != NULL ) {
 	pch3[itoken3]=found;
	itoken3++;
    }
    //cout << "memcache key pieces = " << itoken2 << endl;
    
    /*
    for(int ik=0;ik<itoken3;ik++) {
      printf(" pch3[%d] = %s \n",ik,pch3[ik]);
    }
    */

    // convert labview timestamp to normal timestamp
    double timechar3=atof(pch3[1]);
    // and subtract seconds from 0:0 1/1/1970 to 0:00 1/1/1904
    double  time3 = timechar3 -  2082844800.;
    // printf(" new time is %f \n",time2);
    char array3[100];
    sprintf(array3, "%f", time3);
    // printf(" array is %s \n",array);
  
    // convert first data to normal time stamp
    time_t timedata3 = atoi(array3); // convert to time_t, ignores msec
    char *dateconv3;
    dateconv3=asctime(localtime(&timedata3));

    // convert pch3[4] to string and strip last funny chars
    string pch3_4;
    pch3_4=pch3[4];
    //sprintf(pch3_4,"%s",pch3[4]);
    if (pch3_4.size () > 0)  pch3_4.resize (pch3_4.size () - 4);

    char send_string[70];
    
    // reinterpret timestamp from unix timestamp
    //time_t timeval=ctime(pch2[1]);
    //printf("Converted Time %s",timeval);
    sprintf(send_string, "%.24s;%s;%s;%s \n",dateconv3,pch3[2],pch3[3],pch3_4.c_str());
    int lenght3=strlen(send_string);
    
    cout << " memcache key3 " << key3 << " sent_value = "<< send_string << " - length =  " << lenght3 << endl;


    // convert vacuum to float
    double vacuum_pressure=atof(pch3_4.c_str());

    // and send value to file
    FILE * pFile;
    // int n;
    std::string filename1="data/VUG_Padme";
    pFile = fopen (filename1.c_str(),"w");
    // write values to file
    fprintf(pFile," %s ",send_string);
    fclose(pFile);
    
    // and send value to file
    FILE * pFile3;
    std::string filename3="VUGDATA.txt";
    pFile3 = fopen (filename3.c_str(),"w");
    
    /*  example of data for monitor    
PLOTID TARGET_QXCh0
PLOTNAME BTF Data for Padme
PLOTTYPE text
DATA [ ["Timestamp","Mon Oct  1 15:07:05 2018"],["Producer status",0,0], ["DHPTB101 status",0,0],["BTF part type",0,1],["DHSTB001 status",0,0],["DHSTB002 status",0],["DHSTB001 Energy",4.50,2.00],["DHSTB002 Energy",0.00,2.00],["Padme Magnet current",50.00,60.0] ]
     */

      // write values to file
      fprintf(pFile3,"PLOTID DCS_VUG_Padme \nPLOTNAME VUG Data for Padme \nPLOTTYPE text \n");
      fprintf(pFile3,"DATA  [ [\"Timestamp\",\"%.24s\"]",dateconv3);
      fprintf(pFile3,",[\"VUG Producer status\",%s]",pch3[2]);
      fprintf(pFile3,",[\"VUG sensor status\",%s]",pch3[3]);
      fprintf(pFile3,",[\"Vacuum - mbar\",%.4e] ]\n",vacuum_pressure);
      fclose(pFile3);

      // and copy file to monitor@l0padme3
      string scp2="scp -q VUGDATA.txt monitor@l0padme3:PadmeMonitor/watchdir/. ";
      // cout << " scp command " << scp2 << endl; 
      char * writable2 = new char[scp2.size() + 1];
      std::copy(scp2.begin(), scp2.end(), writable2);
      writable2[scp2.size()] = '\0'; // don't forget the terminating 0
      // scp to monitor@l0padme3
      system(writable2);

     

  } else {
    fprintf(stderr, "Couldn't retrieve key %s: %s\n", key3,memcached_strerror(memc, rc));
    DrvBTFBeam_except::BTFBeamRetStatus(handle,3, "IP = "+fIPAddress+" ");
     
  }  


  // wait 5 secs for next read
  usleep(5000000);

}

