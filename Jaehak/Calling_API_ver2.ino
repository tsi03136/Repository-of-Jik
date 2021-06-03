#include <ezTask.h>
#include <ESP8266WiFi.h>

const char* ssid     = "KT_GiGA_2G_Wave2_EEF9";
const char* password = "ge4ahg2950"; 
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
    Serial.print(line);
    
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
  delay(10000);
});

ezTask task2(2000,[](){
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
 String url= "http://apis.data.go.kr/1360000/VilageFcstInfoService/getUltraSrtFcst?serviceKey=TfNuVVqnjKaZ9N198WGqwTjbohrzGAy48OP39UynQWR0yl7NFOYZtMgzatuNyJVGE7c0o%2FOtZh8QORMDclitzg%3D%3D&pageNo=1&numOfRows=10&dataType=XML&base_date=20210530&base_time=0630&nx=90&ny=91";
 
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
    Serial.print(line);
    
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
  Serial.println("PM 30 : "+wfEn10); 
  Serial.println("PM 40 : "+wfEn25);
  delay(10000);
});


void loop() {
  // put your main code here, to run repeatedly:
  task1.run();
  task2.run();

}
