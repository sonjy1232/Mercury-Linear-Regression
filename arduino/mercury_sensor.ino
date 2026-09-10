char cmd;
float vout = 0.0000;
float vin = 0.0000;
float R1 = 28998.0;     // 전압 분배 저항 1
float R2 = 7531.0;      // 전압 분배 저항 2
int value = 0;
float sum = 0.0;

// 오름차순 정렬 함수 (버블 정렬)
void sortArray(float arr[], int size) {
  for (int i = 0; i < size - 1; i++) {
    for (int j = 0; j < size - i - 1; j++) {
      if (arr[j] > arr[j + 1]) {
        float temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
}

void loop() {
  if (Serial.available()) {
    cmd = Serial.read();

    if (cmd == 'a') {
      float readings[100]; // 100개의 측정값을 저장할 배열
      sum = 0.0;

      // 100회 동안 전압 데이터 수집
      for (int i = 0; i < 100; i++) {
        value = analogRead(A0);
        vout = (value * 5.0) / 1024.0;
        vin = vout * (R1 + R2) / R2;
        
        readings[i] = vin;
        delay(2);
      }

      // 100개의 데이터 오름차순 정렬
      sortArray(readings, 100);

      // 최저값 5개(인덱스 0~4)와 최고값 5개(인덱스 95~99)를 제외한 90개 누적
      for (int i = 5; i < 95; i++) {
        sum += readings[i];
      }

      // 90개 데이터의 평균 계산 후 출력 (소수점 4자리)
      float avg_vin = sum / 90.0;
      Serial.println(avg_vin, 4);
    }
  }
}
