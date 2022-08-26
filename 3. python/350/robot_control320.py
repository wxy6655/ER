import time
import threading
from pymycobot.mycobot import MyCobot
import serial.tools.list_ports

mc = []
ports = []  
Speed = 100
runing_mode = 1

def detected_port():                                            #检测链接设备数量
    port_list = list(serial.tools.list_ports.comports())
    for i in range(len(port_list)):
        ports.append(port_list[i][0])
    print(ports)


def robot_init(cobot):                                         # 机械臂姿态初始化
    cobot.send_angles([0,0,0,0,0,0], 50)
    time.sleep(3)
    cobot.send_angles([0, -10, -123, 45, 90, 20], 50)
    time.sleep(5)
    print("angles init ok")


def set_line_runing(cobot, axis, step):                              #控制末端做直线移动，cobot是实例化后的串口指向，axis是轴向：X,Y,Z, step是步数，步长是10mm。
    pos_1 = cobot.get_coords()
    if all(pos_1):
        print(pos_1)
        time.sleep(0.05)
        if step > 0:
            for _ in range(step):
                pos_1[axis] += 10
                cobot.send_coords(pos_1, Speed, runing_mode)
                time.sleep(0.05)
        else:
            step = 0 - step
            for _ in range(step):
                pos_1[axis] -= 10
                cobot.send_coords(pos_1, Speed, runing_mode)
                time.sleep(0.05)
        time.sleep(2)
    else:
        print("con not find pos")


def setup():
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
    _baud = input("Please input baud(default:115200):")
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


if __name__ == "__main__":  
    # setup()
    # print(mc)
    # cobot = mc[0]
    
    # print(cobot.get_angles())
    # cobot.set_refresh_mode(1)
    # time.sleep(5)
    # print(cobot.get_angles())
    # robot_init(mc[0])
    # for _ in range(5):
    #     set_line_runing(mc[0],0,10)  #x负方向走10步
    #     time.sleep(3)
    #     set_line_runing(mc[0],0,-10)  #x正方向走10步
    #     time.sleep(3)
    #     set_line_runing(mc[0],1,10)  #x负方向走10步
    #     time.sleep(3)
    #     set_line_runing(mc[0],1,-10)  #x正方向走10步
    #     time.sleep(3)
    #     set_line_runing(mc[0],2,10)  #x负方向走10步
    #     time.sleep(3)
    #     set_line_runing(mc[0],2,-10)  #x正方向走10步
    #     time.sleep(3)
    # cobot.send_angles([0,0,0,0,0,0],50)
    cobot = MyCobot('COM5', 115200)
    time.sleep(0.5)
    cobot.set_refresh_mode(1)
    time.sleep(0.5)
    cobot.send_angles([(-10.01),(1.75),-76.02,(-16.87),(89.12),(-53.17)],30)
    time.sleep(2)
    cobot.send_coords([187,(-122.7),275,-179,0.08,(-47.1)],10,0)
    time.sleep(3)
    cobot.send_coords([243.7,(-124.3),274.2,-179,0.33,(-47.19)],10,0)
    time.sleep(5)
    cobot.send_coords([187,(-122.7),275,-179,0.08,(-47.1)],20,0)
    time.sleep(2)
    