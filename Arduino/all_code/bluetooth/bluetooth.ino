while ( soft.available() >= 2.)
  {
    int ch = soft.read();
    Serial.write(ch);
    if ( ch == 20 )
    {
      delay(50);
      if ( soft.available() )
      {
        ch = soft.read();
        Serial.print(ch);
        Serial.print(" ");

        if ( ch == '0' ) // 0 을 수신하면 창문 이동 정지
        {
          window_status = 0;
          window_control = 0; // 창문 제어 명령이 수신된 경우 수동 제어로 전환
          digitalWrite(ENABLE_PIN, HIGH);
          prev_window_status = 0; // 마지막 창문의 상태는 알수 없음
        }
        if ( ch == '1' ) // 1을 수신하면 창문 열기
        {
          window_status = 1;
          window_control = 0; // 창문 제어 명령이 수신된 경우 수동 제어로 전환
          prev_window_status = 0; // 마지막 창문의 상태는 알수 없음
        }
        if ( ch == '2' ) // 2를 수신하면 창문 닫기
        {
          window_status = 2;
          window_control = 0; // 창문 제어 명령이 수신된 경우 수동 제어로 전환
          prev_window_status = 0; // 마지막 창문의 상태는 알수 없음
        }
        if ( ch == '3' ) // 3을 수신하면 창문 제어 자동
        {
          window_control = 1;
          prev_window_status = 0; // 마지막 창문의 상태는 알수 없음
        }
        if ( ch == '4' ) // 4를 수신하면 창문 제어 수동
        {
          window_control = 0;
          prev_window_status = 0; // 마지막 창문의 상태는 알수 없음
        }
      }
    }
  }
