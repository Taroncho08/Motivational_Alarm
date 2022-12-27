#include <EEPROM.h>
#include <GyverEncoder.h>
#include <RTClib.h>
#include "BluetoothSerial.h"
#include <Wire.h>
#include <GyverTM1637.h>

#define CLK 4
#define DIO 5

char x;
bool istime = false;
int clickCount = 0;
int ListIndex = 1;
int hrs;
int mins;
int alarmList[50];
int testVal = 0;
uint32_t tmr;
int address = 0;
bool isInList = false;
int testArr[2];
int testArrIndex = 0;
bool del = false;
bool change = false;



RTC_DS3231 rtc;
GyverTM1637 disp(CLK, DIO);
Encoder enc(35, 34, 32);
BluetoothSerial BTSerial;


bool flag = true;
void setup() {

  Serial.begin(9600);  
  EEPROM.begin(4096);
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
    Serial.println(111);
    EEPROM.get(2, alarmList);
    EEPROM.get(4, ListIndex);
  }
}



void loop(){
  DateTime now = rtc.now();
  enc.tick();   
  disp.point(flag);
  
  if(BTSerial.available()){
    x = BTSerial.read();
 
    if (x == 'g'){
        for(int i = 1; i<ListIndex; i++){
          if(i % 2 != 0){
          BTSerial.print(String(alarmList[i]) + ":" + String(alarmList[i+1]));
          delay(100);
          }
        }
      }
   
    else if(x == 'd'){
      del = true;    
      }

    else if(x == 'e'){
        del = false;
        change = false;
      }
      
    else{
      if (del == true){
          testArr[testArrIndex] = x;
          testArrIndex++;
          if (testArrIndex == 2){
            for(int r = 1; r < ListIndex; r++){
                if (r % 2 != 0){
                  if (testArr[0] == alarmList[d] and testArr[1] == alarmList[d+1]){
                    for (int w = r+2; w < ListIndex; w++){
                        
                      }
                  }
                      
              }
                }
            }
        }
      if (del == false and change == false){
        testArr[testArrIndex] = x;
        testArrIndex++;
        if (testArrIndex == 2){
          for(int d = 1; d < ListIndex; d++){
            if(d % 2 != 0){
                if (testArr[0] == alarmList[d] and testArr[1] == alarmList[d+1]){
                    isInList = true;
                  }
                }
          }
          
        
          if (isInList == false){
            alarmList[ListIndex] = testArr[0];
            alarmList[ListIndex + 1] = testArr[1];
            ListIndex+=2;
            testVal = 1;
            EEPROM.put(0, testVal);
            EEPROM.put(2, alarmList);
            EEPROM.put(4, ListIndex);
            EEPROM.commit();
            
            
          }
          else{
              isInList == false;
            }
            testArrIndex = 0;
        }
      }
    }
      
  }
  


  if (enc.isClick()){
    clickCount++;
    if (clickCount > 3){
        clickCount = 0;
      }  
   }

   if (clickCount == 1){
      
      disp.displayClock(hrs, now.minute());
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
    for(int c = 1; c < ListIndex; c++){
      if(c % 2 != 0){
          if (hrs == alarmList[c] and mins == alarmList[c+1]){
            clickCount = 0;
            }
          }
    }
    if(clickCount == 3){
      alarmList[ListIndex] = hrs;
      alarmList[ListIndex + 1] = mins;
      ListIndex+=2;
      mins = 0;
      hrs - 0;
      testVal = 1; 
      EEPROM.put(0, testVal);
      EEPROM.put(2, alarmList);
      EEPROM.put(4, ListIndex);
      EEPROM.commit();

      clickCount = 0;
    }
    }
   else{
        disp.displayClock(now.hour(), now.minute());
    }
  

    
        
    if (millis() - tmr >= 500){
        tmr = millis();
        flag = !flag;
      }

 
        
    
    for(int v = 1; v < ListIndex; v++){
      if(v % 2 != 0){
          if (now.hour() == alarmList[v] and now.minute() == alarmList[v+1]){
              Serial.println("Ve kac agera");
              istime = true;
            }
           else{
              istime = false;
            }
        }
      
    }        

// if (istime == false){    
//  Serial.print(now.hour(), DEC);    // Час
//  Serial.print(':');
//  Serial.println(now.minute(), DEC);
// }


//Serial.println(testVal);
//Serial.println(ListIndex);

  for(int r = 1; r < ListIndex; r++){
      Serial.println(alarmList[r]);
    }
    Serial.println("ListIndex" + String(ListIndex));

  
  

  
}
