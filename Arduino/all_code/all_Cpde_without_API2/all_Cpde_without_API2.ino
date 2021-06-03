//센서값 Json화
#include <ArduinoJson.h>
//함수 실행 시간 관리
#include <ezTask.h>

//Wifi
#include <ESP8266WiFi.h>
const char* ssid = "Free_Wifi_2.4G";
const char* password = "cjddl2@2";
WiFiServer server(80);

//바람
#define wind_velocity_transducer 2
int pulse;
float wind_speed;  
void rpm (){     //This is the function that the interupt calls 
  pulse++;  //This function measures the rising and falling edge of the hall effect sensors signal
} 

//빗물
int analogIn = A0; // ADC 1 or 2

//미세
#include <SoftwareSerial.h>  
SoftwareSerial mySerial(5,4); // Arudino Uno port RX, TX  
#define START_1 0x42  
#define START_2 0x4d  
#define DATA_LENGTH_H        0  
#define DATA_LENGTH_L        1  
#define PM1_0_ATMOSPHERE_H   8  
#define PM1_0_ATMOSPHERE_L   9  
#define PM2_5_ATMOSPHERE_H   10  
#define PM2_5_ATMOSPHERE_L   11  
#define PM10_ATMOSPHERE_H    12  
#define PM10_ATMOSPHERE_L    13  
#define PM2_5_PARTICLE_H   16  
#define PM2_5_PARTICLE_L   17  
#define VERSION              26  
#define ERROR_CODE           27  
#define CHECKSUM             29  
byte bytCount1 = 0;  
byte bytCount2 = 0;  
unsigned char chrRecv;  
unsigned char chrData[30];  
unsigned int GetPM_Data(unsigned char chrSrc[], byte bytHigh, byte bytLow)  {  
   return (chrSrc[bytHigh] << 8) + chrSrc[bytLow];  
}

//온습도
#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT11 // 온도 센서의 종류에 따라 DHT11 이나 DHT22
DHT dht(DHTPIN, DHTTYPE);
int8_t humi, temp; // 현재의 습도와 온도를 저장하고 있는 변수

//강우량
const int sensorMin = 0;     // sensor minimum
const int sensorMax = 1024;  // sensor maximum

//센서값 관리 0: 미세먼지1.0, 1: 미세먼지2.5, 2: 미세먼지10.0, 3: 온도, 4: 습도, 5. 빗물, 6. 풍속
float ALL_Sensor_Value[7];

//JSON


void setup() {
  Serial.begin(115200);

  //바람
  pinMode(wind_velocity_transducer, INPUT);
  attachInterrupt(digitalPinToInterrupt(wind_velocity_transducer), rpm, RISING);
  
  //미세
  mySerial.begin(9600);

  //온습도
  dht.begin();

  //Wifi
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

//와이파이
ezTask ez_connect_Wifi(2000,connect_Wifi);
void connect_Wifi(){
//센서값 관리 0: 미세먼지1.0, 1: 미세먼지2.5, 2: 미세먼지10.0, 3: 온도, 4: 습도, 5. 빗물, 6. 풍속
  /*
  Serial.print("미세먼지 1.0 : ");
  Serial.print((int)ALL_Sensor_Value[0]);
  Serial.print(", 미세먼지 2.5 : ");
  Serial.print((int)ALL_Sensor_Value[1]);
  Serial.print(", 미세먼지 10 : ");
  Serial.print((int)ALL_Sensor_Value[2]);
  Serial.print(", 온도 : ");
  Serial.print(ALL_Sensor_Value[3]);
  Serial.print(", 습도 : ");
  Serial.print(ALL_Sensor_Value[4]);
  Serial.print(", 빗물 : ");
  Serial.print(ALL_Sensor_Value[5]);
  Serial.print(", 풍속 : ");
  Serial.println(ALL_Sensor_Value[6]);
  */
  
}
//풍속
ezTask ez_wind(1000,wind);
void wind(){
  pulse = 0; 
  interrupts(); 
  delay(1000);
  noInterrupts();      //Disable interrupts
  wind_speed = ( pulse * 0.0875+0.1); 
  ALL_Sensor_Value[6] = wind_speed;
}

//온습도
ezTask ez_dht(10000,air_pol);
void F_dht(){
  temp = (int8_t)dht.readTemperature();
  humi = (int8_t)dht.readHumidity();
  ALL_Sensor_Value[3] = temp;
  ALL_Sensor_Value[4] = humi;
}

//미세먼지
ezTask ez_air_pol(10000,air_pol);
void air_pol(){
  if (mySerial.available())   {  
       for(int i = 0; i < 32; i++)     {  
           chrRecv = mySerial.read();  
           if (chrRecv == START_2 ) {   
              bytCount1 = 2;  
              break;  
            }  
       }   
      if (bytCount1 == 2)  
      {  
         bytCount1 = 0;  
         for(int i = 0; i < 30; i++){  
            chrData[i] = mySerial.read();  
         }
         if ((unsigned int) chrData[ERROR_CODE] == 0) {  
            ALL_Sensor_Value[0] = GetPM_Data(chrData, PM1_0_ATMOSPHERE_H, PM1_0_ATMOSPHERE_L);  
            ALL_Sensor_Value[1] = GetPM_Data(chrData, PM2_5_ATMOSPHERE_H, PM2_5_ATMOSPHERE_L);  
            ALL_Sensor_Value[2] = GetPM_Data(chrData, PM10_ATMOSPHERE_H, PM10_ATMOSPHERE_L);
         }  
         else{  
            Serial.println("PMS7003  ERROR");  
         }  
      }   
   }  
   else{  
      Serial.println("PMS7003 NOT available");  
   }
}

//빗물센서
ezTask ez_rain(10000,rain);
void rain(){
  //Serial.print(analogRead(analogIn)); //아날로그 0 = A2번 핀 정상작동, A1핀은 잘 모르겠음
  ALL_Sensor_Value[5] = analogRead(analogIn);
  if (analogRead(1) < 300) Serial.print("Heavy Rain");
  else if (analogRead(0) < 500) Serial.print("Moderate Rain");
  else Serial.println("No Rain");
} 

void loop() {
  /*
  ez_air_pol.run();
  ez_rain.run();
  ez_dht.run();
  
  ez_connect_Wifi.run();
  */
  
  //
  //
  //
  
  /*
  pulse = 0; 
  interrupts(); 
  delay(1000);
  noInterrupts();      //Disable interrupts
  wind_speed = ( pulse * 0.0875+0.1); 
  ALL_Sensor_Value[6] = wind_speed;
*/
  //Wifi
  WiFiClient client = server.available();
  if (client) {
    StaticJsonBuffer<400> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    JsonObject& response = root.createNestedObject("response");
    JsonObject& body = response.createNestedObject("body");
    JsonObject& item = body.createNestedObject("item");
    JsonObject& items = item.createNestedObject("items");
    air_pol();
    items["Air_Pollution_1.0"] = ALL_Sensor_Value[0];
    items["Air_Pollution_2.5"] = ALL_Sensor_Value[1];
    items["Air_Pollution_10.0"] = ALL_Sensor_Value[2];
    F_dht();
    items["Temperature"] = ALL_Sensor_Value[3];
    items["Humidity"] = ALL_Sensor_Value[4];
    rain();
    items["Rain_Check"] = ALL_Sensor_Value[5];
    ez_wind.run();
    items["Wind_Speed"] = ALL_Sensor_Value[6];
    root.printTo(Serial);
    Serial.println();
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
          client.println("<head></head><body>");
          root.printTo(client); //테스트 완료 : 문제 없음
          client.println("</body></html>");
               
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
    delay(1000);
    client.stop();
    Serial.println("Client disconnected.");
  }
  delay(1000);
}
