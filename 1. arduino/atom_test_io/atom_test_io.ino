void setup() {
  Serial.begin(115200);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
//  digitalWrite(A12,1);
  Serial.print("6\7:");
  Serial.print(digitalRead(6));
//  delay(1000);
//  digitalWrite(A12,0);
  Serial.println(digitalRead(7));
  delay(1000);
}
