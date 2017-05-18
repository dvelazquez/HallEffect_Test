#include <stdlib.h>  
 #include <stdio.h>  
 #include <string.h>  
 #include <unistd.h>     //close()  
 #include <fcntl.h>     //define O_WONLY and O_RDONLY  
 #include <time.h>
 #define MAX_BUF 64     //This is plenty large  
 
 //Function declarations  
 int readADC(unsigned int pin);  
 char FileName1[70];
 char FileName2[70];
 char Date[70];
 int Toggle=0;
 int Log;
 FILE *fp1;
 FILE *fp2;
 int Vueltas = 0;
 int OUT1=0;
 int OUT2=0;




 //main program
 int main()
 {
      //Enable ADC pins within code  
        //system("echo BB-ADC > /sys/devices/bone_capemgr.*/slots");
	strcat(FileName1,"/media/C4C1-97A4/Log_HallEffect_Test_Out1.csv");
	strcat(FileName2,"/media/C4C1-97A4/Log_HallEffect_Test_Out2.csv");

        fp1= fopen(FileName1,"w+");
        fp2= fopen(FileName2,"w+");


   //Read ADCs
   while(1){
      int IR = readADC(5);

      //Print ADC readings
      printf("IR: %d\n",IR);  // IR is around 1700 when HI, lets use < 1000 for Low
//      fprintf(fp1,"%i,",IR);


	if(IR<1000){
		if (Toggle==1){
			Vueltas=Vueltas+1;
			Toggle=0;

			time_t t= time(NULL);
			struct tm *tm = localtime(&t);
			//printf("%s\n", asctime(tm));
			strftime(Date, sizeof(Date), "%c",tm);
			fprintf(fp1,"\n%s,OUT1-%i,",Date,Vueltas);
			strftime(Date, sizeof(Date), "%c",tm);
			fprintf(fp2,"\n%s,OUT2-%i,",Date,Vueltas);
		}
	}
	else
		Toggle=1;
	printf("Vuelta %i\n",Vueltas);
	OUT1 = readADC(1);
	printf("ADC 1: %d\n",OUT1);
      	fprintf(fp1,"%i,",OUT1);

	OUT2 = readADC(3);
      	printf("ADC 3: %d\n",OUT2);
      	fprintf(fp2,"%i,",OUT2);
      	//system("clear");
	// Need to reset the variables and check why every 16th OUT1 puts OUT2s 16th
	OUT1=0;
	OUT2=0;
}
      fclose(fp1);
      fclose(fp2);

      return 0;  
}//end main  

 //Function definitions  
 int readADC(unsigned int pin)  
 {  
      int fd;          //file pointer  
      char buf[MAX_BUF];     //file buffer  
      char val[4];     //holds up to 4 digits for ADC value  
      strcat(val,"0");      

      //Create the file path by concatenating the ADC pin number to the end of the string  
      //Stores the file path name string into "buf"  
      snprintf(buf, sizeof(buf), "/sys/devices/ocp.3/helper.17/AIN%d", pin);     //Concatenate ADC file name  
      
      fd = open(buf, O_RDONLY);     //open ADC as read only  
      
      //Will trigger if the ADC is not enabled  
      if (fd < 0) {  
           perror("ADC - problem opening ADC");  
      }//end if  
      
      read(fd, &val, 4);     //read ADC ing val (up to 4 digits 0-1799)  
      usleep(1000);
      close(fd);     //close file and stop reading  
      
      return atoi(val);     //returns an integer value (rather than ascii)  
 }//end read ADC()
