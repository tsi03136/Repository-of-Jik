int motorIA = D3;
int motorIB = D4;
int Limit_button1 = D5;
int Limit_button2 = D6;
int button_open = D7;
int button_close = D8;
int control_button = D9;

int worst_air_pol = 0;
int worst_rain = 0;
int worst_temp = 0;
float worst_wind = 0;

int i = 1;
int j = 0;
int k = 0;
int l = 0;
String air_pol_VF;
String Temp_VF;
String Rain_VF;
String Wind_VF;

int window_state = 0; // 0:자동 1:수동

void ChangeType(){
  worst_air_pol = air_pol_VF.toInt();  ;
  worst_rain = Rain_VF.toInt();
  worst_temp = Temp_VF.toInt();
  worst_wind = Wind_VF.toFloat();
}

//자동화
void auto_control() {
  if(digitalRead(control_button) == 0) {
    window_state = 0; 
  }
    
    //조건1 : 미세먼지, 바람, 비 하나라도 이상이 감지되면 창문을 닫기
  if(window_state == 0) {
  if (worst_air_pol == 1 || worst_wind == 1 || worst_rain == 1) {
    digitalWrite(motorIA, HIGH);
    digitalWrite(motorIB, LOW);
    if (digitalRead(Limit_button2) == 0) {
      digitalWrite(motorIA, LOW);
      digitalWrite(motorIB, LOW);
    }
    delay(500);
  }
    //조건2 : 미세먼지, 바람, 비의 상태가 좋지만 온도가 낮을 떄 창문을 닫기
    //worst_temp = 1 추울 때만
    else if(worst_temp == 1){
      digitalWrite(motorIA, HIGH);
      digitalWrite(motorIB, LOW);
      if (digitalRead(Limit_button2) == 0) {
        digitalWrite(motorIA, LOW);
        digitalWrite(motorIB, LOW);
      }
      delay(500);
    }
    else { //조건3 : 조건1과 조건2를 만족하면(미세먼지,비,바람 상태 모두좋고, 온도가 낮지않으면) 창문열기
      digitalWrite(motorIA, LOW);
      digitalWrite(motorIB, HIGH);
      if (digitalRead(Limit_button1) == 0) {
        digitalWrite(motorIA, LOW);
        digitalWrite(motorIB, LOW);
      }
     delay(500);
    } 
  }
}

void control(){
  //수동화 시작
    if (digitalRead(button_close) == 0)
    {
      window_state = 1;
      digitalWrite(motorIA, HIGH);
      digitalWrite(motorIB, LOW);
      if (digitalRead(Limit_button2) == 0)
      {
        digitalWrite(motorIA, LOW);
        digitalWrite(motorIB, LOW);
        delay(1000);
      }
    }
    else if (digitalRead(button_open) == 0)
    {
      window_state = 1;
      digitalWrite(motorIA, LOW);
      digitalWrite(motorIB, HIGH);
      if (digitalRead(Limit_button1) == 0)
      {
        digitalWrite(motorIA, LOW);
        digitalWrite(motorIB, LOW);
        delay(1000);
      }
    }
    else{
      digitalWrite(motorIA, LOW);
      digitalWrite(motorIB, LOW);
    }
  delay(100);
}

void parsedata(){
  while(Serial.available()){
    String inString = Serial.readStringUntil('\n');
    
    Serial.println(inString);
    
    //전체 문자열 파싱 함수
    String str = "{"; //문자열 시작
    j = inString.indexOf("}"); //문자열 끝
    String AllString = inString.substring(inString.indexOf('{'),j+1); //json만
    if(AllString.substring(0,1) ==(String)'{'){
      //미세먼지 파싱
      String str2 = "WorstairPol";
      k = inString.indexOf("WorstTemp");
      String air_pol_V1 = AllString.substring(AllString.indexOf(str2)+str2.length(),k);  
      String str3 = ":";
      l = air_pol_V1.indexOf(",");
      air_pol_VF = air_pol_V1.substring(air_pol_V1.indexOf(str3)+str3.length(),l);

    
    //온도 파싱
    if (i>0) {
      String str2 = "WorstTemp";
      k = inString.indexOf("WorstRain");
      String Temp_V1 = AllString.substring(AllString.indexOf(str2)+str2.length(),k);
      if (i>0) {
        String str3 = ":";
        l = Temp_V1.indexOf(",");
        Temp_VF = Temp_V1.substring(Temp_V1.indexOf(str3)+str3.length(),l);
      }
    }
    
    //비 파싱
    if (i>0) {
      String str2 = "WorstRain";
      k = inString.indexOf("WorstWind");
      String Rain_V1 = AllString.substring(AllString.indexOf(str2)+str2.length(),k);
      if (i>0) {
        String str3 = ":";
        l = Rain_V1.indexOf(",");
        Rain_VF = Rain_V1.substring(Rain_V1.indexOf(str3)+str3.length(),l);
      }
    }

    //바람 파싱
    if (i>0) {
      String str2 = "WorstWind";
      String Wind_V1 = AllString.substring(AllString.indexOf(str2)+str2.length(),j);
      if (i>0) {
        String str3 = ":";
        Wind_VF = Wind_V1.substring(Wind_V1.indexOf(str3)+str3.length(),j);
      }
    }
    Serial.println(air_pol_VF);
    Serial.println(Temp_VF);
    Serial.println(Rain_VF);
    Serial.println(Wind_VF);
    }
    ChangeType();
    break;
    //data 파싱 후 데이터를 바탕으로 모터 조작하여 창문 작동시키는 코드 작성할 것
    //에러 상황에 대처할 것
    //미세먼지 크기 10pm으로 수정
  }
}

void setup(){
  Serial.begin(115200);
  pinMode(motorIA, OUTPUT);
  pinMode(motorIB, OUTPUT);
  pinMode(button_open, INPUT_PULLUP);
  pinMode(button_close, INPUT_PULLUP);
  pinMode(Limit_button1, INPUT_PULLUP);
  pinMode(Limit_button2, INPUT_PULLUP);
  pinMode(control_button, INPUT_PULLUP);
  //pinMode(LED, OUTPUT);
  
}

void loop(){
  parsedata();
  control();
  auto_control();
  Serial.println(window_state);
}
