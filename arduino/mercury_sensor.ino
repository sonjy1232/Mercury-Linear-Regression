char cmd;
float vout = 0.0000;
float vin = 0.0000;       // = 대입 연산자 추가
float R1 = 28998.0;      // 전압 분배 저항 1
float R2 = 7531.0;       // 전압 분배 저항 2
int value = 0;
float sum = 0.0;

void setup() {
  Serial.begin(9600);    // s 소문자를 대문자 Serial로 수정
  pinMode(A0, INPUT);
}

void loop() {
  if (Serial.available()) {
    cmd = Serial.read();

    if (cmd == 'a') {
      sum = 0.0; // 측정 전 누적값 초기화
      
      // 100회 동안 전압 데이터 수집 및 누적
      for (int i = 0; i < 100; i++) {
        value = analogRead(A0);
        vout = (value * 5.0) / 1024.0;
        
        // 전압 분배 수식을 이용하여 실제 입력 전압 vin 계산
        vin = vout * (R1 + R2) / R2; 
        
        sum += vin;
        delay(2);
      }
      
      // 100회 측정한 전압의 평균값 계산 후 파이썬으로 전송 (소수점 4자리)
      float avg_vin = sum / 100.0;
      Serial.println(avg_vin, 4); // println (소문자 l) 사용
    }
  }
}
