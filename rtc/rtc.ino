#include <Wire.h>
#include <RTClib.h>


RTC_DS3231 rtc;
void setup() {
  Serial.begin(9600);
  if (! rtc.begin()) {
  Serial.println("Couldn't find RTC");
  while (1);
  }
//  rtc.adjust(DateTime(2022, 12, 12, 16, 11, 0));
  
}

void loop() {
  DateTime now = rtc.now();
  Serial.print(now.year(), DEC);    // Год
    Serial.print('/');
    Serial.print(now.month(), DEC);   // Месяц
    Serial.print('/');
    Serial.print(now.day(), DEC);     // День
    Serial.print(" ");
    Serial.print(now.hour(), DEC);    // Час
    Serial.print(':');
    Serial.print(now.minute(), DEC);  // Минуты
    Serial.print(':');
    Serial.print(now.second(), DEC);  // Секунды
    Serial.println();
    delay(1000);

}
