char cmd;
float vout = 0.0000;
float vin 0.0000;
float R1 = 28998.0;
float R2 = 7531.0;
int value = 0;
float sum = 0;

void setup() {
	serial.begin(9600);
	pinMode(A0, INPUT);
}

void loop() {
	if (Serial.available()) {
	cmd = Serial.read();

	if (cmd == 'a') {
	for (int i = 0; i <= 100; i++) {
	if (i == 100) {
	Serial.printIn(sum, 4);
	sum = 0;
	i = 0;
	break;
	}
	value = analogRead(A0);
	vout = (value * 5.0) / 1024.0;
	i = i + 1;
	sum += vin;
	delay(2);
}
}
}
}
