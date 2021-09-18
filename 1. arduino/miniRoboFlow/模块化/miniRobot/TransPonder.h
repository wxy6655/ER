#ifndef TransPonder_h
#define TransPonder_h

#include <MycobotBasic.h>




class Transponder
{
private:
    bool checkHeader(MycobotBasic &myCobot);
    int  readSerial(MycobotBasic &myCobot, unsigned char* nDat, int nLen);
    void rFlushSerial();
    int  readData(MycobotBasic &myCobot);
    void connect_ATOM(MycobotBasic &myCobot);
    void info();
public:
    void ponder(MycobotBasic &myCobot);
    void init();

};

#endif


