#define BUTTON_PIN 14 // GIOP21 pin connected to button

// Variables will change:


void setup() {
  Serial.begin(9600);
  // initialize the pushbutton pin as an pull-up input
  //pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
if (digitalRead(14) == 0){
     Serial.println("aaaaaaaaaa");
   }
}