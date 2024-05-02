#include "../../Headers/HAL/GPS_module.h"
#include "../../Headers/MCAL/UART.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


char logElements[12][20];
char * pointer; 
//char message[]="$GPRMC, 203522.00, A, 5109.0262308, S, 11401.8407342, E, 0.004, 133. 4, 130522, 0.0, E, D*2B";
char log_name[]="$GPRMC, ";
char recievedChar[100];  
int correct =0;
char c0,c1,c2,c3,c4,c5,c6;

static int x=0;


int GPS_read(char *GPS_module)
{ 
    int flag = 1;
    c0 = charIn2(); //$
    if (c0 == log_name[0]) 
    {   
        c1 = charIn2();  //G
        if (c1 == log_name[1])
        {
            c2 = charIn2();  //p
            if (c2 == log_name[2])
            {
                c3 = charIn2();   //R
                if (c3 == log_name[3])
                {
                    c4 = charIn2();    //M
                    if (c4 == log_name[4])
                    {
                        c5 = charIn2();     //C
                        if (c5 == log_name[5])
                        {
                            c6 = charIn2();
                            if (c6 == log_name[6]) //,
                            {
                                int i = 0;
                                do
                                { 
                                    GPS_module[i] = charIn2(); 
                                    i++;
                                } while (charIn2()!= '*');
                                flag=1;
                              
                            }
                            else
                            {
                                flag = 0;
                            }
                        }
                        else
                        {
                            flag = 0;
                        }
                    }
                    else
                    {
                        flag = 0;
                    }
                }
                else
                {
                    flag = 0;
                }
            }
            else
            {
                flag = 0;
            }
        }
        else
        {
            flag = 0;
        }
    }
    else
    {
        flag = 0;
    }

    return flag;
}


																					
		
	
	int GPS_output_format(char * GPS_module,float *mess){
	   
	int  flag =1;
	 char numOfGPSElements=0;
		flag= GPS_read(GPS_module);

		if(flag==1)
		{
		pointer =strtok(GPS_module,", ");
		do{
		strcpy(logElements[numOfGPSElements], pointer);
			pointer=strtok(NULL,", ");
			numOfGPSElements++;
		}while(pointer!=NULL);

		if(strcmp(logElements[1],"A")==0)
		{      
					if(strcmp(logElements[3],"N")==0)
					{
						mess[0]=atof(logElements[2]);
					printf("currLatit: %f \n",mess[0]);	
					}	
					else if(strcmp(logElements[3],"S")==0){
					mess[0] =-atof(logElements[2]);
					printf("currLatit: %f \n",mess[0]);	
					}
					if(strcmp(logElements[5],"E")==0){
						mess[1]=atof(logElements[4]);
					 printf("currLong: %f",mess[1]);	
					}
					else if(strcmp(logElements[5],"W")==0){
					mess[1] =-atof(logElements[4]);
					printf("currLong: %f",	mess[1]);	
					}
			
		}		
		else if(strcmp(logElements[2],"V")==0)
		{ 
				//printf("INVALID DATA");
				flag =0;
		}
	}
		return flag;
}

           
	
//then we are sure that we are recieving the correct data 

// for formating the gps module output:
		
	


float toDegree(float angle) {
	float angleindegree=(int)angle/100;
	float mins = angle -(float)angleindegree*100;
	return (angleindegree + (mins/60));
}

float toRad(float angle)
{
	return ((angle * PI) / 180);
}

float harversine(float LongAinRad, float LatAinRad, float LongBinRad, float LatBinRad)
{
		//we will use Haversine law to calculate the distance on sphere
		float x=pow(sin((LatBinRad-LatAinRad)/2),2);
		float y=pow(sin((LongBinRad-LongAinRad)/2),2);
		float a=x+y*cos(LatAinRad)*cos(LatBinRad);
		float c = 2*atan2(sqrt(a),sqrt(1-a));
		return earthradius*c;
}

float obtainDistance(float LongA, float LatA, float LongB, float LatB) {
	//first we need to convert the distance given to radian
		float LatAinRad=toRad(toDegree(LatA));
		float LatBinRad=toRad(toDegree(LatB));
		float LongAinRad=toRad(toDegree(LongA));
		float LongBinRad=toRad(toDegree(LongB));
	//now we have them in radian
	//now we call Harversine method to get the distance
		return harversine(LongAinRad,LatAinRad,LongBinRad,LatBinRad);
}
