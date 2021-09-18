#include <MycobotBasic.h>
#include <M5Stack.h>

MycobotBasic myCobot;

void setup()
{
  // Initialize the individual instances.
  myCobot.setup();
  //Serial.begin(9600);
  M5.begin();

  // Initialize all status.
  myCobot.powerOn();
  delay(50);
  myCobot.setLEDRGB(255, 215, 0); // gold
  delay(50);
  // Initialize terminal input / output mode.
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  pumpOff();
  displayToScreen(0);
}

void loop()
{
  M5.update();

  // put your main code here, to run repeatedly
  if (M5.BtnA.wasPressed())
  {
    palletizing();
    displayToScreen(0);
  }
  if (M5.BtnB.wasPressed())
  {
    myCobot.writeAngles({0, 0, 0, 0, 0, 0}, 80);
    displayToScreen(0);
  }
  if (M5.BtnC.wasPressed())
  {
    myCobot.setFreeMove();
    displayToScreen(0);
  }
}

void palletizing()
{
  displayToScreen(1);
  Angles mid = {2.02, -44.47, -17.75, -148.44, 119.88, -7.91};
  Angles mid2 = {1.84, -55.45, -16.61, -142.11, 118.03, -9.58};
  Angles mid3 = {2.02, -44.47, -7.75, -148.44, 119.88, -7.91};
  myCobot.writeAngles({2.63, -22.23, -17.57, -132.71, 111.09, -7.73}, 37);
  delay(5000);
  displayToScreen(2);
  // Coords sourceArr[] = {
  //     Coords{80, -160, 120, 90, -10, -40},
  //     Coords{85, -160, 80, 90, -10, -40},
  //     Coords{20, -160, 120, 90, -10, -30},
  //     Coords{20, -160, 80, 90, -10, -30}};
  // Coords deviationArr[] = {
  //     Coords{80, -160, 105, 90, -10, -40},
  //     Coords{85, -160, 65, 90, -10, -40},
  //     Coords{20, -160, 105, 90, -10, -30},
  //     Coords{20, -160, 65, 90, -10, -30}};
  // Coords targetArr[] = {
  //     Coords{65, 195, 110, 80, -20, 150},
  //     Coords{125, 195, 110, 80, -10, 150},
  //     Coords{60, 175, 110, 100, -5, 40},
  //     Coords{107, 157, 110, 93, -11, 60}};

  Angles sourceArr[] = {
      Angles{-87.89, -35.15, -49.65, -89.82, 111.09, -16.17},
      Angles{-68.55, -29.97, -58.53, -80.95, 123.13, -16.78},
  };
  Angles targetArr[] = {
      Angles{57.56, -78.92, 1.23, -65.3, 81.12, 17.4},
      Angles{69.24, -70.75, -7.03, -90.57, 83.4, -10.73},
  };

  int i = 0;
  while (i < 2)
  {
    clearAndPrint("Go to cargo location");
    myCobot.writeAngles(sourceArr[i], 35);
    delay(5000);
    // down
    down_pos(4);
    delay(1000);
    pumpOn();
    delay(500);
    // up
    up_pos(5);
    delay(4000);

    myCobot.writeAngles({2.63, -22.23, -17.57, -132.71, 111.09, -7.73}, 37);
    delay(1500);
    myCobot.writeAngles(mid2, 15);
    delay(3000);
    myCobot.writeAngles({2.63, -22.23, -17.57, -132.71, 111.09, -7.73}, 20);
    delay(2500);
    myCobot.writeAngle(1, 50, 30);
    delay(2000);
    clearAndPrint("Go to target location");

    myCobot.writeAngles(targetArr[i], 43);
    delay(2000);
    down_pos(3);
    delay(1500);
    pumpOff();
    delay(300);
    up_pos(5);
    delay(2000);
    myCobot.writeAngles({2.63, -22.23, -17.57, -132.71, 111.09, -7.73}, 37);
    // myCobot.writeAngles(mid, 53);
    clearAndPrint("Finish No." + String(i + 1) + " times");
    delay(2500);
    i++;
  }
  delay(2000);
}

void pumpOn()
{
  digitalWrite(2, 0);
  digitalWrite(5, 0);
}

void pumpOff()
{
  digitalWrite(2, 1);
  digitalWrite(5, 1);
}

/**
 * Screen display help method.
 */
void displayToScreen(int displayMode)
{
  // Clear full screen.
  M5.Lcd.fillScreen(0);
  // Write title.
  M5.Lcd.setTextSize(4);
  M5.Lcd.drawString("Palletizing", 30, 0);
  // Write buttons.
  M5.Lcd.setTextSize(2);
  M5.Lcd.drawString("A", 60, 210);
  M5.Lcd.drawString("B", 150, 210);
  M5.Lcd.drawString("C", 240, 210);
  M5.Lcd.setCursor(0, 40);

  // Follow displayMode Write different content.
  switch (displayMode)
  {
  case 0:
    M5.Lcd.print("This demo is used to feel the difference between different control methods.\n\n");
    M5.Lcd.print("- Press A to palletizing 2x2.\n");
    // M5.Lcd.print("- Press B to move with coord.\n");
    M5.Lcd.print("- Press C to set free move.\n");
    M5.Lcd.print("");
    break;
  case 2:
    M5.Lcd.print("Start palletizing --> 2x2(four times in total)");
    break;
  case 4:
    M5.Lcd.print("Coord moving. Press A to move one by one, Press C to exit.");
    break;
  case 6:
    M5.Lcd.print("Linear coord moving. Press A to move one by one, Press C to exit.");
    break;
  case 1:
  case 3:
  case 5:
    M5.Lcd.print("Waiting mycobot initialize status...");
    break;
  default:
    break;
  }
}

void clearAndPrint(String msg)
{
  M5.Lcd.setCursor(0, 110);
  M5.Lcd.print("                                                            ");
  M5.Lcd.setCursor(0, 110);
  M5.Lcd.print(msg);
}

Coords pos_data;
Angles angles_data;

void read_pos()
{
  Coords data1;
  data1 = myCobot.getCoords();
  delay(50);
  for (int i = 0; i < 7; i++)
  {
    Serial.print(data1[i]);
    Serial.print(", ");
  }
  Serial.println();
  pos_data = data1;
}

void down_pos(int count)
{
  read_pos();
  Coords data1;
  data1 = pos_data;
  for (int i = 0; i < count; i++)
  {
    myCobot.writeCoords(data1, 20);
    data1[2] -= 10;
    delay(50);
  }
}

void up_pos(int count)
{
  read_pos();
  Coords data1;
  data1 = pos_data;
  for (int i = 0; i < count; i++)
  {
    myCobot.writeCoords(data1, 20);
    data1[2] += 12;
    delay(50);
  }
}
