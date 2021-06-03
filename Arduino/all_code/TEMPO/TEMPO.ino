#include <ezTask.h>
#include <ESP8266WiFi.h>
#include "time.h" // 시계 라이브러리

const char* ssid     = "hdch";
const char* password = "12345678"; 
const char* host = "apis.data.go.kr";
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(10);
  Serial.println("setup() START");
 
  // We start by connecting to a WiFi network
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
ezTask task1(1000,[](){
  delay(5000);
 int i=0;
 String tmp_str;
 String wfEn10;
 String wfEn25;
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
 
  Serial.println();
  Serial.println("closing connection");
  Serial.println("PM 10 : "+wfEn10); 
  Serial.println("PM 25 : "+wfEn25);
  delay(1000);
});
ezTask task2(2000,[](){
  delay(5000);

  
 int i=0;
 int j=0;
 String tmp_str;
 String tmp_str1;
 String tmp_str2;
 String wfEn10;
 String wfEn20;
 String wfEn30;
 String wfEn40;
 String wfEn50;
 String wfEn60;
 String wfEn70;
 String wfEn80;
 String wfEn90;
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
 String url= "http://apis.data.go.kr/1360000/VilageFcstInfoService/getUltraSrtNcst?serviceKey=TfNuVVqnjKaZ9N198WGqwTjbohrzGAy48OP39UynQWR0yl7NFOYZtMgzatuNyJVGE7c0o%2FOtZh8QORMDclitzg%3D%3D&pageNo=1&numOfRows=10000&dataType=XML&base_date=20210601&base_time=0600&nx=90&ny=91";
 
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

  Serial.println();
  Serial.println("closing connection");
  Serial.println("기온 : "+wfEn30); 
  Serial.println("습도 : "+wfEn40);
  Serial.println("1시간 강수량 : "+wfEn60);
  Serial.println("풍속 : "+wfEn80);
  delay(1000);
});

void loop() {
  // put your main code here, to run repeatedly:
  task1.run();
  task2.run();
}
