#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
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
bool del = false;
bool change = false;
int key = 1;
int t;



RTC_DS3231 rtc;
GyverTM1637 disp(CLK, DIO);
Encoder enc(35, 34, 32);
BluetoothSerial BTSerial;
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);
TaskHandle_t Task1;

bool flag = true;
void setup() {

  Serial2.begin(9600);
  Serial.begin(115200); 

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(Serial2)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.volume(30);

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
for (int i = 0; i < 4090; i++) {
 EEPROM.write(i, 0);
 }
EEPROM.commit();


  EEPROM.get(0, testVal);
  if (testVal == 1){
    Serial.println(111);
    EEPROM.get(2, alarmList);
    EEPROM.get(4, ListIndex);
  }
  
  rtc.adjust(DateTime(2023, 02, 11, 22, 41, 0));
  xTaskCreatePinnedToCore(
                    Sound,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
             delay(500); 
}



void loop(){
  DateTime now = rtc.now();
  enc.tick();   
  disp.point(flag);

  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
  
  if(BTSerial.available()){
    x = BTSerial.read();
 //
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
      
    else{
      if (del == true){
          testArr[testArrIndex] = x;
          testArrIndex++;

          if (testArrIndex == 2){
            for(int r = 1; r < ListIndex; r++){
                if (r % 2 != 0){
                  if (testArr[0] == alarmList[r] and testArr[1] == alarmList[r+1]){

                      for (int w = r; w < ListIndex; w++){
                          alarmList[w] = alarmList[w+2];
                        }
                    
                   
                    ListIndex-=2;
                    break;
                  }
              }
              
                }
                testArrIndex = 0;
                del = false;
            }
            
        }

      else if (del == false and change == false){
        Serial.println("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
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
    //1 = music off
    //0 = music on
    
  
   if (digitalRead(14) == HIGH){
     myDFPlayer.stop();
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
              istime = true;
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

  for(int r = 1; r < 49; r++){
      Serial.println(alarmList[r]);
    }
  Serial.println("ListIndex" + String(ListIndex));
  Serial.println("testArrIndex" + String(testArrIndex));
  Serial.println("del" + String(del));

  
  

  
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      myDFPlayer.play(random(1, 4));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  
}

void Sound(void * pvParameters){
  for(;;){
    for(int v = 1; v < ListIndex; v++){
        if(v % 2 != 0){
            if (now.hour() == alarmList[v] and now.minute() == alarmList[v+1] and key == 1 and now.minute() != t){
                key = 0;
                t = now.minute();  
                break;
            } 
        }
      } 
      if(key==0){
          break;
        }
  }
    myDFPlayer.play(random(1, 4));
  }
