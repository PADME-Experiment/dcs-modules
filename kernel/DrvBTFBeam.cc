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

 handle=0;  
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
  //char *key1 = "DAQ_BTF";
  char *key3 = "VUG_PADME";
  char *key2 = "BTFDATA_PADME";
  char *key1 = "BTFDATA_LOG";
  char *retrieved_value1;
  char *retrieved_value2;
  char *retrieved_value3;
  size_t value_length1;
  size_t value_length2;
  size_t value_length3;
  uint32_t flags;
  memcached_return rc;
  //char *serv;
  bool debug=false;
  memcached_server_st *servers = NULL;
  int iloop=0;
  memcached_st *memc;
 
 int portNum = 11211; // port number (same that server)


  char *retrieved_value;
  size_t value_length;
 
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

 
  if(iloop==0) {
    cout << "\n BTFBEAM - Starting memcached client with server "<< fIPAddress<< " - port "<< port << endl;
  }
  iloop++;

  if (rc == MEMCACHED_SUCCESS) {
    if(debug) {
      printf("Added server %s successfully\n",serv);
    }
  } else {
    fprintf(stderr, "Couldn't add server: %s\n", memcached_strerror(memc, rc));
    DrvBTFBeam_except::BTFBeamRetStatus(handle,6, "IP = "+fIPAddress+" ");
  }

  // key 2
  // here get value from memcache server
  retrieved_value2 = memcached_get(memc, key2, strlen(key2), &value_length2, &flags, &rc);
    //printf("Yay!\n");

  if (rc == MEMCACHED_SUCCESS) {
    if(debug) {
      printf("Key retrieved successfully  \n");
      printf("The key '%s' returned value '%s'.\n", key2,retrieved_value2);
    }
    int length=value_length2;
      
    // printf ("Splitting string DAQ into tokens:\n");
    char *BTF_string;
 
    BTF_string=strdup(retrieved_value2);
    free(retrieved_value2);
    
    if(debug) {
      cout <<  " memcache key " << key2 << " server says ";
      cout << BTF_string << " - length =  " << length << endl;
    }

    char * pch2[30];
    char *found;
    int itoken2=0;
    while ((found = strsep(&BTF_string,",")) != NULL ) {
 	pch2[itoken2]=found;
	itoken2++;
    }
   
    if(debug) {
      cout << "memcache key2 pieces = " << itoken2 << endl;
      for(int ik=0;ik<itoken2;ik++) {
	printf(" pch2[%d] = %s \n",ik,pch2[ik]);
      }
    }
    
    int last=14;

    // reset number of keys to only data fields
    if(itoken2>last) itoken2=last;

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

    // char *dateconv;
    // human readable time format
    //dateconv=asctime(localtime(&timedata));

    struct tm * timeinfo;
    char dateconv[25];
    // new format for timestamp
    timeinfo = localtime (&timedata);
    strftime(dateconv,25,"%Y-%m-%d %T",timeinfo);


    char send_string[70];
 
    // 9/11/2018 BTFDATA string enlarged from 10 to 13 values 
    // last 3 values : 
    // phase linac 0=e-/1=e+
    // linac up = 1
    // BTF phase =1
 
    // convert pch2[10] to string and strip last funny chars
    string pch2_last;
    // sprintf(pch2_10,"%s",pch2[10] );
    pch2_last=pch2[last];
    if(debug) {
      printf("pch2[%d] length before = %d \n",last,pch2_last.size());
    }
    // old resize
    //if (pch2_10.size () > 4)  pch2_10.resize (pch2_10.size () - 4);
    if (pch2_last.size () > 14)  pch2_last.resize (pch2_last.size () - 14);
    if(debug) {
      printf("pch2[%d] length after = %d \n",last,pch2_last.size());
    }
      
    sprintf(send_string, "%.24s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s \n",dateconv,pch2[2],pch2[3],pch2[4],pch2[5],pch2[6],pch2[7],pch2[8],pch2[9],pch2[10],pch2[11],pch2[12],pch2[13],pch2_last.c_str());
    int lenght2=strlen(send_string);
      
    cout << " memcache key2 " << key2 << " sent_value = "<< send_string << " - length =  " << lenght2 << endl;

    // and send value to file
    FILE * pFile;
    std::string filename1="data/BTFDATA_Padme";
    FILE * pFile_app;
    std::string filename_app="history/BTFDATA_Padme";
    pFile = fopen (filename1.c_str(),"w");
    pFile_app = fopen (filename_app.c_str(),"a");
    // write values to file
    fprintf(pFile," %s ",send_string);
    fclose(pFile);
    fprintf(pFile_app," %s ",send_string);
    fclose(pFile_app);
    
    // and send value to file
    FILE * pFile2;
    std::string filename2="monitor/BTFDATA.txt";
    pFile2 = fopen (filename2.c_str(),"w");

    
    char *parttype1="e-";
    if(strcmp(pch2[4],"0")!=0) parttype1="e+";
    //parttype1[3]='\0';
    char *parttype2="e+";
    if(strcmp(pch2[11],"1")!=0) parttype2="e-";
    //parttype2[3]='\0';
    char *producer="OK";
    if(strcmp(pch2[2],"0")!=0) producer="not OK";
    char *btfdata="OK";
    if(strcmp(pch2_last.c_str(),"1")!=0) btfdata="not OK";
    if(debug) {
      printf(" parttype1 = %s - parttype2= %s \n",parttype1,parttype2);
      printf(" producer status = %s - btfdata status = %s \n",producer,btfdata);
    }

    // write values to file
    fprintf(pFile2,"PLOTID DCS_BTF_Beam \nPLOTNAME BTF Data for Padme \nPLOTTYPE activetext \n");
    fprintf(pFile2,"DATA [ {\"title\":\"Timestamp\",\"current\":{\"value\":\"%.24s\"}}",dateconv);
    //fprintf(pFile2,",{\"title\":\"Producer status\",\"alarm\":{\"min\":\"-1\",\"max\":\"1\"},\"current\":{\"value\":\"%s\",\"col\":\"#00CC00\"}}",pch2[2]);
    fprintf(pFile2,",{\"title\":\"Producer status\",\"current\":{\"value\":\"%s\"}}",producer);
    fprintf(pFile2,",{\"title\":\"DHPTB101 status\",\"warn\":{\"min\":\"1\",\"max\":\"2\"},\"current\":{\"value\":\"%s\",\"col\":\"#00CC00\"}}",pch2[3]);
    fprintf(pFile2,",{\"title\":\"BTF part type\",\"current\":{\"value\":\"%s\"} }",parttype1);
    fprintf(pFile2,",{\"title\":\"Padme Magnet current\",\"warn\":{\"min\":\"220\",\"max\":\"260\"},\"alarm\":{\"min\":\"100\",\"max\":\"280\"},\"current\":{\"value\":\"%s\",\"col\":\"#00CC00\"} }",pch2[10]);
    // new keys from Luca 9/11/2018
    fprintf(pFile2,",{\"title\":\"Linac Phase\",\"current\":{\"value\":\"%s\"} }",parttype2);
    fprintf(pFile2,",{\"title\":\"Modulator status\",\"warn\":{\"min\":\"1\",\"max\":\"2\"},\"current\":{\"value\":\"%s\",\"col\":\"#00CC00\"} }",pch2[12]);
    fprintf(pFile2,",{\"title\":\"Gun status \",\"current\":{\"value\":\"%s\",\"col\":\"#00CC00\"},\"warn\":{\"min\":\"1\",\"max\":\"2\"} }",pch2[13]);
    //    fprintf(pFile2,",{\"title\":\"BTF data OK \",\"warn\":{\"min\":\"1\",\"max\":\"2\"},\"current\":{\"value\":\"%s\",\"col\":\"#00CC00\"} }]\n",pch2_last.c_str());
    fprintf(pFile2,",{\"title\":\"BTF data OK \",\"current\":{\"value\":\"%s\"} }]\n",btfdata);

    fprintf(pFile2,"PLOTID DHSTB001_BTF_Beam \nPLOTNAME DHSTB001 Data for Padme \nPLOTTYPE activetext \n");
    fprintf(pFile2,"DATA [ {\"title\":\"Timestamp\",\"current\":{\"value\":\"%.24s\"}}",dateconv);
    fprintf(pFile2,",{ \"title\":\"DHSTB001 status\",\"warn\":{\"min\":\"1\",\"max\":\"2\"} ,\"current\":{\"value\":\"%s\",\"col\":\"#00CC00\"} }",pch2[5]);
    fprintf(pFile2,",{ \"title\":\"DHSTB001 Energy\",\"warn\":{\"min\":\"500.0\",\"max\":\"600.0\"},\"current\":{\"value\":\"%s\",\"col\":\"#00CC00\"} }]\n",pch2[8]);

    fprintf(pFile2,"PLOTID DHSTB002_BTF_Beam \nPLOTNAME DHSTB002 Data for Padme \nPLOTTYPE activetext \n");
    fprintf(pFile2,"DATA [ {\"title\":\"Timestamp\",\"current\":{\"value\":\"%.24s\"}}",dateconv);
    fprintf(pFile2,",{ \"title\":\"DHSTB002 status\",\"warn\":{\"min\":\"1\",\"max\":\"2\"},\"current\":{\"value\":\"%s\",\"col\":\"#00CC00\"} }",pch2[6]);
    fprintf(pFile2,",{ \"title\":\"DHSTB002 Energy\",\"warn\":{\"min\":\"500.0\",\"max\":\"600.0\"},\"current\":{\"value\":\"%s\",\"col\":\"#00CC00\"} }]\n",pch2[9]);
    fclose(pFile2);
    
    // and copy file to monitor@l0padme3
    string scp="scp -q "+filename2+" monitor@l0padme3:PadmeMonitor/watchdir/. ";
    // cout << " scp command " << scp << endl; 
    char * writable1 = new char[scp.size() + 1];
    std::copy(scp.begin(), scp.end(), writable1);
    writable1[scp.size()] = '\0'; // don't forget the terminating 0
    // scp to monitor@l0padme3
    system(writable1);
    

  } else {
    fprintf(stderr, "Couldn't retrieve key %s: %s\n", key2,memcached_strerror(memc, rc));
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

    // old human readable format
    //char *dateconv3;
    //dateconv3=asctime(localtime(&timedata3));

    struct tm * timeinfo3;
    char dateconv3[25];
    // new format for timestamp
    timeinfo3 = localtime (&timedata3);
    strftime(dateconv3,25,"%Y-%m-%d %T",timeinfo3);
  
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
    std::string filename1="data/VUG_Padme";
    FILE * pFile_app;
    std::string filename_app="history/VUG_Padme";
    pFile = fopen (filename1.c_str(),"w");
    pFile_app = fopen (filename_app.c_str(),"a");
    // write values to file
    fprintf(pFile," %s ",send_string);
    fclose(pFile);
    fprintf(pFile_app," %s ",send_string);
    fclose(pFile_app);

    
    // and send value to file
    FILE * pFile3;
    std::string filename3="monitor/VUGDATA.txt";
    pFile3 = fopen (filename3.c_str(),"w");
    

      // write values to file
      fprintf(pFile3,"PLOTID DCS_VUG_Padme \nPLOTNAME VUG Data for Padme \nPLOTTYPE activetext \n");
      fprintf(pFile3,"DATA  [ {\"title\":\"Timestamp\",\"current\":{\"value\":\"%.24s\"}}",dateconv3);
      fprintf(pFile3,",{\"title\":\"VUG producer status\",\"alarm\":{\"min\":\"-1\",\"max\":\"1\"},\"current\":{\"value\":\"%s\",\"col\":\"#00CC00\"}}",pch3[2]);
      fprintf(pFile3,",{\"title\":\"VUG sensor status\",\"alarm\":{\"min\":\"-1\",\"max\":\"1\"},\"current\":{\"value\":\"%s\",\"col\":\"#00CC00\"}}",pch3[3]);
      fprintf(pFile3,",{\"title\":\"Vacuum - mbar\",\"warn\":{\"max\":\"1.e-5\"},\"alarm\":{\"max\":\"1.e-4\"},\"current\":{\"value\":\"%.4e\",\"col\":\"#00CC00\"}} ]\n",vacuum_pressure);
      fclose(pFile3);

      // and copy file to monitor@l0padme3
      string scp2="scp -q "+filename3+" monitor@l0padme3:PadmeMonitor/watchdir/. ";
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



  // get key1
  retrieved_value1 = memcached_get(memc, key1, strlen(key1), &value_length1, &flags, &rc);

  if (rc == MEMCACHED_SUCCESS) {
    if(debug) {
      printf("The key %s on server %s was OK \n",key1,serv);
      printf("Key  value= %s \n",retrieved_value1);
      // cout << " key value = << retrieved_value << endl;
      printf(" the key length is %d \n",value_length1);
      // free(retrieved_value);
    }

    int length=value_length1;
      
    // printf ("Splitting string DAQ into tokens:\n");
    // get only last 4 values of all string
    char *BTF_string;
 
    BTF_string=strdup(retrieved_value1);
    free(retrieved_value1);
  
    
    char * pch3[100];
    char * pch4[100][2];
    char *found;
    int itoken3=0;
    while ((found = strsep(&BTF_string,"\t\r\n")) != NULL ) {
 	pch3[itoken3]=found;
	itoken3++;
    }
    if(debug) printf("memcache key pieces = %d \n ",itoken3);
    
    int ij,ii;
    int maxnum=37;
    ii=0;
    for(ij=0;ij<maxnum;ij++) {
      pch4[ii][1]=pch3[ij];
      pch4[ii][2]=" ";
      if(ij>5 && ij<23 ) {
	pch4[ii][2]=pch3[ij+1];
	ij++;
      } else {
      }
      ii++;
    }

    if(debug) {
      printf(" new number of keys =%d \n",ii);
      for(ij=0;ij<ii;ij++) {
	// double values read
	printf(" %d -  %s = %s %s \n",ij,pch3[ij+maxnum+1],pch4[ij][1],pch4[ij][2]);
      }
    }
    
    // if timestamp convert to our timestamp
    // convert labview timestamp to normal timestamp
    double timechar=atof(pch4[1][1]);
    // and subtract seconds from 0:0 1/1/1970 to 0:00 1/1/1904
    double  time2 = timechar -  2082844800.;
    // printf(" new time is %f \n",time2);
    char array[100];
    sprintf(array, "%f", time2);
    // printf(" array is %s \n",array);
    
    // convert first data to normal time stamp
    time_t timedata = atoi(array); // convert to time_t, ignores msec
    
    struct tm * timeinfo;
    char dateconv1[25];
    // new format for timestamp
    timeinfo = localtime (&timedata);
    strftime(dateconv1,25,"%Y-%m-%d %T",timeinfo);
    // and print converted data
    printf(" converted timestamp = %.24s \n",dateconv1);
 
    char send_string[300];
    sprintf(send_string, "%.24s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s \n",
	    dateconv1,pch3[0],pch3[6],pch3[7],pch3[8],pch3[9],pch3[10],pch3[11],pch3[12],pch3[13],pch3[15],pch3[16],
pch3[17],pch3[18],pch3[19],pch3[20],pch3[21],pch3[22],pch3[28],pch3[29],pch3[30],pch3[31],pch3[35],pch3[36]);

    int lenght1=strlen(send_string);    
    cout << " memcache key1 " << key1 << " sent_value = "<< send_string << " - length =  " << lenght1 << endl;



    // and send value to file
    FILE * pFile;
    std::string filename1="data/BTF_LOG";
    FILE * pFile_app;
    std::string filename_app="history/BTF_LOG";
    pFile = fopen (filename1.c_str(),"w");
    pFile_app = fopen (filename_app.c_str(),"a");
    // write values to file
    fprintf(pFile," %s ",send_string);
    fclose(pFile);
    fprintf(pFile_app," %s ",send_string);
    fclose(pFile_app);

    // and send value to file
    FILE * pFile3;
    std::string filename3="monitor/BTFLOG.txt";
    pFile3 = fopen (filename3.c_str(),"w");

   // write values to file
  fprintf(pFile3,"PLOTID BTF_LOG \nPLOTNAME BTF Collimator data \nPLOTTYPE text \n");
  fprintf(pFile3,"DATA  [ [\"Timestamp\",\"%.24s\"]",dateconv1);
  fprintf(pFile3,",[\"TB4 Right set/RO \",\"%s  %s\"]",pch3[6],pch3[15]);
  fprintf(pFile3,",[\"TB4 Left set/RO \",\"%s  %s\"]",pch3[7],pch3[16]);
  fprintf(pFile3,",[\"TB2 Right set/RO \",\"%s  %s\"]",pch3[8],pch3[17]);
  fprintf(pFile3,",[\"TB2 Left set/RO \",\"%s  %s\"]",pch3[9],pch3[18]);
  fprintf(pFile3,",[\"TB3 Up set/RO \",\"%s  %s\"]",pch3[10],pch3[19]);
  fprintf(pFile3,",[\"TB3 Down set/RO \",\"%s  %s\"]",pch3[11],pch3[20]);
  fprintf(pFile3,",[\"TB1 Up set/RO \",\"%s  %s\"]",pch3[12],pch3[21]);
  fprintf(pFile3,",[\"TB1 Down set/RO \",\"%s  %s\"] ]\n",pch3[13],pch3[22]);
  fprintf(pFile3,"PLOTID BTF_LOG1 \nPLOTNAME BTF trigger data \nPLOTTYPE text \n");
  fprintf(pFile3,"DATA [ [\"Padme trigger delay \",\"%s\"]",pch3[28]);
  fprintf(pFile3,",[\"Padme gate time \",\"%s\"]",pch3[29]);
  fprintf(pFile3,",[\"Pulser trigger delay \",\"%s\"]",pch3[30]);
  fprintf(pFile3,",[\"Pulser gate time \",\"%s\"]",pch3[31]);
  fprintf(pFile3,",[\"BTF1 Pressure \",\"%s\"]",pch3[35]);
  fprintf(pFile3,",[\"BTF1 Press sens status \",\"%s\"] ]\n",pch3[36]);
  fclose(pFile3);

  // and copy file to monitor@l0padme3
  string scp2="scp -q "+filename3+" monitor@l0padme3:PadmeMonitor/watchdir/. ";
  // cout << " scp command " << scp2 << endl; 
  char * writable2 = new char[scp2.size() + 1];
  std::copy(scp2.begin(), scp2.end(), writable2);
  writable2[scp2.size()] = '\0'; // don't forget the terminating 0
  // scp to monitor@l0padme3
  system(writable2);
  

  } else {
    fprintf(stderr, "Couldn't retrieve key %s: %s\n", key1,memcached_strerror(memc, rc));
    DrvBTFBeam_except::BTFBeamRetStatus(handle,3, "IP = "+fIPAddress+" ");
  }
  
 

  // and free memc
  memcached_servers_reset(memc);
  memcached_free(memc);


  // wait 20 secs for next read
  usleep(20000000);

}

