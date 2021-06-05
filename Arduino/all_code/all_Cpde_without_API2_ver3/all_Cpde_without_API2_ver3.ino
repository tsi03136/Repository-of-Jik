//센서값 Json화
#include <ArduinoJson.h>
#include <ezTask.h>
//Wifi----------------------------------------------------------------
#include <ESP8266WiFi.h>
const char* ssid = "AndroidHotspot2383";
const char* password = "11111111";
WiFiServer server(80);

//API관련---------------------------------------------------------------
const char* host = "apis.data.go.kr";

WiFiClient client;
typedef struct{
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
}Nocktime;

String ymd;
String hm;

//api 변수 
int api_air_pol; 
int api_temp; 
int api_humi; 
int api_rain; 
int api_wsd;
String wfEn25;
String wfEn30;
String wfEn40;
String wfEn60; 
String wfEn80; 

//api,센서 비교 변수

int worst_pm2_5;
int worst_temp;
int worst_humi;
int worst_rain;
int worst_wsd;




int window_state = 0; //0 자동 1 수동

//빗물----------------------------------------------------------------
int Rain_Sensor = D7;


//미세----------------------------------------------------------------
#include <SoftwareSerial.h>
SoftwareSerial mySerial(D5, D6); //매개변수1 : TX, 매개변수2 : RX
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

//온습도----------------------------------------------------------------
#include "DHT.h"
#define DHTPIN D3
#define DHTTYPE DHT11 // 온도 센서의 종류에 따라 DHT11 이나 DHT22
DHT dht(DHTPIN, DHTTYPE);

//강우량----------------------------------------------------------------
const int sensorMin = 0;     // sensor minimum
const int sensorMax = 1024;  // sensor maximum

//센서값 관리 0: 미세먼지1.0, 1: 미세먼지2.5, 2: 미세먼지10.0, 3: 온도, 4: 습도, 5. 빗물, 6. 풍속
float ALL_Sensor_Value[7];

//JSON----------------------------------------------------------------
int count = 0;

//모터-----------------------------------------------------------------
int butten_open = D8;
int butten_close = D9;
int motorIA = D10;
int motorIB = D11;
int Limit_butten1 = D12;
int Limit_butten2 = D13;
int control_butten = D4;


//----------------------------------------SETUP()----------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);

  //미세----------------------------------------------------------------
  mySerial.begin(9600);

  //온습도----------------------------------------------------------------
  dht.begin();

  //Wifi----------------------------------------------------------------
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

  //빗물 디지털 모드 ----------------------------------------------------------------
  pinMode(Rain_Sensor, INPUT);

  //모터 핀 모드
  pinMode(motorIA, OUTPUT);
  pinMode(motorIB, OUTPUT);
  pinMode(butten_open, INPUT_PULLUP);
  pinMode(butten_close, INPUT_PULLUP);
  pinMode(Limit_butten1, INPUT_PULLUP);
  pinMode(Limit_butten2, INPUT_PULLUP);
  pinMode(control_butten, INPUT_PULLUP);
}
//api 미세먼지-----------------------------------------------------------

void call_air_pol(){
  delay(5000);
 int i=0;
 String tmp_str;
 String wfEn10;
// String wfEn25;
 String wfEntest;
  Serial.print("connecting to ");
  Serial.println(host);
 
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
 
  // Request
  // Call Address
 String url = "http://apis.data.go.kr/B552584/ArpltnInforInqireSvc/getMsrstnAcctoRltmMesureDnsty?serviceKey=TfNuVVqnjKaZ9N198WGqwTjbohrzGAy48OP39UynQWR0yl7NFOYZtMgzatuNyJVGE7c0o%2FOtZh8QORMDclitzg%3D%3D&returnType=xml&numOfRows=1&pageNo=1&stationName=%EC%A7%84%EC%B2%9C%EB%8F%99&dataTerm=DAILY&ver=1.0";
  Serial.print("Requesting URL: ");
  Serial.println(url); // call Address (api)
 
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    //Serial.print(line);
    
    i= line.indexOf("</pm10Value>");

    if(i>0){
      tmp_str="<pm10Value>";
      wfEn10 = line.substring(line.indexOf(tmp_str)+tmp_str.length(),i);
      //Serial.println(wfEn10);  
    }
     i= line.indexOf("</pm25Value>");

    if(i>0){
      tmp_str="<pm25Value>";
      wfEn25 = line.substring(line.indexOf(tmp_str)+tmp_str.length(),i);
      //Serial.println(wfEn25);  
    }  
  }
api_air_pol = wfEn25.toInt(); 

  Serial.println();
  Serial.println("closing connection");
  Serial.println("PM 10 : "+wfEn10); 
  Serial.println("PM 25 : "+wfEn25);
  delay(1000);
}

//api 날씨 --------------------------------------------------------------

void call_weather(){
  delay(5000);
  
//날씨  
 int i=0;
 int j=0;
 String tmp_str;
 String tmp_str1;
 String tmp_str2;
 String wfEn10;
 String wfEn20;
// String wfEn30;
 //String wfEn40;
 String wfEn50;
 //String wfEn60;
 String wfEn70;
// String wfEn80;
 String wfEn90;
  Serial.print("connecting to ");
  Serial.println(host);
 
  // Use WiFiClient class to create TCP connections
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
 
  // Request
  // Call Address
 String url= "http://apis.data.go.kr/1360000/VilageFcstInfoService/getUltraSrtNcst?serviceKey=TfNuVVqnjKaZ9N198WGqwTjbohrzGAy48OP39UynQWR0yl7NFOYZtMgzatuNyJVGE7c0o%2FOtZh8QORMDclitzg%3D%3D&pageNo=1&numOfRows=10000&dataType=XML&base_date=" + ymd + "&base_time=" + hm + "&nx=88&ny=89";
  Serial.print("Requesting URL: ");
  Serial.println(url); // call Address (api)
 
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  //Serial.println();
  
 
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    //Serial.print(line);
    
    i= line.indexOf("UUU");

    if(i>0){
      
      tmp_str="T1H";
      wfEn10 = line.substring(line.indexOf(tmp_str)+tmp_str.length(),i);
      //Serial.println(wfEn10); 


      j= wfEn10.indexOf("</obsrValue>");      
          if(j>0){
      tmp_str1 = "<obsrValue>";
      wfEn30 = wfEn10.substring(wfEn10.indexOf(tmp_str1)+tmp_str1.length(),j);
    }
  }
  /********************************************************************************/
  //습도
     i= line.indexOf("RN1");

    if(i>0){
      tmp_str="REH";
      wfEn20 = line.substring(line.indexOf(tmp_str)+tmp_str.length(),i);
      //Serial.println(wfEn20); 
      
      j=wfEn20.indexOf("</obsrValue>"); 
      if(j>0) {
        tmp_str1 = "<obsrValue>";
        wfEn40 = wfEn20.substring(wfEn20.indexOf(tmp_str1)+tmp_str1.length(),j);
      }
      }
  /********************************************************************************/
  //1시간 강수량
  i= line.indexOf("T1H");

    if(i>0){
      tmp_str="RN1";
      wfEn50 = line.substring(line.indexOf(tmp_str)+tmp_str.length(),i);
      //Serial.println(wfEn20); 
      
      j=wfEn50.indexOf("</obsrValue>"); 
      if(j>0) {
        tmp_str1 = "<obsrValue>";
        wfEn60 = wfEn50.substring(wfEn50.indexOf(tmp_str1)+tmp_str1.length(),j);
      }
      }

      
  /********************************************************************************/
  //풍속
  i= line.indexOf("<numOfRows>");

    if(i>0){
      tmp_str="WSD";
      wfEn70 = line.substring(line.indexOf(tmp_str)+tmp_str.length(),i);
      //Serial.println(wfEn20); 
      
      j=wfEn70.indexOf("</obsrValue>"); 
      if(j>0) {
        tmp_str1 = "<obsrValue>";
        wfEn80 = wfEn70.substring(wfEn70.indexOf(tmp_str1)+tmp_str1.length(),j);
      }
      }

  
   }

api_temp = wfEn30.toInt(); 
api_humi = wfEn40.toInt(); 
api_rain = wfEn60.toInt(); 
api_wsd = wfEn80.toInt();

  Serial.println();
  Serial.println("closing connection");
  Serial.println("기온 : "+wfEn30); 
  Serial.println("습도 : "+wfEn40);
  Serial.println("1시간 강수량 : "+wfEn60);
  Serial.println("풍속 : "+wfEn80);
  delay(1000);
}

//시간---------------------------------------------------------
bool get_nocktime(Nocktime &t){
  //--------------------------------------------
  //클라이언트와 서버와 커넥트
  if (!client.connect("google.com", 80)) {
    Serial.println("connection failed");
    return false;
  }
  client.print("HEAD / HTTP/1.1\r\n\r\n");

  //서버로부터 응답이 있을때까지 대기하겠다..
  //언제까지 기다려야하나요..?
  //timeout!
  unsigned long timeout = millis(); //생존시간
  while(1){
    if(client.available()) break;
    if(millis() - timeout > 10000) break;
  }


  //응답이 날라오는 부분!
  Serial.println("request sent");

  bool iscollected = false;
  while(client.available()){
    String data = client.readStringUntil('\n'); //readline();
    //Date: Thu, 17 Dec 2020 10:26:44 GMT
    //01234567890123456789012345678901234
    
    if(data.indexOf("Date: ") != -1){
      //검색결과 date:라는 패턴이 존재하더라~
      //조건문안에 들어온 시점에 data안에는 우리가 원하는 값이 들어있다!
      iscollected = true;
      //Serial.println(data);
      int day = data.substring(11,13).toInt(); //day
      int month = 0;
      //Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
      String mo = data.substring(14,17);
      if(mo == "Jan"){
        month = 1;
      }else if(mo == "Feb"){
        month = 2;
      }else if(mo == "Mar"){
        month = 3;
      }else if(mo == "Apr"){
        month = 4;
      }else if(mo == "May"){
        month = 5;
      }else if(mo == "Jun"){
        month = 6;
      }else if(mo == "Jul"){
        month = 7;
      }else if(mo == "Aug"){
        month = 8;
      }else if(mo == "Sep"){
        month = 9;
      }else if(mo == "Oct"){
        month = 10;
      }else if(mo == "Nov"){
        month = 11;
      }else if(mo == "Dec"){
        month = 12;
      }
      int year = data.substring(18,22).toInt(); //year
      int hour = data.substring(23,25).toInt(); //hour
      int minute = data.substring(26,28).toInt(); //minute
      int second = data.substring(29,31).toInt(); //second
     
      int dayofmonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
      //년도가 4의 배수일때마다 2월이 29일까지다!
      //year % 4 == 0
      hour += 9;
      if(hour > 24){
        //20 + 9 = 29 = 29-24 = 5
        hour -= 24;
        day++;
        int yoondal = 0;
        if(year % 4 == 0 && month == 2) yoondal = 1;
        
        if(day > dayofmonth[month-1]+yoondal){
          day -= dayofmonth[month-1]+yoondal;
          month++;
          if(month > 12){
            month -= 12;
            year++;
          }
        }
      }
      /*
      Serial.println(year);
      Serial.println(month);
      Serial.println(day);
      Serial.println(hour);
      Serial.println(minute);
      Serial.println(second);
      */
      t.year = year;
      t.month = month;
      t.day = day;
      t.hour = hour;
      t.minute = minute;
      t.second = second;
    }
  }
  return iscollected;
}

//와이파이----------------------------------------------------------------
void connect_Wifi() {
 WiFiClient client = server.available();
  if (client) {
    StaticJsonBuffer<400> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    JsonObject& response = root.createNestedObject("response");
    JsonObject& body = response.createNestedObject("body");
    JsonObject& item = body.createNestedObject("item");
    JsonObject& items = item.createNestedObject("items");
    items["Air_Pollution_1.0"] = ALL_Sensor_Value[0];
    items["Air_Pollution_2.5"] = ALL_Sensor_Value[1];
    items["Air_Pollution_10.0"] = ALL_Sensor_Value[2];
    items["Wind_Speed"] = ALL_Sensor_Value[6];
    items["Temperature"] = ALL_Sensor_Value[3];
    items["Humidity"] = ALL_Sensor_Value[4];
    items["Rain_Check"] = ALL_Sensor_Value[5];


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
}

//풍속 주파수---------------------------------------------------------------------------------
void wind_freq() {
  float a = 0;
  a = analogRead(0);

  Serial.print("A0 : ");
  Serial.println(a);
  float b = 0;
  b = a / 1023.0 * 5.0;     //아두이노는 10bit ADC므로 0~1023로 표시되므로 0~5범위로 매핑합니다.

  Serial.print("\t ,b : ");
  Serial.println(b);

  float c = b * 6;
  ALL_Sensor_Value[6] = c;
  Serial.print(c);
  Serial.println("m/s");
  //delay(100);
}

//온습도--------------------------------------------------------------------------------------
void F_dht() {
  int temp = dht.readTemperature();
  int humi = dht.readHumidity();
  ALL_Sensor_Value[3] = temp;
  ALL_Sensor_Value[4] = humi;
  Serial.println(ALL_Sensor_Value[3]);
  Serial.println(ALL_Sensor_Value[4]);
}

//미세먼지--------------------------------------------------------------------------------------
void air_pol() {
  if (mySerial.available())   {
    for (int i = 0; i < 32; i++)     {
      chrRecv = mySerial.read();
      if (chrRecv == START_2 ) {
        bytCount1 = 2;
        break;
      }
    }
    if (bytCount1 == 2)
    {
      bytCount1 = 0;
      for (int i = 0; i < 30; i++) {
        chrData[i] = mySerial.read();
      }
      if ((unsigned int) chrData[ERROR_CODE] == 0) {
        ALL_Sensor_Value[0] = GetPM_Data(chrData, PM1_0_ATMOSPHERE_H, PM1_0_ATMOSPHERE_L);
        ALL_Sensor_Value[1] = GetPM_Data(chrData, PM2_5_ATMOSPHERE_H, PM2_5_ATMOSPHERE_L);
        ALL_Sensor_Value[2] = GetPM_Data(chrData, PM10_ATMOSPHERE_H, PM10_ATMOSPHERE_L);
      }
      else {
        Serial.println("PMS7003  ERROR");
      }
    }
  }
  else {
    Serial.println("PMS7003 NOT available");
  }
}

//빗물센서--------------------------------------------------------------------------------------
void rain() {
  ALL_Sensor_Value[5] = digitalRead(Rain_Sensor);
  Serial.println(ALL_Sensor_Value[5]);
}


//----------------------------------------------LOOP()-----------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
void loop() {
  //Run Wifi-------------------------------------------------------------
  wind_freq();
  F_dht();
  //air_pol();
  rain();
  delay(1000);
  connect_Wifi();
  
  //Run Time
Nocktime nt;
  String st_hour;
  String st_minute;
  String st_year;
  String st_month;
  String st_day;
  String st_second;

  
  if(get_nocktime(nt)){
    st_year = (String)nt.year;
    //시간이 00시 일때
    if (nt.hour == 0) {
      if (nt.minute < 30) {
        nt.hour = 23;
        if(nt.day == 1) {
          if(nt.month == 1) {
            nt.month = 12;
            nt.year = nt.year - 1;
            nt.day = 31;
          }
            else if (nt.month == 5 || nt.month == 7 || nt.month == 10 || nt.month== 12){
              nt.month = nt.month -1;
              nt.day = 30;                           
            }
            else if (nt.month == 2 || nt.month == 4 || nt.month == 6 || nt.month == 8 || nt.month == 9 || nt.month == 11){
              nt.month = nt.month -1;
              nt.day = 31;                           
            }
            else if (nt.month == 3){
              nt.month = nt.month -1;
              nt.day = 28;                           
            }
          }
          else {
            nt.day = nt.day -1;
          }
        }
      }
      else { //시간이 00시가 아닐 때
      if (nt.minute<30) {
        nt.hour = nt.hour -1;
        nt.minute = 0;               
      }
      else {
        nt.minute = 0;
      }
    }
    //10시 이하일때 스트링 변환
    if(nt.hour < 10) {
      st_hour = "0"+String(nt.hour);
    }
    
    else {
      st_hour = String(nt.hour);
    }
    //10분 이하일때 스트링 변환
    if(nt.minute < 10) {
      st_minute = String(nt.minute);
      st_minute = "0"+st_minute;
    }
    
    else {
      st_minute = String(nt.minute);
    }
    //월
    if (nt.month < 10) {
      st_month = String(nt.month);
      st_month = "0"+st_month;
    }
    
    else {
      st_month = String(nt.month);
    }
    //일
    if (nt.day < 10) {
      st_day = String(nt.day);
      st_day = "0"+st_day;
    }
    
    else {
      st_day = String(nt.day);
    }
    
    ymd = st_year +st_month+st_day;
    hm = st_hour +st_minute;
    Serial.println(ymd);
    Serial.println(hm);
    }
    call_air_pol();
    call_weather();
  
  

 //api랑 센서값 비교
if(ALL_Sensor_Value[1]>api_air_pol){
  worst_pm2_5=ALL_Sensor_Value[1]; 
}
else {
  worst_pm2_5=api_air_pol;
}

if(ALL_Sensor_Value[6]>api_wsd){
  worst_wsd=ALL_Sensor_Value[6]; 
}
else {
  worst_wsd=api_wsd;
}


 //자동화 시작
  if (digitalRead(control_butten) == 0) {
    window_state = 0;
    if (worst_pm2_5 >= 35 || worst_wsd >= 14 || digitalRead(Rain_Sensor) == 0) {
      digitalWrite(motorIA, HIGH);
      digitalWrite(motorIB, LOW);
      if (digitalRead(Limit_butten1) == 0) {
        digitalWrite(motorIA, LOW);
        digitalWrite(motorIB, LOW);
      }
      else if(16>=ALL_Sensor_Value[3] || 16>= api_temp){
        digitalWrite(motorIA, HIGH);
        digitalWrite(motorIB, LOW);
      if (digitalRead(Limit_butten1) == 0) {
        digitalWrite(motorIA, LOW);
        digitalWrite(motorIB, LOW);
      }
}
//30도 이상일 떄 창문열기
      else if(30<=api_temp || 30<=ALL_Sensor_Value[3]){
        digitalWrite(motorIA, LOW);
        digitalWrite(motorIB, HIGH);
        if (digitalRead(Limit_butten2) == 0) {
          digitalWrite(motorIA, LOW);
          digitalWrite(motorIB, LOW);
        }
}

      /*
      else if (ALL_Sensor_Value[3] <= 16) {
        digitalWrite(motorIA, HIGH);
        digitalWrite(motorIB, LOW);
        if (digitalRead(Limit_butten1) == 0) {
          digitalWrite(motorIA, LOW);
          digitalWrite(motorIB, LOW);
        }
      }
      else {
        digitalWrite(motorIA, LOW);
        digitalWrite(motorIB, HIGH);
        if (digitalRead(Limit_butten2) == 0) {
          digitalWrite(motorIA, LOW);
          digitalWrite(motorIB, LOW);
        }
      } */
    }
  }

  //수동화 시작
  if (digitalRead(butten_close) == 0)
  {
    window_state = 1;
    digitalWrite(motorIA, HIGH);
    digitalWrite(motorIB, LOW);
    if (digitalRead(Limit_butten1) == 0)
    {
      digitalWrite(motorIA, LOW);
      digitalWrite(motorIB, LOW);
      delay(5000);
    }
  }
  else if (digitalRead(butten_open) == 0)
  {
    window_state = 1;
    digitalWrite(motorIA, LOW);
    digitalWrite(motorIB, HIGH);
    if (digitalRead(Limit_butten2) == 0)
    {
      digitalWrite(motorIA, LOW);
      digitalWrite(motorIB, LOW);
      delay(5000);
    }
  }
  else
  {
    digitalWrite(motorIA, LOW);
    digitalWrite(motorIB, LOW);
  }
}
