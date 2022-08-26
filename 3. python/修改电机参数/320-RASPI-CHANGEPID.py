from json.tool import main
import time
from turtle import goto
from pymycobot.mycobot import MyCobot
import serial.tools.list_ports
import numpy as np

data_id = [21, 22, 23, 24, 26, 27] 
data    = [25, 25, 1, 0, 3, 3]
mc = []
ports = []

def setup():                                                    #机械臂检测函数，选择正确的串口
    global mc
    print("")

    plist = list(serial.tools.list_ports.comports())
    idx = 0
    for port in plist:
        print("{} : {}".format(idx, port))
        idx += 1
    if idx == 0:
        print("The connected device was not detected. Please try reconnecting.")
        exit(1)
    _in = input("\nPlease input 0 - {} to choice, you can choice many like: '2,1,3':".format(idx))
    idxes = _in.split(',')
    try:
        idxes = [int(i) for i in idxes]
    except Exception:
        print('Error: Input format error.')
        exit(1)

    ports = [str(plist[i]).split(' - ')[0].strip() for i in idxes]
    
    print(ports)
    print("")

    baud = 115200
    _baud = input("Please input baud(default:115200，can input 1000000):")
    try:
        baud = int(_baud)
    except Exception:
        pass
    print(baud)
    print("")

    for port in ports:
        try:
            mycobot = MyCobot(port, baud)
        except Exception as e:
            print(e)
            exit(1)
        mc.append(mycobot)

def change():
    global mc, data, data_id
    _mycbot = mc
    print(_mycbot)
    for i in range(1,7):
        for j in range(len(data_id)):
            _mycbot.set_servo_data(i, data_id[j], data[j])
            time.sleep(0.2)
            _data = _mycbot.get_servo_data(i, data_id[j])
            time.sleep(0.2)
            if _data == data[j]:
                print("Servo motor :" + str(i) + "  data_id : " + str(data_id[j]) + "   data: " + str(_data) + "  modify successfully ")
            else:
                print("Servo motor :"  + str(i) + "  data_id : " + str(data_id[j]) + "   data: " + str(_data) + "  modify error ")

if __name__ == "__main__":                                      #主函数
    # setup()
    mc = MyCobot('/dev/ttyAMA0',115200)
    print(mc)
    try:
        change()                            
    except Exception as e:
        print(e)

    


