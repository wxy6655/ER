void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);//打开串口，波特率9600
    pinMode(5, OUTPUT);
    pinMode(15, OUTPUT);    
}

void loop() {
  // put your main code here, to run repeatedly:
    digitalWrite(5, HIGH);
    digitalWrite(15, HIGH);
    delay(1000);
    digitalWrite(5, LOW);
    digitalWrite(15, LOW);
    delay(1000);
}