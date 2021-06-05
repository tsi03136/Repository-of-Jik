void run_motor(){
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
      */
      else { //모든조건충족하여 창문열기
        digitalWrite(motorIA, LOW);
        digitalWrite(motorIB, HIGH);
        if (digitalRead(Limit_butten2) == 0) {
          digitalWrite(motorIA, LOW);
          digitalWrite(motorIB, LOW);
        }
      } 
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

void setup(){
  Serial.begin(115200);
}

void loop(){
  while(Serial.available()){
    char data = Serial.read();
    Serial.print(data);
    
    //data 파싱 후 데이터를 바탕으로 모터 조작하여 창문 작동시키는 코드 작성할 것
    //에러 상황에 대처할 것
    
    run_motor();
    
  }  
}
