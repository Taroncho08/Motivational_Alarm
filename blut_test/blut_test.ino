#include <EEPROM.h>
#include "BluetoothSerial.h"

BluetoothSerial BTSerial;

int arr[50][2] = {{1, 20}, {20, 35}};
String l;

int arr3[20][2] = {};
int arr3index = 0;
int arr2[20] = {}; 
int ListIndex = 0;
char x;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  BTSerial.begin("Motivational Alarm");
  EEPROM.put(1, arr);
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if(BTSerial.available()){
    x = BTSerial.read();
    l += String(x);
    
    
    if (x == 'g'){
        
       
        for(int i = 0; i<2; i++){
          if(arr[i][0] != -1 and arr[i][1] != -1){
          BTSerial.print(String(arr[i][0]) + ":" + String(arr[i][1]));
          delay(1000);
          }
          //BTSerial.print(arr[i][1]);    
    }
      }
    else if(x == 'c'){
        for (int i = 0; i < EEPROM.length(); i++) EEPROM.write(i, 255);
        EEPROM.get(1, arr);

      }
    
    else{
        
        arr2[ListIndex] = x;
        ListIndex++;

      }
      
  }
  
  for(int v = 0; v < ListIndex; v++){
      if(v % 2 == 0){
          if 
        }
    }        


  for(int m = 0; m < arr3index; m++){
    Serial.print(arr3[m][0]);
      Serial.println(arr3[m][1]);
  }
//for(int k = 0; k < ListIndex; k++) Serial.println(arr2[k]);

}
