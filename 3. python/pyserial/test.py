import serial #导入模块
import time
from asyncio.windows_events import NULL

_PORT = 'COM30'
_BAUD = 1000000

def Wriaterial(_mySerial,data):
     #十六进制的发送
    result = NULL
    for _data in data:
        _mySerial.write(hex(_data).encode())#写数据
        print(hex(_data).encode())
        result += 1
    print("write sum:",result)

def Readserial(_mySerial):
    if _mySerial.inWaiting() > 0:
        data = _mySerial.read(_mySerial.inWaiting())
        print("_read: {}".format(data))
        time.sleep(0.5)
    else:
        print("_read: no data can be read")
        data = None
    return data

try:
    _mySerial = serial.Serial(_PORT, _BAUD, timeout=0.1 ,rtscts=0, dsrdtr = 0)
    # _mySerial = serial.Serial()
    # _mySerial.port = _PORT
    # _mySerial.baudrate = _BAUD
    # _mySerial.timeout = 1
    # _mySerial.setDTR(False)
    # _mySerial.open()
    time.sleep(1)
    if _mySerial.isOpen():
        print(_mySerial)
except Exception as e:
    print("---error---：",e)
    exit(1)

while True:
    print(Readserial(_mySerial))
    time.sleep(0.002)