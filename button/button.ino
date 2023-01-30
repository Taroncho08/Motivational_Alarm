int time = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(19, INPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  time = digitalRead(19);
  Serial.println(time);
}
