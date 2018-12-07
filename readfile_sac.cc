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
  char *inumber[700];
  int slot[700];
  int nch[700];
  int x[700];
  int y[700];
  char *inumber1[700];
  int slot1[700];
  int nch1[700];
  float hv[700];
  float current[700];
  float status[700];
  bool  debug=false;
  bool alarm=true;

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

  input.open ("maps/SacMap.txt");

  while(std::getline(input, line)) {
    if(line_number>0) {
      int ntoken=0;
      // cast string to char
      linec = strdup(line.c_str());

      //std::cout << "linec = " << linec << std::endl;
      int tokcount=0;
      int ik=line_number-1;
      while( (tok=strsep(&linec,";")) != NULL) {
	if(tokcount==0) inumber[ik]=tok;
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
      //printf(" line %d , inum=%s , x=%d , y=%d, slot=%d, nch=%d \n",
      //     ik,inumber[ik],x[ik],y[ik],slot[ik],nch[ik]);
     }
    line_number++;
    
    // std::cout<<line<<'\n';
  }

  printf(" Map file  is long %d lines \n",line_number);
  printf(" SAC values read =  %d \n",line_number-1);
 
  // and close the input file
  input.close();

  // now read the actual data file
  int line1_number=0;
  std::ifstream input1;  

  input1.open ("data/CAENHV_SAC");

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
	if(tokcount1==2) inumber1[ik]=tok;
	if(tokcount1==3) hv[ik]=atof(tok);
	if(tokcount1==4) current[ik]=atof(tok);
	if(tokcount1==5) status[ik]=atof(tok);
	tokcount1++;
	//printf(" line %d - token %d = %s \n",line_number,tokcount,tok);
      }
      //printf(" line1 %d , inum=%s , slot=%d, nch=%d , hv=%f, current=%f \n",
      //	     ik,inumber1[ik],slot1[ik],nch1[ik],hv[ik],current[ik]);
    }
    line1_number++;
    // std::cout<<line<<'\n';
  }
  printf(" Data file  is long %d lines \n",line1_number);
  printf(" Sac values read =  %d \n",line1_number-1);


  // and close the input file
  input1.close();

  int imatch=0;
  int imulti=0;

  float hv2[30][30];
  float current2[30][30];
  float status2[30][30];
  char *inumber2[30][30];
  int slot2[30][30];
  int ich2[30][30];
  int iused1[700];
  int iused[700];

  // zero matrices
  for(int x=0;x<5;x++) {
    for(int y=0;y<5;y++) {
      inumber2[x][y]='\0';
      hv2[x][y]=0.;
      current2[x][y]=0.;
      status2[x][y]=0.;
      slot2[x][y]=0.;
      ich2[x][y]=0.;
    }
  }
  for(int ic=0;ic<30;ic++) {
    iused[ic]=0;
    iused1[ic]=0;
  }
    
  // now loop over data and find x,y values
  for(int ik=0;ik<line_number-1;ik++) {
    // check number is equal
    for(int ik1=0;ik1<line1_number-1;ik1++) {
      if(strcmp(inumber[ik],inumber1[ik1])==0) {
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
	  //strcpy(inumber2[ix][iy],inumber[ik]);
	  iused1[ik1]++;
	  imatch++;
        } else {
	  printf(" number %s multiple used ! ik=%s - ik1=%d \n",inumber[ik],ik,ik1); 
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
      printf(" HV list channel %s - slot=%d - chan=%d not in calo map !\n",
	     inumber1[ii],slot1[ii],nch1[ii]);
    }
    if(iused1[ii]>1) {
      printf(" HV list channel %s - slot=%d - chan=%d is multiple iin calo map !\n",
	     inumber1[ii],slot1[ii],nch1[ii]);
    }
  }

  // Now check Calo map list unused channels
  for(int ii=0;ii<line_number-1;ii++) {
    if(iused[ii]==0) {
      printf(" CALO map channel %s - slot=%d - chan=%d not in HV map !\n",
	     inumber[ii],slot[ii],nch[ii]);
    }
    if(iused[ii]>1) {
      printf(" CALO map channel %s - slot=%d - chan=%d is multiple iin HV map!\n",
	     inumber[ii],slot[ii],nch[ii]);
    }
  }

  if(debug) {
  
    printf("\n");
    // and now print resulting matrix
    for (int iriga=0;iriga<5;iriga++) {
      printf(" riga %d - ",iriga);
      for(int icolonna=0;icolonna<5;icolonna++) {
	printf(" %.9s ",inumber2[iriga][icolonna]); 
      }
      printf("\n");
    }
 

    printf("\n");
    // and now print resulting matrix
    for (int iriga=0;iriga<5;iriga++) {
      for(int icolonna=0;icolonna<5;icolonna++) {
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
    // create and send alarm file
    FILE * pFile2;
    std::string filename2="alarm/sac_alarm.txt";
    pFile2 = fopen (filename2.c_str(),"w");
    fprintf(pFile2," <span foreground=\"red\" font=\"32\">DCS Alarm \n SAC HV Alarm\n at %.24s \n",timestamp);

    printf("SAC  - Checking channels for alarms \n");
    // check strange values for HV and current
    for (int ix=0;ix<5;ix++) {
      for(int iy=0;iy<5;iy++) {
	// check HV is up and valid channel
	if(hv2[ix][iy]<50. && inumber2[ix][iy]>0) {
	  printf(" ALARM : channel %s is at HV %5.2f - slot %d - channel %d \n",inumber2[ix][iy],hv2[ix][iy],slot2[ix][iy],ich2[ix][iy]); 
	  // and write file for alarm
	  // <span foreground=\"red\" font=\"32\">DCS Alarm \n ECAL HV Alarm\n\n<i> check channel 1 board 2 </i> </span>
	  fprintf(pFile2," <i> check channel %s - HV=%5.2f - slot %d, channel %d  \n</i> ",inumber2[ix][iy],hv2[ix][iy],slot2[ix][iy],ich2[ix][iy]);
	  ioff_channels++;
	}
	// check current is up and valid channel
	if(current2[ix][iy]<10. && inumber2[ix][iy]>0) {
	  printf(" ALARM : channel %s is at current %5.2f - slot %d - channel %d\n ",inumber2[ix][iy],current2[ix][iy],slot2[ix][iy],ich2[ix][iy]); 
	  fprintf(pFile2," <i> check channel %s - current=%5.2f - slot %d, channel %d  \n</i> ",inumber2[ix][iy],current2[ix][iy],slot2[ix][iy],ich2[ix][iy]);
	  ioff_channels++;
	}

      }
      // printf("\n");
    }
    // And close alarm file
    fprintf(pFile2," </span> ");
    fclose(pFile2);
    // and send alarm if there are off channels
    if(ioff_channels>0) {
      system("sendmessage_sac");
    }
  } else {
    printf(" ALARMS disabled ! \n");
  }

  // AND PRODUCE THE DATA FILE FOR THE MONITORING

  // and write data for monitoring
  FILE * pFile3;
  std::string filename3="monitor/SACHV";
  filename3+=".txt";
  pFile3 = fopen (filename3.c_str(),"w");
  float dummyhv=-1.0;
  float value;



  // write values to file
  //HV
  fprintf(pFile3,"PLOTID SAC_HV \n");
  fprintf(pFile3,"PLOTNAME SAC Voltages %s\nCHANNELS 5 5 \nRANGE_X 0 5\nRANGE_Y 0 5\nTITLE_X 5-Y\nTITLE_Y X\nPLOTTYPE heatmap\n",timestamp);
  fprintf(pFile3,"DATA  [ ");
  for (int ix=0;ix<5;ix++) {
    fprintf(pFile3,"[");
    for (int iy=4;iy>0;iy--) {
     if(inumber2[ix][iy]=='\0') { 
	value=dummyhv;
      } else {
        value=hv2[ix][iy];
      }
      fprintf(pFile3,"%5.2f,",value);
    }
    // last column
    if(inumber2[ix][0]=='\0') {
      value = dummyhv;
    } else {
      value=hv2[ix][0];
    }    
    // check last row
    if(ix<4) {
      fprintf(pFile3,"%5.2f ],",value);
    } else {
      fprintf(pFile3,"%5.2f ]",value);
    }
  }	
  fprintf(pFile3,"]\n");

  // write values to file
  //current
  fprintf(pFile3,"PLOTID SAC_CURRENT \n");
  fprintf(pFile3,"PLOTNAME SAC Currents %s\nCHANNELS 5 5 \nRANGE_X 0 5\nRANGE_Y 0 5\nTITLE_X 5-Y\nTITLE_Y X\nPLOTTYPE heatmap\n",timestamp);
  fprintf(pFile3,"DATA  [ ");
  for (int ix=0;ix<5;ix++) {
    fprintf(pFile3,"[");
    for (int iy=4;iy>0;iy--) {
      if(inumber2[ix][iy] =='\0') { 
	value=dummyhv;
      } else {
        value=current2[ix][iy];
      }
      fprintf(pFile3,"%5.2f,",value);
    }
    // last column
    if(inumber2[ix][0]=='\0') {
      value = dummyhv;
    } else {
      value=current2[ix][0];
    }    
    // check last row
    if(ix<4) {
      fprintf(pFile3,"%5.2f ],",value);
    } else {
      fprintf(pFile3,"%5.2f ]",value);
    }
  }	
  fprintf(pFile3,"]\n");


  // write values to file
  //status
  fprintf(pFile3,"PLOTID SAC_STATUS \n");
  fprintf(pFile3,"PLOTNAME SAC Status %s\nCHANNELS 5 5 \nRANGE_X 0 5\nRANGE_Y 0 5\nTITLE_X 5-Y\nTITLE_Y X\nPLOTTYPE heatmap\n",timestamp);
  fprintf(pFile3,"DATA  [ ");
  for (int ix=0;ix<5;ix++) {
    fprintf(pFile3,"[");
    for (int iy=4;iy>0;iy--) {
      if(inumber2[ix][iy]=='\0') { 
	value=dummyhv;
      } else {
        value=status2[ix][iy];
      }
      fprintf(pFile3,"%5.2f,",value);
    }
    // last column
    if(inumber2[ix][0]=='\0') {
      value = dummyhv;
    } else {
      value=status2[ix][0];
    }    
    // check last row
    if(ix<4) {
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
	  
  int ialarm_chan=0;
  
  if(alarm) {

     // and write data for monitoring alarm
     FILE * pFile4;
     std::string filename4="monitor/alarm_SACHV";
     filename4+=".txt";
     pFile4 = fopen (filename4.c_str(),"w");

     // and write data for monitoring alarm
     FILE * pFile5;
     std::string filename5="nchan_summ_alarm_SACHV";
     filename5+=".txt";
     pFile5 = fopen (filename5.c_str(),"w");

     // write to alarm monitor
     // write values to file
     //status
     fprintf(pFile4,"PLOTID SAC_ALARM \n");
     fprintf(pFile4,"PLOTNAME SAC Alarm %s\nPLOTTYPE activetext\n",timestamp);
     fprintf(pFile4,"DATA  [ ");
     

     for (int ix=0;ix<5;ix++) {
       for(int iy=0;iy<5;iy++) {
	 // check HV is up and valid channel
	 if( ( hv2[ix][iy]<400. || current2[ix][iy]<20. ) && inumber2[ix][iy]>0) {
	   if(ialarm_chan>0) fprintf(pFile4," , ");
	   fprintf(pFile4," {\"title\":\"SAC PMT \",\"current\": {\"value\":\"%.10s\",\"col\":\"#FF0000\"}},{\"title\":\"HV board \",\"current\": {\"value\":\"%d\",\"col\":\"#FF0000\"}},{\"title\":\"HV channel \",\"current\": {\"value\":\"%d\",\"col\":\"#FF0000\"}},{\"title\":\" HV \",\"current\": {\"value\":\"%5.2f\",\"col\":\"#FF0000\"}},{\"title\":\" Current \",\"current\": {\"value\":\"%5.2f\",\"col\":\"#FF0000\"}},{\"title\":\" Status \",\"current\": {\"value\":\"%5.2f\",\"col\":\"#FF0000\"}} ",inumber2[ix][iy],slot2[ix][iy],ich2[ix][iy],hv2[ix][iy],current2[ix][iy],status2[ix][iy]);
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

  printf(" SAC alarm channels = %d\n",ialarm_chan);


  return 0;
}
