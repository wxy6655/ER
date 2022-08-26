
int A_KEY = 21;
int B_KEY = 22;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(A_KEY, INPUT);
  pinMode(B_KEY, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(digitalRead(A_KEY),digitalRead(B_KEY));
}
