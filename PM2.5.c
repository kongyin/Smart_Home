byte PM2_5;
void setup()
{
  Serial.begin(9600);
  Serial2.begin(9600);  
}
串口接受数据：
void loop()
{
  int i=0;
  byte c[3];
  while(1)
    {
    if(Serial2.available()>0)
       {
          c[i]=Serial2.read();
              
        
          if(i==0 && c[i]!=0xAA)
            i=-1;
          if(i==1 && c[i]!=0xBB)
            i=-1;

          i++;
          if( i==3 )
            break;
      
        }     
    }
      PM2_5=c[2];
      Serial.print("PM2_5: ");
      Serial.println(PM2_5);
 }

PM2.5
#include <dht11.h>
dht11 DHT11;
#define DHT11PIN 3

int pin2_5 = 7;
int pin1 = 10;
int count=0;
int Data;
int Data1;
float temp;
float add=0;

unsigned long duration2_5;
unsigned long duration1;
unsigned long starttime;
unsigned long sampletime_ms = 30000;
unsigned long lowpulseoccupancy2_5 = 0;
unsigned long lowpulseoccupancy1 = 0;
float ratio2_5 = 0;
float ratio1 =0;

float concentration2_5 = 0;
float concentration1 = 0;
float concentration =0;
float PM2_5;
float PM[5];

void setup()
{
  Serial.begin(9600);
  pinMode(7,INPUT);   //2.5
  pinMode(10,INPUT);   //1
  starttime = millis();
}

  void loop()
{       
            duration2_5 = pulseIn(pin2_5, LOW);
            duration1 = pulseIn(pin1,LOW);
  
           lowpulseoccupancy2_5 = lowpulseoccupancy2_5+duration2_5;
           lowpulseoccupancy1 = lowpulseoccupancy1+ duration1;
            
           Serial.print("Time:");
           Serial.println(millis()-starttime);
        
           Serial.print("lowpulseconpancy2_5: ");
           Serial.println(lowpulseoccupancy2_5);
           Serial.print("lowpulseoccupancy1: ");
           Serial.println(lowpulseoccupancy1);
    
           if ((millis()-starttime) > sampletime_ms)//if the sampel time == 30s
           {
              ratio2_5 = lowpulseoccupancy2_5/(sampletime_ms*10.0);  // Integer percentage 0=>100
              ratio1 = lowpulseoccupancy1/(sampletime_ms*10.0);  // Integer percentage 0=>100
    
              concentration2_5 = 1.1*pow(ratio2_5,3)-3.8*pow(ratio2_5,2)+520*ratio2_5+0.62; // using spec sheet curve
              concentration1 = 1.1*pow(ratio1,3)-3.8*pow(ratio1,2)+520*ratio1+0.62; // using spec sheet curve
          
               Serial.print("conentration2_5: ");
               Serial.println(concentration2_5);
               Serial.print("conentration1: ");
               Serial.println(concentration1);
    
               if(concentration1 > concentration2_5)
                {
                    concentration = concentration1 - concentration2_5;//get the dust densty between 1um--2.5um;\
                    Serial.print("concentration: ");
                    Serial.println(concentration);
                    PM2_5 = 0.182* concentration+3.65; //using spec sheet cuve;
      
                    PM[count]=PM2_5;
                    count++;
                    Serial.print(count);
                    Serial.print(" : ");
                    Serial.println(PM[count-1]);
            
                  if( 5 == count )
                  {         
                    count=0;    
                      for(int i=0;i<5;i++)
                      {
                            Serial.print(PM[i]);
                            Serial.print("---");
                       }
                       Serial.println();
                       
                       for(int i=1;i<5;i++)
                       for(int j=0;j<5-i;j++)
                       if(PM[j]>PM[j+1])
                         {
                              temp=PM[j+1];
                              PM[j+1]=PM[j];
                              PM[j]=temp;
                          }    
                       for(int i=0;i<5;i++)
                        {
                          Serial.print(PM[i]);
                          Serial.print("---");
                         }
                        Serial.println();
                        
                        for(int i=1;i<4;i++)
                        add+=PM[i];
                        
                        PM2_5 = add/3.0;
                        
                        Serial.print("PM2.5: ");
                        Serial.print(PM2_5);
                        Serial.println("ug/M3");
                        Serial.println("");
                        add=0;
                  }
                   
               }     
                  lowpulseoccupancy2_5 = 0;
                  lowpulseoccupancy1 = 0;
                  starttime = millis();//get the current time again;
         }
              
              Data=(unsigned int)PM2_5;
              byte info[3]={ 0xAA,0xBB,0};
              info[2]=Data;
             for(int i=0;i<3;i++)
                 Serial.write(info[i]);
         
     
              
             
}         
  
  
  /*            
 {
  int i=0;
  byte c[4];
  while(1)
    {
    if(Serial3.available()>0)
       {
          c[i]=Serial3.read();    

          if(i==0 && c[i]!=0xAA)
            i=-1;
          if(i==1 && c[i]!=0xBB)
            i=-1;

	  if(-1 == i)
	  {
	    Serial.println("receive Data Wrong!");
	    break;
	  }

          i++;
          if( i==3 )
            break;
      
        }     
    } 
      
      PM2_5=c[2];
      Serial.println(PM2_5);
  
 }
 
 */
