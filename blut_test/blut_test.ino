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
  
        
//for(int l = 0; l < arr3index; l++){
//    Serial.print(arr3[l][0]);
//      Serial.println(arr3[l][1]);
//  }
if (ListIndex % 2 != 0){
       for(int k = 0; k < ListIndex; k++){
          if (k % 2 == 0){
              arr3[arr3index][0] = arr2[k];
              arr3[arr3index][1] = arr2[k + 1];
              arr3index++;
            }
        }
  }
//for(int k = 0; k < ListIndex; k++) Serial.println(arr2[k]);

}
