/*
   4WD mecanum wheel PS2 control
*/

#include <PS2X_lib.h>  //for v1.8
#include <Adafruit_NeoPixel.h>

#define MODE_MOTOR  0
#define MODE_RGBLED 1

/******************************************************************
   set pins connected to PS2 controller: replace pin
   numbers by the ones you use
 ******************************************************************/
#define PS2_DAT        A0
#define PS2_CMD        A1
#define PS2_CS         A2
#define PS2_CLK        A3

// sound pin number
#define SOUND_PIN    9

// motor pins
#define M1PinDir 4
#define M1PinSpeed 3
#define M2PinDir 7
#define M2PinSpeed 5
#define M3PinDir 8
#define M3PinSpeed 6
#define M4PinDir 12
#define M4PinSpeed 11

#define RGBPIN 13
#define NUMPIXELS 6

/******************************************************************
   select modes of PS2 controller:
     - pressures = analog reading of push-butttons
     - rumble    = motor rumbling
   uncomment 1 of the lines for each mode selection
 ******************************************************************/
//#define pressures   true
#define pressures   false
//#define rumble      true
#define rumble      false

PS2X ps2x; // create PS2 Controller Class
//right now, the library does NOT support hot pluggable controllers, meaning
//you must always either restart your Arduino after you connect the controller,
//or call config_gamepad(pins) again after connecting the controller.
int error = 0;
byte type = 0;
byte vibrate = 0;

Adafruit_NeoPixel strip(NUMPIXELS, RGBPIN, NEO_GRB + NEO_KHZ800);

uint8_t modeControl = 0;
uint8_t moveMode = 6;

const uint8_t defaultSpeed = 120;
#define maxSpeed  255
#define minSpeed  80
int m1Speed = defaultSpeed;
int m2Speed = defaultSpeed;
int m3Speed = defaultSpeed;
int m4Speed = defaultSpeed;


// Reset func
void (* resetFunc) (void) = 0;

void setup() {
  Serial.begin(9600);
  delay(500);  //added delay to give wireless ps2 module some time to startup, before configuring it

  pinMode(M1PinDir, OUTPUT);
  pinMode(M1PinSpeed, OUTPUT);
  pinMode(M2PinDir, OUTPUT);
  pinMode(M2PinSpeed, OUTPUT);
  pinMode(M3PinDir, OUTPUT);
  pinMode(M3PinSpeed, OUTPUT);
  pinMode(M4PinDir, OUTPUT);
  pinMode(M4PinSpeed, OUTPUT);

  strip.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all strip ASAP
  //  strip.setBrightness(50);

  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_CS, PS2_DAT, pressures, rumble);

  if (error == 0) {
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
    if (pressures)
      Serial.println("true ");
    else
      Serial.println("false");
    Serial.print("rumble = ");
    if (rumble)
      Serial.println("true)");
    else
      Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }
  else if (error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

  else if (error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if (error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

  type = ps2x.readType();
  switch (type) {
    case 0:
      Serial.println("Unknown Controller type found ");
      break;
    case 1:
      Serial.println("DualShock Controller found ");
      break;
    case 2:
      Serial.println("GuitarHero Controller found ");
      break;
    case 3:
      Serial.println("Wireless Sony DualShock Controller found ");
      break;
  }

  MSound(3, 60, 2000, 80, 2250, 100, 2500);

  strip.setPixelColor(0, strip.Color(150, 0, 0));
  strip.setPixelColor(1, strip.Color(150, 150, 0));
  strip.setPixelColor(2, strip.Color(0, 150, 0));
  strip.setPixelColor(3, strip.Color(0, 150, 150));
  strip.setPixelColor(4, strip.Color(0, 0, 150));
  strip.setPixelColor(5, strip.Color(150, 0, 150));
  strip.show();   // Send the updated pixel colors to the hardware.
}

void loop() {
  /* You must Read Gamepad to get new values and set vibration values
     ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
     if you don't enable the rumble, use ps2x.read_gamepad(); with no values
     You should call this at least once a second
  */
  if (error == 1) { //skip loop if no controller found
    resetFunc();
  }
  if (type != 2) { //DualShock Controller
    ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed

    if (ps2x.Button(PSB_START))        //will be TRUE as long as button is pressed
    {
      Serial.println("Start is being held");
    }
    if (ps2x.ButtonPressed(PSB_SELECT)) {
      MSound(1, 60, 2800);
      modeControl++;
      if (modeControl >= 2) {
        modeControl = 0;
        MSound(1, 60, 2000);
      }
      Serial.print("modeControl ");
      Serial.print(modeControl);
      Serial.println(" Select just pressed");
    }

    if (ps2x.ButtonPressed(PSB_PAD_UP)) {     //will be TRUE as long as button is pressed
      Serial.print("Up");
      //      Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
      moveMode = 0;
    }
    if (ps2x.ButtonPressed(PSB_PAD_RIGHT)) {
      Serial.print("Right");
      //      Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
      moveMode = 1;
    }
    if (ps2x.ButtonPressed(PSB_PAD_LEFT)) {
      Serial.print("LEFT");
      //      Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
      moveMode = 2;
    }
    if (ps2x.ButtonPressed(PSB_PAD_DOWN)) {
      Serial.print("DOWN");
      //      Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
      moveMode = 3;
    }
    // 左旋转
    if (ps2x.ButtonPressed(PSB_SQUARE)) {            //will be TRUE if button was JUST released
      Serial.println("Square just pressed");
      moveMode = 4;
    }
    // 右旋转
    if (ps2x.ButtonPressed(PSB_CIRCLE)) {             //will be TRUE if button was JUST pressed
      Serial.println("Circle just pressed");
      moveMode = 5;
    }

    // 停止
    if (ps2x.ButtonReleased(PSB_PAD_UP) || ps2x.ButtonReleased(PSB_PAD_RIGHT)
        || ps2x.ButtonReleased(PSB_PAD_LEFT) || ps2x.ButtonReleased(PSB_PAD_DOWN)
        || ps2x.ButtonReleased(PSB_SQUARE) || ps2x.ButtonReleased(PSB_CIRCLE)
        || ps2x.ButtonReleased(PSB_L1) || ps2x.ButtonReleased(PSB_R1)) {
      moveMode = 6;
    }
    // 增速 / RGBLED test
    if (ps2x.ButtonPressed(PSB_TRIANGLE)) {
      Serial.print("Triangle just pressed");
      if (modeControl == MODE_MOTOR) {
        MSound(1, 60, 2000);
        int sp = 10;
        m1Speed += sp; // range(minSpeed ~ maxSpeed)
        if (m1Speed > maxSpeed) {
          MSound(1, 60, 2500);
          m1Speed = maxSpeed;
        }
        m2Speed = m1Speed; // range(minSpeed ~ maxSpeed)
        m3Speed = m1Speed; // range(minSpeed ~ maxSpeed)
        m4Speed = m1Speed; // range(minSpeed ~ maxSpeed)
        Serial.print("  + speed  ");
        Serial.print(m1Speed);
        Serial.println();
      } else if (modeControl == MODE_RGBLED) {
        rgbTest();
        strip.clear(); // Set all pixel colors to 'off'
        strip.show();   // Send the updated pixel colors to the hardware.
      }
    }
    // 减速 / RGBLED test rainbow
    if (ps2x.ButtonPressed(PSB_CROSS)) {             //will be TRUE if button was JUST pressed OR released
      Serial.print("X just pressed");
      if (modeControl == MODE_MOTOR) {
        MSound(1, 60, 2000);
        int sp = 10;
        m1Speed -= sp; // range(minSpeed ~ maxSpeed)
        if (m1Speed < minSpeed) {
          MSound(1, 60, 2500);
          m1Speed = minSpeed;
        }
        m2Speed = m1Speed; // range(minSpeed ~ maxSpeed)
        m3Speed = m1Speed; // range(minSpeed ~ maxSpeed)
        m4Speed = m1Speed; // range(minSpeed ~ maxSpeed)
        Serial.print("  - speed  ");
        Serial.print(m1Speed);
        Serial.println();
      } else if (modeControl == MODE_RGBLED) {
        rainbow(1);             // Flowing rainbow cycle along the whole strip
        strip.clear(); // Set all pixel colors to 'off'
        strip.show();   // Send the updated pixel colors to the hardware.
      }
    }

    // 恢复初始速度 defaultSpeed 120
    if (ps2x.ButtonPressed(PSB_R2)) {
      // reset speed;
      m1Speed = defaultSpeed;
      m2Speed = defaultSpeed;
      m3Speed = defaultSpeed;
      m4Speed = defaultSpeed;
      Serial.print("Reset speed  ");
      Serial.print(m1Speed);
      Serial.println();
    }
    // 电机运动模式
    if (modeControl == MODE_MOTOR) {
      if (moveMode == 0)
        setMotor(m1Speed, m2Speed, m3Speed, m4Speed);
      else if (moveMode == 1)
        setMotor(m1Speed, -m2Speed, -m3Speed, m4Speed);
      else if (moveMode == 2)
        setMotor(-m1Speed, m2Speed, m3Speed, -m4Speed);
      else if (moveMode == 3)
        setMotor(-m1Speed, -m2Speed, -m3Speed, -m4Speed);
      else if (moveMode == 4)
        setMotor(-m1Speed, m2Speed, -m3Speed, m4Speed);
      else if (moveMode == 5)
        setMotor(m1Speed, -m2Speed, m3Speed, -m4Speed);
      else if (moveMode == 6)
        setMotor(0, 0, 0, 0);
    }

    vibrate = ps2x.Analog(PSAB_CROSS);  //this will set the large motor vibrate speed based on how hard you press the blue (X) button

    if (ps2x.NewButtonState()) {        //will be TRUE if any button changes state (on to off, or off to on)
      if (ps2x.Button(PSB_L3))
        Serial.println("L3 pressed");
      if (ps2x.Button(PSB_R3))
        Serial.println("R3 pressed");
    }

    if (ps2x.Button(PSB_L2)) {
      Serial.println("  L2 pressed");
    }

    if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) { //print stick values if either is TRUE
      //      Serial.print("Stick Values:");
      //      Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX
      //      Serial.print(",");
      //      Serial.print(ps2x.Analog(PSS_LX), DEC);
      //      Serial.print(",");
      //      Serial.print(ps2x.Analog(PSS_RY), DEC);
      //      Serial.print(",");
      //      Serial.println(ps2x.Analog(PSS_RX), DEC);

      int speedm_x = 0;
      int speedm_y = 0;
      int speedm_w = 0;

      if (ps2x.Analog(PSS_LY) < 127 && ps2x.Analog(PSS_LY) >= 0) {
        speedm_y = map(ps2x.Analog(PSS_LY), 127, 0, 0, maxSpeed);
      } else if (ps2x.Analog(PSS_LY) > 127 && ps2x.Analog(PSS_LY) <= 255) {
        speedm_y = map(ps2x.Analog(PSS_LY), 255, 127, -maxSpeed, 0);
      } else
        speedm_y = 0;

      if (ps2x.Analog(PSS_LX) < 128 && ps2x.Analog(PSS_LX) >= 0) {
        speedm_x = map(ps2x.Analog(PSS_LX), 0, 128, -maxSpeed, 0);
      } else if (ps2x.Analog(PSS_LX) > 128 && ps2x.Analog(PSS_LX) <= 255) {
        speedm_x = map(ps2x.Analog(PSS_LX), 128, 255, 0, maxSpeed);
      } else
        speedm_x = 0;

      if (ps2x.Analog(PSS_RX) < 128 && ps2x.Analog(PSS_RX) >= 0) {
        speedm_w = map(ps2x.Analog(PSS_RX), 0, 128, -maxSpeed, 0);
      } else if (ps2x.Analog(PSS_RX) > 128 && ps2x.Analog(PSS_RX) <= 255) {
        speedm_w = map(ps2x.Analog(PSS_RX), 128, 255, 0, maxSpeed);
      } else
        speedm_w = 0;

#ifdef DEBUGESERIAL
      Serial.print("y:");
      Serial.print(speedm_y);
      Serial.print(",x:");
      Serial.print(speedm_x);
      Serial.print(",M1:");
      Serial.print(speedm_y + speedm_x);
      Serial.print(",M2:");
      Serial.print(speedm_y - speedm_x);
      Serial.print(",M3:");
      Serial.print(speedm_y - speedm_x);
      Serial.print(",M4:");
      Serial.print(speedm_y + speedm_x);
      Serial.println();
#endif

#ifdef DEBUGESERIAL
      Serial.print("M1:");
      Serial.print(speedm_w);
      Serial.print(",M2:");
      Serial.print( - speedm_w);
      Serial.print(",M3:");
      Serial.print(speedm_w);
      Serial.print(",M4:");
      Serial.print( - speedm_w);
      Serial.println();
#endif
      //      setMotor(speedm_w, - speedm_w, speedm_w, - speedm_w); // 电机M1,M2,M3,M4 运动
      setMotor(speedm_y + speedm_x + speedm_w, speedm_y - speedm_x - speedm_w, speedm_y - speedm_x + speedm_w, speedm_y + speedm_x - speedm_w); // 电机M1,M2,M3,M4 运动
    }
    delay(50);
  }
}

void rgbTest() {
  for (int i = 0; i < 6; i++)
    strip.setPixelColor(i, strip.Color(150, 0, 0));
  strip.show();   // Send the updated pixel colors to the hardware.
  delay(100);
  for (int i = 0; i < 6; i++)
    strip.setPixelColor(i, strip.Color(0, 150, 0));
  strip.show();   // Send the updated pixel colors to the hardware.
  delay(100);
  for (int i = 0; i < 6; i++)
    strip.setPixelColor(i, strip.Color(0, 0, 150));
  strip.show();   // Send the updated pixel colors to the hardware.
  delay(100);
}

// Fill strip strip one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between strip.
void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  for (int a = 0; a < 10; a++) { // Repeat 10 times...
    for (int b = 0; b < 3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all strip in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for (int c = b; c < strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256) {
    for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for (int a = 0; a < 30; a++) { // Repeat 30 times...
    for (int b = 0; b < 3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all strip in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for (int c = b; c < strip.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}

/*
  驱动电机函数
  参数：m1Speed - 1电机速度 m2Speed - 2电机速度
       m3Speed - 3电机速度 m4Speed - 4电机速度 （取值范围：-255 ~ 255）
*/
void setMotor(int m1Speed, int m2Speed, int m3Speed, int m4Speed) {
  if (m1Speed > 0)
    digitalWrite(M1PinDir, LOW);
  else if (m1Speed < 0)
    digitalWrite(M1PinDir, HIGH);
  analogWrite(M1PinSpeed, abs(m1Speed));

  if (m2Speed > 0)
    digitalWrite(M2PinDir, HIGH);
  else if (m2Speed < 0)
    digitalWrite(M2PinDir, LOW);
  analogWrite(M2PinSpeed, abs(m2Speed));

  if (m3Speed > 0)
    digitalWrite(M3PinDir, LOW);
  else if (m3Speed < 0)
    digitalWrite(M3PinDir, HIGH);
  analogWrite(M3PinSpeed, abs(m3Speed));

  if (m4Speed > 0)
    digitalWrite(M4PinDir, HIGH);
  else if (m4Speed < 0)
    digitalWrite(M4PinDir, LOW);
  analogWrite(M4PinSpeed, abs(m4Speed));
}

// BUGBUG:: Move to some library...
//==============================================================================
//    SoundNoTimer - Quick and dirty tone function to try to output a frequency
//            to a speaker for some simple sounds.
//    快速而粗糙的音调功能试图向扬声器输出一些频率以获得一些简单的声音。
//==============================================================================
#ifdef SOUND_PIN
void SoundNoTimer(unsigned long duration,  unsigned int frequency) {
#ifdef __AVR__
  volatile uint8_t *pin_port;
  volatile uint8_t pin_mask;
#else
  volatile uint32_t *pin_port;
  volatile uint16_t pin_mask;
#endif
  long toggle_count = 0;
  long lusDelayPerHalfCycle;

  // Set the pinMode as OUTPUT
  pinMode(SOUND_PIN, OUTPUT);

  pin_port = portOutputRegister(digitalPinToPort(SOUND_PIN));
  pin_mask = digitalPinToBitMask(SOUND_PIN);

  toggle_count = 2 * frequency * duration / 1000;
  lusDelayPerHalfCycle = 1000000L / (frequency * 2);

  // if we are using an 8 bit timer, scan through prescalars to find the best fit
  while (toggle_count--) {
    // toggle the pin
    *pin_port ^= pin_mask;

    // delay a half cycle
    delayMicroseconds(lusDelayPerHalfCycle);
  }
  *pin_port &= ~(pin_mask);  // keep pin low after stop
}

void MSound(byte cNotes, ...) {
  va_list ap;
  unsigned int uDur;
  unsigned int uFreq;
  va_start(ap, cNotes);

  while (cNotes > 0) {
    uDur = va_arg(ap, unsigned int);
    uFreq = va_arg(ap, unsigned int);
    SoundNoTimer(uDur, uFreq);
    cNotes--;
  }
  va_end(ap);
}
#else
void MSound(byte cNotes, ...)
{
};
#endif
