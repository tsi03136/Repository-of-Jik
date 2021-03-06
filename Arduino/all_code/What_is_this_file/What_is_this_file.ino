#include <ezTask.h>
#include <ESP8266WiFi.h>

const char* ssid     = "AndroidHotspot2383";
const char* password = "11111111";
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

//미세먼지----------------------------------------------------------
void call_air_pol(){
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
}


//날씨---------------------------------------------------------
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
 
  Serial.println();
  Serial.println("closing connection");
  Serial.println("기온 : "+wfEn30); 
  Serial.println("습도 : "+wfEn40);
  Serial.println("1시간 강수량 : "+wfEn60);
  Serial.println("풍속 : "+wfEn80);
  delay(1000);
}

/********************* 날씨 task ************************/
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



void loop() {
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
    delay(1000); 
}
