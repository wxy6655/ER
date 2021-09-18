// #define ARDUINOOSC_DEBUGLOG_ENABLE
// #include <ArduinoOSC.h>

#include <ArduinoOSCWiFi.h>
#include <MycobotBasic.h>
MycobotBasic myCobot;
//
#define TEXT "aA MWyz~12"
//
#include <M5Stack.h>
#include "Free_Fonts.h" 

// WiFi stuff
const char* ssid = "Anki Go";
const char* pwd = "31415926";

const IPAddress ip(192,168,3,9);
//const IPAddress ip(192, 168, 1, 201);
const IPAddress gateway(192, 168, 3, 1);
const IPAddress subnet(255, 255, 255, 0);

// for ArduinoOSC
const char* host = "10.123.1.65";
const int recv_port = 54321;
const int bind_port = 54345;
const int send_port = 55555;
const int publish_port = 54445;
// send / receive varibales
int i;
float f;
String s;
String text = "hello";
String timer;
void onOscReceived(const OscMessage& m) {

    M5.begin();
    M5.Power.begin();
    
    Serial.print(m.remoteIP());
    Serial.print(" ");
    Serial.print(m.remotePort());

}

void setup() {
    Serial.begin(115200);
    delay(2000);
    
    myCobot.setup();

    // WiFi stuff (no timeout setting for WiFi)
#ifdef ESP_PLATFORM
    WiFi.disconnect(true, true);  // disable wifi, erase ap info
    delay(1000);
    WiFi.mode(WIFI_STA);
#endif
    WiFi.begin(ssid, pwd);
    WiFi.config(ip, gateway, subnet);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.print("WiFi connected, IP = ");
    Serial.println(WiFi.localIP());
  

    OscWiFi.subscribe(recv_port, "/test", [](const OscMessage& m) {

        
        int _speed = m.arg<int>(7);
        Angles _data = myCobot.getAngles();
        delay(200);
        for(int i = 1; i < 7; i++)        
        {
            //myCobot.writeAngle(i,angle,0-100);
            myCobot.writeAngle(i, m.arg<int>(i), _speed);   // 
            Serial.print(" i : ");
            Serial.print(i);
            Serial.print("  agt angle : ");
            Serial.print(m.arg<int>(i));
            Serial.print("  now angle : ");
            Serial.print(_data[i-1]);
            delay(100);
        }
        Serial.print(" _speed : ");
        Serial.print(_speed);
        Serial.println();
//        delay(2000);
//        if(text != m.arg<String>(8))
//        {
//          text = m.arg<String>(8);
//          M5.Lcd.fillScreen(TFT_BLACK);
//          
//          M5.Lcd.setFreeFont(FF32);
//          M5.Lcd.setTextDatum(MC_DATUM);
////          M5.Lcd.setTextSize(10);
//          M5.Lcd.setTextColor(WHITE);
//          M5.Lcd.drawString(text, 160, 120, GFXFF);
//          delay(10);
//          
////          M5.Lcd.setTextSize(5);å
//
//          M5.Lcd.setFreeFont(FF8);
//          M5.Lcd.setTextColor(TFT_PINK);
//          M5.Lcd.drawString("Red", 60, 170, GFXFF);
//          M5.Lcd.setTextColor(TFT_BLUE);
//          M5.Lcd.drawString("Blue", 215, 170, GFXFF);
//          delay(100);
//          M5.update(); 
//        }
       
    
    });

     OscWiFi.subscribe(recv_port, "/timer", [](const OscMessage& m) 
     {
        timer = m.arg<String>(0);
        Serial.println(timer);
        M5.Lcd.setFreeFont(FF8);
        M5.Lcd.setTextColor(WHITE,BLACK);
        M5.Lcd.drawString(timer, 160, 60, GFXFF);
        delay(100);
        
       M5.update();
     });
}
void loop() {
    OscWiFi.update();  // should be called to receive + send osc
    delay(100);
      
    // if you want to use Releasefor("was released for"), use .wasReleasefor(int time) below
//    if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(1000, 200)) {
//         OscWiFi.send(host, publish_port, "/End", 1);
//    } 
//    if (M5.BtnB.wasReleased() || M5.BtnB.pressedFor(1000, 200)) {
//       OscWiFi.send(host, publish_port, "/Start", 1);
//    } 
}
