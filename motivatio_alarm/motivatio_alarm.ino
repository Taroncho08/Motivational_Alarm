#include <EEPROM.h>
#include <GyverTM1637.h>
#include <GyverEncoder.h>
#include <RTClib.h>
#include "BluetoothSerial.h"
#include <Wire.h>

#define CLK 4
#define DIO 5


bool istime = false;
int clickCount = 0;
int ListIndex = 0;
int hrs;
int mins;
int alarmList[50];
int testVal = 0;
uint32_t tmr;
int address = 0;



RTC_DS3231 rtc;
GyverTM1637 disp(CLK, DIO);
Encoder enc(11, 10, 9);
BluetoothSerial BTSerial;


bool flag = true;
void setup() {
  Serial.begin(9600);
   
  disp.clear();
  disp.brightness(7);
  disp.point(1);
  enc.setType(TYPE2);
  enc.setFastTimeout(50);
  BTSerial.begin("Motivational Alarm");

    if (! rtc.begin()) {
  Serial.println("Couldn't find RTC");
  while (1);
  }


  EEPROM.get(0, testVal);
  if (testVal == 1){
    EEPROM.get(1, alarmList);
    EEPROM.get(2, ListIndex);
  }
  
 
}
void loop(){
  DateTime now = rtc.now();
  enc.tick();
   
  disp.point(flag);
  

  if(BTSerial.available()){
    x = BTSerial.read();

    
    
    if (x == 'g'){
        
       
        for(int i = 0; i<ListIndex; i++){
          if(i % 2 == 0){
          BTSerial.print(String(arr2[i]) + ":" + String(arr2[i+1]));
          delay(100);
          }
          //BTSerial.print(arr[i][1]);    
    }
      }
   
    
    else{
        
        arr2[ListIndex] = x;
        ListIndex++;

      }
      
  }


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
      alarmList[ListIndex] = hrs;
      alarmList[ListIndex + 1] = mins;
      ListIndex+=2;
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

 
        
    
    for(int v = 0; v < ListIndex; v++){
      if(v % 2 == 0){
          if (now.hour() == alarmList[v] and now.minute() == alarmList[v+1]){
              Serial.println("Ve kac agera");
              istime = true;
            }
           else{
              istime = false;
            }
        }
      
    }        

if (istime == false){    
  Serial.print(now.hour(), DEC);    // Час
  Serial.print(':');
  Serial.println(now.minute(), DEC);
}


//Serial.println(testVal);
//Serial.println(ListIndex);

    
    

  
  

  
}
