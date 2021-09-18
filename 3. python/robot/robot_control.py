'''
Author: your name
Date: 2021-05-12 14:38:04
LastEditTime: 2021-05-12 15:02:31
LastEditors: Please set LastEditors
Description: In User Settings Edit
FilePath: /maix_train/home/wxy/Desktop/feng.py
'''
import time
import threading
from pymycobot.mycobot import MyCobot
import serial.tools.list_ports

mc = []
ports = []

#初始坐标定义，基础末端姿态，只可修改 X, Y, Z， x为前后，y为左右， z为上下， 前加后减，左加右减，上加下减
INIT_X = 135.10
INIT_Y = -45.10
INIT_Z = 90.10

 #推荐15-30，速度过快会卡顿
Speed = 15        

# 基础坐标
Coord_int_1 = [INIT_X, INIT_Y, INIT_Z, -0.45, 5.7, -1.44]        

#异步执行等待时间
wait_time = 10

#提起高度50mm
Z_data_1 = 5

#下降距离50mm
Z_data_2 = -5

#左移距离150mm
Y_data_1 = 10

#右移距离300mm
Y_data_2 = -20

#定义直线运动轴向
X = 0
Y = 1
Z = 2

def detected_port():                                            #检测链接设备数量
    port_list = list(serial.tools.list_ports.comports())
    for i in range(len(port_list)):
        ports.append(port_list[i][0])
    print(ports)

def set_line_runing(cobot, axis, step):                              #控制末端做直线移动，cobot是实例化后的串口指向，axis是轴向：X,Y,Z, step是步数，步长是10mm。
    pos_1 = cobot.get_coords()
    if all(pos_1):
        print(pos_1)
        time.sleep(0.05)
        if step > 0:
            for _ in range(step):
                pos_1[axis] += 10
                cobot.send_coords(pos_1, Speed, 1)
                time.sleep(0.05)
        else:
            step = 0 - step
            for _ in range(step):
                pos_1[axis] -= 10
                cobot.send_coords(pos_1, Speed, 1)
                time.sleep(0.05)
        time.sleep(2)
    else:
        print("con not find pos")

def robot_init(cobot):                                         # 机械臂姿态初始化
    cobot.send_angles([0,0,0,0,0,0], 100)
    time.sleep(3)
    print("angles")
    cobot.send_coords(Coord_int_1, 50, 1)
    time.sleep(3)

def cobot_runing(cobot):                                        #运动控制函数
    global Y, Z, Z_data_1, Z_data_2, Y_data_1, Y_data_2
    time.sleep(1)
    cobot.set_color(0, 255, 0)
    time.sleep(0.50)
    #set_line_runing(cobot, Z, Z_data_1)                              
    time.sleep(1)
    set_line_runing(cobot, Y, Y_data_1)
    time.sleep(1)
    #set_line_runing(cobot, Z, Z_data_1)
    time.sleep(1)
    set_line_runing(cobot, Y, Y_data_2)
    time.sleep(1)
    #set_line_runing(cobot, Z, Z_data_1)
    time.sleep(1)
    set_line_runing(cobot, Y, Y_data_1)
    time.sleep(1)
    time.sleep(0.50)



def paly_robot_Asynchronous():                                          #异步运行函数
    global wait_time
    for cobot in mc:
        threading.Thread(target=robot_init, args=[cobot]).start()
    time.sleep(6)
    for cobot in mc:
        threading.Thread(target=cobot_runing, args=[cobot]).start()
        time.sleep(wait_time)


def play_robot_Synchronous():                                           #同步运行函数
    for cobot in mc:
        threading.Thread(target=robot_init, args=[cobot]).start()
    time.sleep(6)
    for cobot in mc:
        threading.Thread(target=cobot_runing, args=[cobot]).start()

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
    setup()
    print(mc)
    # play_robot_Synchronous()                          #同步执行
    paly_robot_Asynchronous()                           #异步执行
