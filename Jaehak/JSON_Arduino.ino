#include <SoftwareSerial.h>

//B code
#include <ESP8266WiFi.h>
//ArduinoJson ver.5 (Not compatible 6)
#include <ArduinoJson.h>
// wifi code & another (don't test code)
//#include <WiFiClient.h>
//#include <ESP8266WebServer.h>
//#include <ESP8266mDNS.h>
//#include "DHT.h"

//air_pol
//SoftwareSerial mySerial(D12,D13); // Arudino Uno port TX, RX
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
//Rain
//#define RAIN_SENSOR_PIN A0
#define LIMIT_RAIN_VALUE 500 // 비가오는지 구분할 센서값 , 미만이면 비 감지

//Wifi Infomation
const char* ssid = "KT_GiGA_2G_Wave2_EEF9";
const char* password = "ge4ahg2950";
WiFiServer server(80);

byte bytCount1 = 0;  
byte bytCount2 = 0;  
unsigned char chrRecv;  
unsigned char chrData[30];  
int PM01;  
int PM25;  
int PM10;  

//DHT
//#define DHTPIN D11     // DHT11이 연결된 핀
//#define DHTTYPE DHT11   // DHT 11, DHT시리즈중 11을 선택합니다.
//DHT dht(DHTPIN, DHTTYPE);


int rain_value = 0;



unsigned int GetPM_Data(unsigned char chrSrc[], byte bytHigh, byte bytLow)  
{  
  return ((chrSrc[bytHigh] << 8) + chrSrc[bytLow]);  
} 

void setup() {
  pinMode(2,INPUT);
  // for debuging 
  Serial.begin(115200);  
  // Use software serial
  // mySerial.begin(9600);
  // dht.begin();

  //Wifi connected
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

void loop() {
  
/*rain function
  rain_value = analogRead(RAIN_SENSOR_PIN);
  rain_value = rain_value;
  Serial.print("RAIN : ");
  Serial.print(rain_value);
  if ( rain_value < LIMIT_RAIN_VALUE ){
    Serial.println("(DETECTED)");
  }
  else{
    Serial.println("(NOT DETECTED)");
 */


//WIFI Loop
   WiFiClient client = server.available();
   rain_value = analogRead(A0);
  
  if (client) {
    Serial.println("New client");
    // bolean to locate when the http request ends
    boolean blank_line = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
          if (c == '\n' && blank_line) {
       //   pulse = 0; 
          rain_value = rain_value-1024;
          interrupts();      //Enables interrupts
          delay(1000);
          noInterrupts();      //Disable interrupts
    //    wind_speed = ( pulse * 0.0875+0.1); 
         
//        Serial.print (wind_speed, 1); //Prints the number calculated above
//        Serial.print (" m/s\n"); //Prints "m/s" and returns a  new line


//JSON Transport
String jsondata="";
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["rain"]=abs(rain_value);
  root["Humidity"]="h";
  root["Temperature"]="t";
  root["PM01"]="pm01";
  root["PM10"]="pm10";
  root["PM25"]="pm25";
  root["WSD"]="WSD";
  root.printTo(jsondata);
  Serial.println(jsondata);


// Webserver Screen
  client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println("Refresh: 1"); //refresh : 1sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head><title>Arduino Sensor Data</title></head><body><h1>Arduino Sensor Data</h1>");
          client.println("{&quot;respone&quot;:");
          // client.println(jsondata.length());
          client.println();
          client.println(jsondata);
          client.println("}");
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
          
//  F_Air_Pollution();
//  F_DHT();
//  F_Rain();
  delay(1000);
}

/*
void F_Air_Pollution(){
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

         if ( (unsigned int) chrData[ERROR_CODE] == 0 ) {
            PM01  = GetPM_Data(chrData, PM1_0_ATMOSPHERE_H, PM1_0_ATMOSPHERE_L);  
            PM25  = GetPM_Data(chrData, PM2_5_ATMOSPHERE_H, PM2_5_ATMOSPHERE_L);  
            PM10  = GetPM_Data(chrData, PM10_ATMOSPHERE_H, PM10_ATMOSPHERE_L);  
            Serial.print("PM1.0= ");  
            Serial.print(PM01);  
            Serial.print("\t,PM2.5=");  
            Serial.print(PM25);  
            Serial.print("\t,PM10=");  
            Serial.println(PM10);  
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

*/

/*void F_DHT(){
  float h = dht.readHumidity();// 습도를 측정합니다.
  float t = dht.readTemperature();// 온도를 측정합니다.

  // 값 읽기에 오류가 있으면 오류를 출력합니다.
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
*/

/*
  // 보정된 섭씨 값을 가져옵니다.
  int hic = dht.computeHeatIndex(t, h, false);

  Serial.print("습도 : ");
  Serial.print(h);
  Serial.print("%\t");
  Serial.print("온도 : ");
  Serial.print(t);
  Serial.print("℃\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.println("℃");
}


*/
/*
void F_Rain(){
  rain_value = analogRead(RAIN_SENSOR_PIN);
  rain_value = rain_value - 1024;
  rain_value = abs(rain_value);
  Serial.print("RAIN : ");
  Serial.print(rain_value);
  if ( rain_value < LIMIT_RAIN_VALUE ){
    Serial.println("(DETECTED)");
  }
  else{
    Serial.println("(NOT DETECTED)");
  }
}
*/
