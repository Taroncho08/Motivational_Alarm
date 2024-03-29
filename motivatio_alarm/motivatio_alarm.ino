#include "Arduino.h"
#include <DFPlayerMini_Fast.h>
#include <EEPROM.h>
#include <GyverEncoder.h>
#include <RTClib.h>
#include "BluetoothSerial.h"
#include <Wire.h>
#include <GyverTM1637.h>

#define CLK 2
#define DIO 4

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
int changeArr[2];
int changeArrIndex = 0;
bool del = false;
bool change = false;
int key = 1;
int t;
bool flag = true;
int changefor = 1;



RTC_DS3231 rtc;
GyverTM1637 disp(CLK, DIO);
Encoder enc(35, 34, 32);
BluetoothSerial BTSerial;
DFPlayerMini_Fast myMP3;



void setup() {
  Serial.begin(115200); 

  #if !defined(UBRR1H)
  Serial2.begin(9600);
  myMP3.begin(Serial2, true);
  #else
    Serial1.begin(9600);
    myMP3.begin(Serial1, true);
  #endif
  
  myMP3.volume(30);

  EEPROM.begin(4000);
 
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
// for (int i = 0; i < 4090; i++) {
// EEPROM.put(i, 0);
// }
// EEPROM.commit();


  EEPROM.get(0, testVal);
  if (testVal == 1){
    Serial.println(111);
    EEPROM.get(6, alarmList);
    EEPROM.get(4, ListIndex);
    
  }
  pinMode(12, INPUT_PULLUP);
//  rtc.adjust(DateTime(2023, 3, 10, 23, 34, 30));
}



void loop(){
  DateTime now = rtc.now();
  enc.tick();   
  disp.point(flag);
  
  if(BTSerial.available()){
    x = BTSerial.read();

    if (x == 'g'){
        for(int i = 1; i<ListIndex; i++){
          if(i % 2 !=  0){
          BTSerial.print(String(alarmList[i]) + ":" + String(alarmList[i+1]));
          delay(100);
          }
        }
      }
   
    else if(x == 'd'){
      del = true;  
      }

    else if(x == 'c'){
        change = true;
      }
      
    else{
      if (del == true){
          testArr[testArrIndex] = x;
          testArrIndex++;

          if (testArrIndex == 2){
            for(int r = 1; r < ListIndex; r++){
                if (r % 2 !=  0){
                  if (testArr[0] == alarmList[r] and testArr[1] == alarmList[r+1]){

                      for (int w = r; w < ListIndex; w++){
                          alarmList[w] = alarmList[w+2];
                        }
                    
                   
                    ListIndex-=2;
                    if(ListIndex == 1){
                        testVal = 0;
                      }
                    EEPROM.put(0, testVal);
                    EEPROM.put(6, alarmList);
                    EEPROM.put(4, ListIndex);
                    EEPROM.commit();
                    
                    break;
                  }
              }
              
                }
                testArrIndex = 0;
                del = false;
            }
            
        }

      else if(change == true){
        if(testArrIndex != 2){
          testArr[testArrIndex] = x;
          testArrIndex++;
        }
        
          else if (testArrIndex == 2){
              changeArr[changeArrIndex] = x;
              changeArrIndex++;

              
              if(changeArrIndex == 2){
                  for(changefor = 1; changefor < ListIndex; changefor++){
                    if (changefor % 2 !=  0){
                      if (testArr[0] == alarmList[changefor] and testArr[1] == alarmList[changefor+1]){
                        for(int n = 1; n < ListIndex; n++){
                          if(n % 2 != 0){
                            if (changeArr[0] == alarmList[n] and changeArr[1] == alarmList[n+1]){
                              isInList = true;
                              break;
                            }
                          }
                        }

                        if (isInList == false){  
                      alarmList[changefor] = changeArr[0];            
                      alarmList[changefor+1] = changeArr[1];
                      
                      EEPROM.put(0, testVal);
                      EEPROM.put(6, alarmList);
                      EEPROM.put(4, ListIndex);
                      EEPROM.commit();

                      break;

                  }
                   else{
                      isInList = false;
                      
                    }

                      }
                    
                      
                    }
                    
                    }
                    
                  
                  testArrIndex = 0;
                  change = false;
                  changeArrIndex = 0;
                  }
                  
                }
            }
    

      else if (del == false and change == false){
        Serial.println("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
        testArr[testArrIndex] = x;
        testArrIndex++;
        if (testArrIndex == 2){
          for(int d = 1; d < ListIndex; d++){
            if(d % 2 !=  0){
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
            EEPROM.put(6, alarmList);
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
    //1 = music off
    //0 = music on
        for(int v = 1; v < ListIndex; v++){
          if(v % 2 !=  0){
              if (now.hour() == alarmList[v] and now.minute() == alarmList[v+1] and key == 1 and now.minute() != t){
                  t = now.minute();
                  key = 0;
                  
                  
              } 
          }
        } 

        if(key == 0){
            sound();
          }

  
   if (digitalRead(14) == 0 or digitalRead(12) == 0 or (digitalRead(14) == 0 and digitalRead(12))){
     myMP3.stop();
     key = 1;
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
          hrs--;
        }
      
      if(enc.isLeft()){
          hrs++;
        }
      
      if (hrs > 23)hrs = 0;
      if (hrs < 0)hrs = 23;
    }
   else if (clickCount == 2){
      
      disp.displayClock(hrs, mins);
      if (enc.isRight()){
          mins--;
        }
        
     
        
      else if(enc.isLeft()){
          mins++;
        }
     
      if (mins > 59)mins = 0;
      if (mins < 0)mins = 59;
    }
   else if (clickCount == 3){
    for(int c = 1; c < ListIndex; c++){
      if(c % 2 !=  0){
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
      EEPROM.put(6, alarmList);
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
      if(v % 2 !=  0){
          if (now.hour() == alarmList[v] and now.minute() == alarmList[v+1]){
              istime = true;
            }
        }
      
    }        



   for(int r = 1; r < ListIndex; r++){
       Serial.println(alarmList[r]);
     }
     Serial.println("chkpk");


  
  

  
}

void sound(){
    if(!myMP3.isPlaying()){
      myMP3.randomAll();
    }
  }
