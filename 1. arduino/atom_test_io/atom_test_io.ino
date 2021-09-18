void setup() {
  Serial.begin(115200);
  pinMode(A13, 1);
  pinMode(A12, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(A12,1);
  Serial.println(digitalRead(A13));
  delay(1000);
  digitalWrite(A12,0);
  Serial.println(digitalRead(A13));
  delay(1000);
}
