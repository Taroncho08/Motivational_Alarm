#include <EEPROM.h>
#include <GyverTM1637.h>
#include <GyverEncoder.h>
#include <iarduino_RTC.h>

#define CLK 4
#define DIO 5


int clickCount = 0;
int ListIndex = 0;
int hrs;
int mins;
int alarmList[50][2];
int testVal = 0;
uint32_t tmr;
int address = 0;




iarduino_RTC time(RTC_DS1302, 8,6,7);
GyverTM1637 disp(CLK, DIO);
Encoder enc(11, 10, 9);


bool flag = true;
void setup() {
  Serial.begin(9600);
   
  disp.clear();
  disp.brightness(7);
  time.begin();
  disp.point(1);
  enc.setType(TYPE2);
  enc.setFastTimeout(50);
   

  EEPROM.get(0, testVal);
  if (testVal == 1){
    EEPROM.get(1, alarmList);
    EEPROM.get(2, ListIndex);
  }
  
 
}
void loop(){
  enc.tick();
   
  disp.point(flag);
  time.gettime("d-m-Y, H:i:s, D");


  if (enc.isClick()){
    clickCount++;
    if (clickCount > 3){
        clickCount = 0;
      }  
   }

   if (clickCount == 1){
      
      disp.displayClock(hrs, time.minutes);
      if (enc.isRight()){
          hrs++;
        }
      
      if(enc.isLeft()){
          hrs--;
        }
      
      if (hrs > 23)hrs = 0;
      if (hrs < 0)hrs = 23;
    }
   else if (clickCount == 2){
      
      disp.displayClock(hrs, mins);
      if (enc.isRight()){
          mins++;
        }
        
     
        
      else if(enc.isLeft()){
          mins--;
        }
     
      if (mins > 59)mins = 0;
      if (mins < 0)mins = 59;
    }
   else if (clickCount == 3){
      alarmList[ListIndex][0] = hrs;
      alarmList[ListIndex][1] = mins;
      ListIndex++;
      mins = 0;
      hrs - 0;
      testVal = 1;
      EEPROM.put(0, testVal);
      EEPROM.put(1, alarmList);
      EEPROM.put(2, ListIndex);
      clickCount = 0;
    }
   else{
        disp.displayClock(time.Hours, time.minutes);
        
    }
  

    
        
    if (millis() - tmr >= 500){
        tmr = millis();
        flag = !flag;
      }

 
        
    for(int i = 0; i<ListIndex; i++){
      Serial.print(alarmList[i][0]);
      Serial.println(alarmList[i][1]);
        if (time.Hours == alarmList[i][0] and time.minutes == alarmList[i][1]){
            Serial.println("Be kac");
          }
         
      }
//
//Serial.println(testVal);
Serial.println(ListIndex);

    
    

  
  

  
}
