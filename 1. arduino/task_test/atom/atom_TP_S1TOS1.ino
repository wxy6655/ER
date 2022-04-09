
void setup () {
  Serial.begin(1000000);
  Serial.println("Serial open");
}


void loop () {
    while (Serial.available())
        {
            Serial.write(Serial.read());
        }
}
