void setup(){
  pinMode(21,OUTPUT);
  pinMode(22,OUTPUT);
}

void loop(){
  digitalWrite(21,HIGH);
  digitalWrite(22,HIGH);
  delay(5000);
  digitalWrite(21,LOW);
  digitalWrite(22,LOW);
  delay(5000);
}
