//숫자로 할 시에 GPIO 기준으로 작동

#define ledPin0 0 // 작동 함
#define ledPin1 1 //작동 함
#define ledPin2 2 //작동 함
#define ledPin3 3 //작동 안함
#define ledPin4 4 //작동 함
#define ledPin5 5 //작동 함
//#define ledPin 6
//#define ledPin 7
//#define ledPin 8
//#define ledPin 9
//#define ledPin 10
//#define ledPin 11
#define ledPin12 12 //작동 함
#define ledPin13 13 //작동 함
#define ledPin14 14 //작동 함
#define ledPin15 15 //작동 함
#define ledPin16 16 //작동 함
void setup(){
    Serial.begin(115200);
    
    pinMode(ledPin0, OUTPUT);
    pinMode(ledPin1, OUTPUT);
    pinMode(ledPin2, OUTPUT);
    pinMode(ledPin3, OUTPUT);
    pinMode(ledPin4, OUTPUT);
    pinMode(ledPin5, OUTPUT);
    pinMode(ledPin12, OUTPUT);
    pinMode(ledPin13, OUTPUT);
    pinMode(ledPin14, OUTPUT);
    pinMode(ledPin15, OUTPUT);
    pinMode(ledPin16, OUTPUT);
}
void loop(){
    Serial.println("LED WELL");
    
    digitalWrite(ledPin0, HIGH);    // set the LED on
    digitalWrite(ledPin1, HIGH);    // set the LED on
    digitalWrite(ledPin2, HIGH);    // set the LED on
    digitalWrite(ledPin3, HIGH);    // set the LED on
    digitalWrite(ledPin4, HIGH);    // set the LED on
    digitalWrite(ledPin5, HIGH);    // set the LED on
    digitalWrite(ledPin12, HIGH);    // set the LED on
    digitalWrite(ledPin13, HIGH);    // set the LED on
    digitalWrite(ledPin14, HIGH);    // set the LED on
    digitalWrite(ledPin15, HIGH);    // set the LED on
    digitalWrite(ledPin16, HIGH);    // set the LED on
   
    delay(1000); // waits for a second
    
    digitalWrite(ledPin0, LOW);    // set the LED on
    digitalWrite(ledPin1, LOW);    // set the LED on
    digitalWrite(ledPin2, LOW);    // set the LED on
    digitalWrite(ledPin3, LOW);    // set the LED on
    digitalWrite(ledPin4, LOW);    // set the LED on
    digitalWrite(ledPin5, LOW);    // set the LED on
    digitalWrite(ledPin12, LOW);    // set the LED on
    digitalWrite(ledPin13, LOW);    // set the LED on
    digitalWrite(ledPin14, LOW);    // set the LED on
    digitalWrite(ledPin15, LOW);    // set the LED on
    digitalWrite(ledPin16, LOW);    // set the LED on
    
    delay(1000); // waits for a second
}
