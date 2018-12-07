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
  int inumber[700];
  int slot[700];
  int nch[700];
  int x[700];
  int y[700];
  int inumber1[700];
  int slot1[700];
  int nch1[700];
  float hv[700];
  float current[700];
  float status[700];
  bool alarm=true;
  bool debug=false;

  int line_number=0;
  std::ifstream input;  

   if( argc == 2 ) {
     printf("The argument supplied is %s\n", argv[1]);
     // default is alarm on - if 0 alarm=false
     if(strcmp(argv[1],"0")==0) alarm=false;
   }
   else if( argc > 2 ) {
      printf("Too many arguments supplied.\n");
      return 1;
   }
   else {
      printf("One argument expected.\n");
      return 1;
   }

  input.open ("maps/CalorimeterMap20pC.txt");

  while(std::getline(input, line)) {
    if(line_number>0) {
      int ntoken=0;
      // cast string to char
      linec = strdup(line.c_str());

      //std::cout << "linec = " << linec << std::endl;
      int tokcount=0;
      int ik=line_number-1;
      while( (tok=strsep(&linec,";")) != NULL) {
	if(tokcount==0) inumber[ik]=atoi(tok);
	if(tokcount==1) x[ik]=atoi(tok);
	if(tokcount==2) y[ik]=atoi(tok);
	if(tokcount==3) slot[ik]=atoi(tok);
	if(tokcount==4) {
	  // strip 0 if present
	  //if(tok[0]=='0') tok[0]=' ';
	  nch[ik]=atoi(tok);
	}
	tokcount++;
	//printf(" line %d - token %d = %s \n",line_number,tokcount,tok);
      }
      //printf(" line %d , inum=%d , x=%d , y=%d, slot=%d, nch=%d \n",
      //	     ik,inumber[ik],x[ik],y[ik],slot[ik],nch[ik]);
     }
    line_number++;
    
    // std::cout<<line<<'\n';
  }

  printf(" Map file  is long %d lines \n",line_number);
  printf(" Ecal values read =  %d \n",line_number-1);
 
  // and close the input file
  input.close();

  // now read the actual data file
  int line1_number=0;
  std::ifstream input1;  

  input1.open ("data/CAENHV_ECAL");

  while(std::getline(input1, line)) {
    if(line1_number>0) {
      int ntoken1=0;
      // cast string to char
      linec = strdup(line.c_str());

      //std::cout << "linec = " << linec << std::endl;
      int tokcount1=0;
      int ik=line1_number-1;
      while( (tok=strsep(&linec,";")) != NULL) {
	if(tokcount1==0) slot1[ik]=atoi(tok);
	if(tokcount1==1) {
	  // strip 0 if present
	  //if(tok[0]=='0') tok[0]=' ';
	  nch1[ik]=atoi(tok);
	}
	if(tokcount1==2) inumber1[ik]=atoi(tok);
	if(tokcount1==3) hv[ik]=atof(tok);
	if(tokcount1==4) current[ik]=atof(tok);
	if(tokcount1==5) status[ik]=atof(tok);
	tokcount1++;
	//printf(" line %d - token %d = %s \n",line_number,tokcount,tok);
      }
      //printf(" line1 %d , inum=%d , slot=%d, nch=%d , hv=%f, current=%f \n",
      //	     ik,inumber1[ik],slot1[ik],nch1[ik],hv[ik],current[ik]);
    }
    line1_number++;
    // std::cout<<line<<'\n';
  }
  printf(" Data file  is long %d lines \n",line1_number);
  printf(" Ecal values read =  %d \n",line1_number-1);


  // and close the input file
  input1.close();

  int imatch=0;
  int imulti=0;

  float hv2[30][30];
  float current2[30][30];
  float status2[30][30];
  int slot2[30][30];
  int ich2[30][30];
  int inumber2[30][30];
  int iused1[700];
  int iused[700];

  // zero matrices
  for(int x=0;x<30;x++) {
    for(int y=0;y<30;y++) {
      inumber2[x][y]=0;
      hv2[x][y]=0.;
      current2[x][y]=0.;
      status2[x][y]=0.;
    }
  }
  for(int ic=0;ic<699;ic++) {
    iused[ic]=0;
    iused1[ic]=0;
  }
    
  // now loop over data and find x,y values
  for(int ik=0;ik<line_number-1;ik++) {
    // check number is equal
    for(int ik1=0;ik1<line1_number-1;ik1++) {
      if(inumber1[ik1]==inumber[ik]) {
	if(iused1[ik1]==0) {
	  iused[ik]++;
	  int ix=x[ik];
	  int iy=y[ik];
	  slot2[ix][iy]=slot1[ik];
	  ich2[ix][iy]=nch1[ik];
	  hv2[ix][iy]=hv[ik1];
	  current2[ix][iy]=current[ik1];
	  status2[ix][iy]=status[ik1];
	  inumber2[ix][iy]=inumber[ik];
	  iused1[ik1]++;
	  imatch++;
        } else {
	  printf(" number %d multiple used ! ik=%d - ik1=%d \n",inumber[ik],ik,ik1); 
	  printf(" xnew=%d  ynew=%d used1=%d used=%d \n",x[ik],y[ik],iused1[ik1],iused[ik]);
	  printf(" slot %d - nch %d \n",slot[ik],nch[ik]);
	  iused1[ik]++;
	  imulti++; 
	}
	//break;
      }
    }  
  }


  printf(" found %d matches in calorimeter map\n",imatch);
  printf(" found %d multiples in calorimeter map\n",imulti);

  // Now check HV list unused channels
  for(int ii=0;ii<line1_number-1;ii++) {
    if(iused1[ii]==0) {
      printf(" HV list channel %d - slot=%d - chan=%d not in calo map !\n",
	     inumber1[ii],slot1[ii],nch1[ii]);
    }
    if(iused1[ii]>1) {
      printf(" HV list channel %d - slot=%d - chan=%d is multiple iin calo map !\n",
	     inumber1[ii],slot1[ii],nch1[ii]);
    }
  }

  // Now check Calo map list unused channels
  for(int ii=0;ii<line_number-1;ii++) {
    if(iused[ii]==0) {
      printf(" CALO map channel %d - slot=%d - chan=%d not in HV map !\n",
	     inumber[ii],slot[ii],nch[ii]);
    }
    if(iused[ii]>1) {
      printf(" CALO map channel %d - slot=%d - chan=%d is multiple iin HV map!\n",
	     inumber[ii],slot[ii],nch[ii]);
    }
  }

 
  if(debug) {
    printf("\n");
    // and now print resulting matrix
    for (int iriga=0;iriga<29;iriga++) {
      for(int icolonna=0;icolonna<29;icolonna++) {
	printf(" %8d ",inumber2[iriga][icolonna]); 
      }
      printf("\n");
    }
    
    printf("\n");
    // and now print resulting matrix
    for (int iriga=0;iriga<29;iriga++) {
      for(int icolonna=0;icolonna<29;icolonna++) {
	printf(" %8d ",int(round(hv2[iriga][icolonna]))); 
      }
      printf("\n");
    }
  }


  time_t ticks;
  struct tm * timeinfo;
  char timestamp[25];

  // old human readable timestamp
  // time ( &rawtime );
  //timeinfo = localtime ( &rawtime );
  //timestamp=asctime(timeinfo);

  // new formatted timestring
  time (&ticks);
  timeinfo = localtime (&ticks);
  strftime (timestamp,25,"%Y-%m-%d %T",timeinfo);

 
  int ioff_channels=0;

  if(alarm) {
    // create and send alarm files
    FILE * pFile2;
    std::string filename2="alarm/ecal_alarm.txt";
    pFile2 = fopen (filename2.c_str(),"w");
    fprintf(pFile2," <span foreground=\"red\" font=\"32\">DCS Alarm \n ECAL HV Alarm\n at %.24s \n",timestamp);

    printf("ECAL  - Checking channels for alarms \n");
    // check strange values for HV and current
    for (int ix=0;ix<29;ix++) {
      for(int iy=0;iy<29;iy++) {
	// check HV is up and valid channel
	if(hv2[ix][iy]<100. && inumber2[ix][iy]>0) {
	  printf(" ALARM : channel %8d is at HV %5.2f - slot %d - channel %d \n",inumber2[ix][iy],hv2[ix][iy],slot2[ix][iy],ich2[ix][iy]); 
	  // and write file for alarm
	  // <span foreground=\"red\" font=\"32\">DCS Alarm \n ECAL HV Alarm\n\n<i> check channel 1 board 2 </i> </span>
	  fprintf(pFile2," <i> check channel %d - HV=%5.2f - slot %d, channel %d  \n</i> ",inumber2[ix][iy],hv2[ix][iy],slot2[ix][iy],ich2[ix][iy]);
	  ioff_channels++;
	}
	// check current is up and valid channel
	if(current2[ix][iy]<20. && inumber2[ix][iy]>0) {
	  printf(" ALARM : channel %8d is at current %5.2f - slot %d - channel %d \n",inumber2[ix][iy],current2[ix][iy],slot2[ix][iy],ich2[ix][iy]); 
	  fprintf(pFile2," <i> check channel %d - current=%5.2f - slot %d, channel %d  \n</i> ",inumber2[ix][iy],current2[ix][iy],slot2[ix][iy],ich2[ix][iy]);
	  ioff_channels++;
	}
      
      }
      //printf("\n");
    }
    // And close alarm file
    fprintf(pFile2," </span> ");
    fclose(pFile2);
 
    // and send alarm if there are channels off
    if(ioff_channels>0) {
      system("sendmessage_ecal");
      
    }
    
    printf(" Channels with alarm = %d \n",ioff_channels);
  } else {

    printf(" ALARMS disabled ! \n");
  }
  
  // AND PRODUCE THE DATA FILE FOR THE MONITORING

  // and write data for monitoring
  FILE * pFile3;
  std::string filename3="monitor/ECALHV";
  filename3+=".txt";
  pFile3 = fopen (filename3.c_str(),"w");
  float dummyhv=-1.0;
  float value;
 
 // write values to file
  //HV
  fprintf(pFile3,"PLOTID ECAL_HV \n");
  fprintf(pFile3,"PLOTNAME ECAL Voltages %s \nCHANNELS 29 29 \nRANGE_X 0 29\nRANGE_Y 0 29\nTITLE_X X\nTITLE_Y Y\nPLOTTYPE heatmap\n",timestamp);
  fprintf(pFile3,"DATA  [ ");
  for (int iy=0;iy<29;iy++) {
    fprintf(pFile3,"[");
    for (int ix=0;ix<28;ix++) {
      if(inumber2[ix][iy]==0) { 
	value=dummyhv;
      } else {
        value=hv2[ix][iy];
      }
      fprintf(pFile3,"%5.2f,",value);
    }
    // last column
    if(inumber2[28][iy]==0) {
      value = dummyhv;
    } else {
      value=hv2[28][iy];
    }    
    // check last row
    if(iy<28) {
      fprintf(pFile3,"%5.2f ],",value);
    } else {
      fprintf(pFile3,"%5.2f ]",value);
    }
  }	
  fprintf(pFile3,"]\n");

  // write values to file
  //current
  fprintf(pFile3,"PLOTID ECAL_CURRENT \n");
  fprintf(pFile3,"PLOTNAME ECAL Currents %s \nCHANNELS 29 29 \nRANGE_X 0 29\nRANGE_Y 0 29\nTITLE_X X\nTITLE_Y Y\nPLOTTYPE heatmap\n",timestamp);
  fprintf(pFile3,"DATA  [ ");
  for (int iy=0;iy<29;iy++) {
    fprintf(pFile3,"[");
    for (int ix=0;ix<28;ix++) {
      if(inumber2[ix][iy] >0) { 
	value=current2[ix][iy];
      } else {
        value=dummyhv;
      }
      fprintf(pFile3,"%5.2f,",value);
    }
    // last column
    if(inumber2[28][iy]==0) {
      value = dummyhv;
    } else {
      value=current2[28][iy];
    }    
    // check last row
    if(iy<28) {
      fprintf(pFile3,"%5.2f ],",value);
    } else {
      fprintf(pFile3,"%5.2f ]",value);
    }
  }	
  fprintf(pFile3,"]\n");


  // write values to file
  //status
  fprintf(pFile3,"PLOTID ECAL_STATUS \n");
  fprintf(pFile3,"PLOTNAME ECAL Status %s\nCHANNELS 29 29 \nRANGE_X 0 29\nRANGE_Y 0 29\nTITLE_X X\nTITLE_Y Y\nPLOTTYPE heatmap\n",timestamp);
  fprintf(pFile3,"DATA  [ ");
  for (int iy=0;iy<29;iy++) {
    fprintf(pFile3,"[");
    for (int ix=0;ix<28;ix++) {
      if(inumber2[ix][iy] >0) { 
	value=status2[ix][iy];
      } else {
        value=dummyhv;
      }
      fprintf(pFile3,"%5.2f,",value);
    }
    // last column
    if(inumber2[28][iy]==0) {
      value = dummyhv;
    } else {
      value=status2[28][iy];
    }    
    // check last row
    if(iy<28) {
      fprintf(pFile3,"%5.2f ],",value);
    } else {
      fprintf(pFile3,"%5.2f ]",value);
    }	
  }
  fprintf(pFile3,"]\n");

  // and close the file
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
 

  if(alarm) {

     // and write data for monitoring alarm
     FILE * pFile4;
     std::string filename4="monitor/alarm_ECALHV";
     filename4+=".txt";
     pFile4 = fopen (filename4.c_str(),"w");

     // and write data for monitoring alarm
     FILE * pFile5;
     std::string filename5="nchan_summ_alarm_ECALHV";
     filename5+=".txt";
     pFile5 = fopen (filename5.c_str(),"w");

     // write to alarm monitor
     // write values to file
     //status
     fprintf(pFile4,"PLOTID ECAL_ALARM \n");
     fprintf(pFile4,"PLOTNAME ECAL Alarm %s\nPLOTTYPE activetext\n",timestamp);
     fprintf(pFile4,"DATA  [ ");
     
     int ialarm_chan=0;

     for (int ix=0;ix<29;ix++) {
       for(int iy=0;iy<29;iy++) {
	 // check HV is up and valid channel
	 if( ( hv2[ix][iy]<950. || current2[ix][iy]<30. ) && inumber2[ix][iy]>0) {
	   if(ialarm_chan>0) fprintf(pFile4," , ");
	   fprintf(pFile4," {\"title\":\"ECAL PMT \",\"current\": {\"value\":\"%d\",\"col\":\"#FF0000\"}},{\"title\":\"HV board \",\"current\": {\"value\":\"%d\",\"col\":\"#FF0000\"}},{\"title\":\"HV channel \",\"current\": {\"value\":\"%d\",\"col\":\"#FF0000\"}},{\"title\":\" HV \",\"current\": {\"value\":\"%5.2f\",\"col\":\"#FF0000\"}},{\"title\":\" Current \",\"current\": {\"value\":\"%5.2f\",\"col\":\"#FF0000\"}},{\"title\":\" Status \",\"current\": {\"value\":\"%5.2f\",\"col\":\"#FF0000\"}} ",inumber2[ix][iy],slot2[ix][iy],ich2[ix][iy],hv2[ix][iy],current2[ix][iy],status2[ix][iy]);
	   ialarm_chan++;
	 }
       }
     }
     fprintf(pFile4,"]\n");
     fclose(pFile4);

     
     // if alarm exist send it to monitor
     std::string scp4="scp -q ";
     scp4+=filename4;
     scp4+=" monitor@l0padme3:PadmeMonitor/watchdir/. ";
     // cout << " scp command " << scp2 << endl; 
     char * writable4 = new char[scp4.size() + 1];
     std::copy(scp4.begin(), scp4.end(), writable4);
     writable4[scp4.size()] = '\0'; // don't forget the terminating 0
     // scp to monitor@l0padme3
     system(writable4);

     // anad print to file how many alarm channels
     fprintf(pFile5," %d\n",ialarm_chan);
     fclose(pFile5);
    
  } // endif alarm

  return 0;
}
