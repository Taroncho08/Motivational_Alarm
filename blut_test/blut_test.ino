#include <EEPROM.h>
#include "BluetoothSerial.h"
#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;
BluetoothSerial BTSerial;

bool istime = false;
String l;

int arr2[20] = {}; 
int ListIndex = 0;
char x;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  BTSerial.begin("Motivational Alarm");

  if (! rtc.begin()) {
  Serial.println("Couldn't find RTC");
  while (1);
  }
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  DateTime now = rtc.now();
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
  
  for(int v = 0; v < ListIndex; v++){
      if(v % 2 == 0){
          if (now.hour() == arr2[v] and now.minute() == arr2[v+1]){
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


//for(int k = 0; k < ListIndex; k++) Serial.println(arr2[k]);

}
