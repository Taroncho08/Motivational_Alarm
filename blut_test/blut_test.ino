#include <EEPROM.h>

#include "BluetoothSerial.h"
BluetoothSerial BTSerial;

int arr[50][2] = {{1, 20}, {20, 35}};
String l;
char x;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  BTSerial.begin("ESP32test");
  EEPROM.put(1, arr);
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if(BTSerial.available()){
    x = BTSerial.read();
    l += String(x);
    
    
    if (x == '1'){
        
       
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
    
      Serial.println(l);
      
  }

}
