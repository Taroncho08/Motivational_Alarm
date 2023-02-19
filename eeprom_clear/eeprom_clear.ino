#include <EEPROM.h>


void setup() {
  // put your setup code here, to run once:
for (int i = 0; i < 4090; i++) {
 EEPROM.put(i, 0);
 }
EEPROM.commit();
}

void loop() {
  // put your main code here, to run repeatedly:

}
