#ifndef connect_h
#define connect_h

#include "MycobotBasic.h"
#include "SCServo.h"

class Connect
{
private:
    /* data */
    void info();
    void testServo(MycobotBasic &myCobot);
public:
    void test(MycobotBasic &myCobot);
};

#endif
