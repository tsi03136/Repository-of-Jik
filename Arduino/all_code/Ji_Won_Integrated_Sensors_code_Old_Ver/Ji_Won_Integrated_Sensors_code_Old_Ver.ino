#include <SoftwareSerial.h>
SoftwareSerial soft(12, 13);

// 모터 관련 설정
#define STEP_DELAY 3000
#define STEP_COUNT 200
#define ENABLE_PIN 6
#define DIR_PIN 5
#define STEP_PIN 4
#define LIMIT_PIN1 7
#define LIMIT_PIN2 8

int window_control = 1; // 0: 수동 , 1:자동
int window_status = 0; // 0 : 정지 , 1 : 열림 , 2 : 닫힘

int prev_window_status = 0; // 창문의 마지막 상태를 저장, 0:알수 없음, 1:열림, 2:닫힘
unsigned long last_step_micros; // 마지막으로 모터를 움직인 시간을 저장
int last_step_pin_status;
int prev_limit_1_status;
int prev_limit_2_status;

#define SENSOR_CHECK_PERIOD 2000 // 온도센서 측정 주기 2000밀리초이상 설정할것 

// 온도 센서 설정
#include "DHT.h"
#define DHTPIN 11
#define DHTTYPE DHT11 // 온도 센서의 종류에 따라 DHT11 이나 DHT22

DHT dht(DHTPIN, DHTTYPE);
int8_t humi, temp; // 현재의 습도와 온도를 저장하고 있는 변수
unsigned long dht_prev_readed_millis; // 마지막 온도 측정 시간을 저장
#define LIMIT_TEMP_MAX_VALUE 30 // 더위가 감지되었는지 판단할 기준 온도
#define LIMIT_TEMP_MIN_VALUE 10 // 추위가 감지되었는지 판단할 기준 온도


// 빗물 감지 센서
#define RAIN_SENSOR_PIN 0
#define LIMIT_RAIN_VALUE 500 // 비가오는지 구분할 센서값 , 미만이면 비 감지
int rain_value = 0;


void setup()
{
  Serial.begin(9600);
  soft.begin(9600);

  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(LIMIT_PIN1, INPUT);
  pinMode(LIMIT_PIN2, INPUT);

  digitalWrite(ENABLE_PIN, HIGH);
  digitalWrite(DIR_PIN, HIGH);
  last_step_micros = millis();
  prev_limit_1_status = digitalRead(LIMIT_PIN1);
  prev_limit_2_status = digitalRead(LIMIT_PIN2);
  window_status = 0;

  // 온도 센서 초기화
  Serial.println("DHT SENSOR INIT");
  dht.begin();
}

void loop()
{
  // 센서 읽기
  // 온도 센서 측정
  if ( ( ( millis() - dht_prev_readed_millis ) > SENSOR_CHECK_PERIOD ) && ( window_status == 0 ) ) // 창문이 움직이는 동안은 센서 측정안함
  {
    humi = (int8_t)dht.readHumidity();
    temp = (int8_t)dht.readTemperature();
    dht_prev_readed_millis = millis();
    delay(200);

    // 조도센서 값 읽기
    cds_value = analogRead(CDS_PIN);

    // 빗물 감지 센서
    rain_value = analogRead(RAIN_SENSOR_PIN);

    // 가스 감지 센서
    gas_value = analogRead(GAS_SENSOR_PIN);

    // 센서값 시리얼 모니터 출력
    Serial.println("###############################");
    Serial.println("######  SENSOR VALUE ##########");
    Serial.println("###############################");
    Serial.print("TEMPERATURE : ");
    Serial.print(temp);
    Serial.println(" DEG");

    Serial.print("HUMIDITY : ");
    Serial.print(humi);
    Serial.println(" %");

    Serial.print("RAIN : ");
    Serial.print(rain_value);

    if ( rain_value < LIMIT_RAIN_VALUE )
    {
      Serial.println("(DETECTED)");
    }
    else
    {
      Serial.println("(NOT DETECTED)");
    }

    Serial.print("GAS : ");
    Serial.print(gas_value);
    if ( gas_value > LIMIT_GAS_VALUE )
    {
      Serial.println("(DETECTED)");
    }
    else
    {
      Serial.println("(NOT DETECTED)");
    }
  }

  if ( window_control == 1 ) // 자동 제어 모드인 경우
  {
    if ( ( gas_value >= LIMIT_GAS_VALUE ) ) // 가스 값이 크고 창문이 열린 상태가 아니면
    {
      if ( prev_window_status != 1 )
      {
        window_status = 1; // 창문 열기
      }
    }
    else if ( ( rain_value < LIMIT_RAIN_VALUE ) ) // 빗물 센서 값이 작고 창문이 닫힌 상태가 아니면
    {
      if ( prev_window_status != 2 )
      {
        window_status = 2; // 창문 닫기
      }
    }
    else if ( ( cds_value > CDS_VALUE_LIMIT ) ) // 어두운 경우 창문 닫기
    {
      if ( prev_window_status != 2 )
      {
        window_status = 2; // 창문 닫기
      }
    }
    else if ( ( temp >= LIMIT_TEMP_MAX_VALUE ) ) // 온도가 높고 창문이 열린 상태가 아니면
    {
      if ( prev_window_status != 1 )
      {
        window_status = 1; // 창문 열기
      }
    }
    else if ( ( temp <= LIMIT_TEMP_MIN_VALUE ) ) // 온도가 낮고 창문이 닫힌 상태가 아니면
    {
      if ( prev_window_status != 2 )
      {
        window_status = 2; // 창문 닫기
      }
    }
    else
    {
      window_status = 0; // 윗 사항이 아닌 경우 창문 정지
    }
  }

  

  if ( window_status == 1 )
  {
    if ( ( micros() - last_step_micros ) > STEP_DELAY )
    {
      digitalWrite(ENABLE_PIN, LOW);
      digitalWrite(DIR_PIN, HIGH);
      last_step_pin_status = !last_step_pin_status;
      digitalWrite(STEP_PIN, last_step_pin_status);
      last_step_micros = micros();
      int limit_status = digitalRead(LIMIT_PIN1);
      if ( limit_status == LOW ) // LIMIT 스위치가 눌러진 경우
      {
        window_status = 0; // 대기 상태로 변경
        digitalWrite(ENABLE_PIN, HIGH);
        prev_window_status = 1; // 마지막 창문의 상태는 열림 상태
      }
    }
  }

  if ( window_status == 2 )
  {
    if ( ( micros() - last_step_micros ) > STEP_DELAY )
    {
      digitalWrite(ENABLE_PIN, LOW);
      digitalWrite(DIR_PIN, LOW);
      last_step_pin_status = !last_step_pin_status;
      digitalWrite(STEP_PIN, last_step_pin_status);
      last_step_micros = micros();
      int limit_status = digitalRead(LIMIT_PIN2);
      if ( limit_status == LOW ) // LIMIT 스위치가 눌러진 경우
      {
        window_status = 0; // 대기 상태로 변경
        digitalWrite(ENABLE_PIN, HIGH);
        prev_window_status = 2; // 마지막 창문의 상태는 닫힘 상태
      }
    }
  }
}
