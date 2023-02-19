#include <EEPROM.h>
int bot[20] = {1, 20, 20, 55};
int botik[20];
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  EEPROM.begin(4096);
  EEPROM.put(1, bot);

  EEPROM.get(1, botik);
}

void loop() {
  // put your main code here, to run repeatedly:
    for(int i = 0; i < 5; i++){
        Serial.println(botik[i]);
      }
}
