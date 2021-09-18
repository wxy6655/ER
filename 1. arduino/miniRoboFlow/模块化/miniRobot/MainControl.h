#ifndef MainControl_h
#define MainControl_h

#include <MycobotBasic.h>
#include "language.h"



class MainControl
{
private:
    void updateMode(MycobotBasic &myCobot, byte btn_pressed);
    void displayInfo(MycobotBasic &myCobot, byte mc_mode);
    void record(MycobotBasic &myCobot);  // is stop;
    void play(MycobotBasic &myCobot);  // is stop  is pause
    void playFromFlash(MycobotBasic &myCobot);
    void recordIntoFlash(MycobotBasic &myCobot);
    void IO(MycobotBasic &myCobot);
    // bool checkDataLen();
public:
    void Control(MycobotBasic &myCobot);
};

#endif