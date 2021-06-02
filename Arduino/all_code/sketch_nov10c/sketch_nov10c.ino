int led = 13;
void setup() {
 // open the serial port:
 Serial.begin(9600);
 pinMode(led, OUTPUT);
}
void loop() {
 // check for incoming serial data:
 if (Serial.available() > 0) { // read incoming serial data:
 char inChar = Serial.read();
 if (inChar == '1')
 {
 digitalWrite(led, HIGH);
 }
 else if (inChar == '2')
 {
 digitalWrite(led, LOW);
 }
 }
}
