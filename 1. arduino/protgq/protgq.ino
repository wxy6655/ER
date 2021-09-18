#include <MycobotBasic.h>
#include <ParameterList.h>

MycobotBasic myCobot;

Angles angles[]= {
            Angles{0.26, -32.08, -100.81, 48.95, 1.66, -0.52},
            Angles{0.26, -49.74, -111.26, 77.78, 1.66, -0.52},
            Angles{0.26, -32.08, -100.81, 48.95, 1.66, -0.52},
            Angles{28.03, -26.36, -111.88, 54.05, -2.37, 26.54},
            Angles{28.03, -47.37, -123.31, 86.92, -2.37, 26.54},
            Angles{28.03, -26.36, -111.88, 54.05, -2.37, 26.54}      
};

void setup() {
  // put your setup code here, to run once:
  myCobot.setup();                                      
  myCobot.powerOn();                                    //启动机械臂
  delay(50);
  Angles init = {0, 0, 0, 0, 0, 0};
  myCobot.writeAngles(init, 50);
  delay(3000);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0; i<6; i++){
    myCobot.writeAngles(angles[i], 100);
    delay(1000);
  }
  if (M5.BtnA.wasPressed()||M5.BtnC.wasPressed()||M5.BtnB.wasPressed()) {
    while(1){
      if (M5.BtnA.wasPressed()||M5.BtnC.wasPressed()||M5.BtnB.wasPressed()){
        break;
    }
    }
  } 
}
