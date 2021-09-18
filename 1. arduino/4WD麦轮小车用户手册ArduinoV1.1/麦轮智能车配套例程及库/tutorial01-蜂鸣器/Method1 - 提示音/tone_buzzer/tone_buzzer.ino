/*
  buzzer tone
  This example code is in the public domain.
  www.yfrobot.com
*/

// buzzer connected to pin 9
int buzzerPin = 9;

void setup()
{
  /* tone(pin, frequency, duration) */
  tone(buzzerPin, 262, 300); //set buzzer pin to play 264Hz for 300ms
  delay(1000);  //wait 1s

  tone(buzzerPin, 294, 300); //set buzzer pin to play 297Hz for 300ms
  delay(1000);  //wait 1s

  tone(buzzerPin, 330, 300); //set buzzer pin to play 330Hz for 300ms
  delay(1000);  //wait 1s
}

void loop()
{
  // no need to repeat the tone.
}
