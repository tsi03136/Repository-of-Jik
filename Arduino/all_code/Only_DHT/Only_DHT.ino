//온습도
#include "DHT.h"
#define DHTPIN 4 //핀번호 설정
#define DHTTYPE DHT11 // 온도 센서의 종류에 따라 DHT11 이나 DHT22
DHT dht(DHTPIN, DHTTYPE);
int8_t humi, temp; // 현재의 습도와 온도를 저장하고 있는 변수

void F_dht(){
  temp = (int8_t)dht.readTemperature();
  humi = (int8_t)dht.readHumidity();
  ALL_Sensor_Value[3] = temp;
  ALL_Sensor_Value[4] = humi;
  Serial.print("temp : ");
  Serial.print(temp);
  Serial.print("humi : ");
  Serial.println(humi);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.begin();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  F_dht();
}
