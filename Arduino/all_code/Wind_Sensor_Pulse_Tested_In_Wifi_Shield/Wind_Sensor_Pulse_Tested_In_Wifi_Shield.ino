//참고 사이트 : https://arduino-esp8266.readthedocs.io/en/latest/reference.html#digital-io
//참고 사이트 : https://blog.daum.net/rockjjy99/2801
//업로드가 안될 때 조치 사항
//1. 아두이노 전원 케이블을 뽑았다가 끼운다
//2. ON DIP(빨간색 플라스틱)에 똑딱이 4개 중 3, 4를 올려서 업로드(FLASH) 모드로 전환한다.
//4. 아두이노 우노에 스케치 초기화가 되어 있는지 확인 (포스팅 상단 아두이노 우노 스케치 초기화 방법 참조)
//5. 배선 TX, RX 가 이상 없는지 확인 (포스팅 상단 아두이노 장착시 배선 참조)

#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
//#include <ESP8266WebServer.h>
//#include <ESP8266mDNS.h>

const char* ssid = "Free_Wifi_2.4G";
const char* password = "cjddl2@2";
WiFiServer server(80);

int pulse; //measuring the rising edges of the signal
float wind_speed;                               

//#define air_velocity_transducer 15 //GPIO 15 작동 안됨
//#define air_velocity_transducer 2 //GPIO 2 작동 잘 됨
//#define air_velocity_transducer 0 //GPIO 0 작동 안됨
#define air_velocity_transducer4 4 // 작동 잘됨
#define air_velocity_transducer5 5 // 작동 잘됨
#define air_velocity_transducer3 3 // 작동 안됨
#define air_velocity_transducer1 1 // 작동 안됨
#define air_velocity_transducer13 13 //작동 잘됨
#define air_velocity_transducer12 12 //작동 잘됨
#define air_velocity_transducer14 14 //작동 잘됨
#define air_velocity_transducer16 16 //에러 발생, 작동 중지, 열 나는 원인인듯

void rpm (){     //This is the function that the interupt calls 
  pulse++;  //This function measures the rising and falling edge of the hall effect sensors signal
} 




// The setup() method runs once, when the sketch starts
void setup() //
{ 
  pinMode(air_velocity_transducer4, INPUT); //initializes digital pin 2 as an input
  pinMode(air_velocity_transducer5, INPUT); //initializes digital pin 2 as an input
  pinMode(air_velocity_transducer3, INPUT); //initializes digital pin 2 as an input
  pinMode(air_velocity_transducer1, INPUT); //initializes digital pin 2 as an input
  pinMode(air_velocity_transducer13, INPUT); //initializes digital pin 2 as an input
  pinMode(air_velocity_transducer12, INPUT); //initializes digital pin 2 as an input
  pinMode(air_velocity_transducer14, INPUT); //initializes digital pin 2 as an input
  pinMode(air_velocity_transducer16, INPUT); //initializes digital pin 2 as an input
    
  //Serial.begin(115200); //This is the setup function where the serial port is initialised,
  attachInterrupt(digitalPinToInterrupt(air_velocity_transducer4), rpm, RISING); //and the interrupt is attached
  attachInterrupt(digitalPinToInterrupt(air_velocity_transducer5), rpm, RISING);
  attachInterrupt(digitalPinToInterrupt(air_velocity_transducer3), rpm, RISING);
  attachInterrupt(digitalPinToInterrupt(air_velocity_transducer1), rpm, RISING);
  attachInterrupt(digitalPinToInterrupt(air_velocity_transducer13), rpm, RISING);
  attachInterrupt(digitalPinToInterrupt(air_velocity_transducer12), rpm, RISING);
  attachInterrupt(digitalPinToInterrupt(air_velocity_transducer14), rpm, RISING);
  attachInterrupt(digitalPinToInterrupt(air_velocity_transducer16), rpm, RISING);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  

  
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  server.begin();
    // Connecting to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  Serial.println("");
  Serial.println("WiFi connected");
  
  Serial.println(ssid);
  Serial.print("IP address: ");
  // Printing the ESP IP address
  Serial.println(WiFi.localIP());
} 

// the loop() method runs over and over again,
// as long as the Arduino has power
void loop (){
  

  pulse = 0;   
  interrupts();      //Enables interrupts
  delay(1000);
  noInterrupts();      //Disable interrupts
  wind_speed = ( pulse * 0.0875+0.1); 
  Serial.print (wind_speed, 1); //Prints the number calculated above
  Serial.print (" m/s\n"); //Prints "m/s" and returns a  new line
          
  if (client) {
    Serial.println("New client");
    // bolean to locate when the http request ends
    boolean blank_line = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
          if (c == '\n' && blank_line) {
          
          
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
  
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head><title>Wind Speed!</title></head><body><h1>ESP8266 - Wind Speed</h1><h3>Wind Speed : ");
          client.println(wind_speed);
          client.println("</h3></body></html>");     
          break;
        }
        if (c == '\n') {
          // when starts reading a new line
          blank_line = true;
        }
        else if (c != '\r') {
          // when finds a character on the current line
          blank_line = false;
        }
      }
    }
    delay(100);
    client.stop();
    Serial.println("Client disconnected.");
  }
}
