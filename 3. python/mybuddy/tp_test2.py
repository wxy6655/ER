from pyexpat.errors import XML_ERROR_FEATURE_REQUIRES_XML_DTD
from re import A
import time
import threading
from pymycobot.mycobot import MyCobot
import serial.tools.list_ports
import numpy as np
import os

mc = []
ports = []


init_X_L = -114.0
init_y_L = 202.9
init_z_L = 53.4


x_base_L = 202.9 
y_base_L = 53.4  
z_base_L = 127.5 - 50

x_fa_L = z_base_L - 241.5
y_fa_L = x_base_L
z_fa_L = y_base_L

y_offset = 100 # jiange zhi 

x_fa_R = -z_base_L + 241.5
y_fa_R = x_base_L
z_fa_R = -y_base_L + 100

init_angles_L = [-90.05, 45.04, 90.1, 45.52, -90.37, 0.64]
init_coords_L = [x_fa_L,y_fa_L,z_fa_L, 90.02, -0.78, -89.65]
init_X_R = 114.0
init_y_R = 202.9
init_z_R = 53.4
init_angles_R = [90.15, -45.06, -90.49, -45.18, -90.67, 0.24]
init_coords_R = [x_fa_R,y_fa_R,z_fa_R, 89.96, -0.96, 90.79]



def detected_port():                                            #检测链接设备数量
    port_list = list(serial.tools.list_ports.comports())
    for i in range(len(port_list)):
        ports.append(port_list[i][0])
    print(ports)

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
    # _baud = input("Please input baud(default:115200):")
    # try:
    #     baud = int(_baud)
    # except Exception:
    #     pass
    print(baud)
    print("")

    for port in ports:
        try:
            mycobot = MyCobot(port, baud)
        except Exception as e:
            print(e)
            exit(1)
        mc.append(mycobot)

class Helper(object):
    def __init__(self) -> None:
        self.w, self.h = os.get_terminal_size()

    def echo(self, msg):
        print("\r{}".format(" " * self.w), end="")
        print("\r{}".format(msg), end="")

class TeachingTest(Helper):
    def __init__(self, mycobot) -> None:
        super().__init__()
        self.mc = mycobot
        

    def robot_init(self, cobot):                                          #机械臂姿态初始化
        init_angles = [0,0,0,0,0,0]
        while True:
            try:
                print("init_angles_start")
                time.sleep(0.2)
                cobot.send_angles(init_angles, 50)
                time.sleep(0.10)
                cobot.set_color(0, 255, 0)
                time.sleep(0.10)
                if(self.Get_Angles_parameters(cobot, init_angles)):                       #判断角度误差范围是否符合
                    print("init_angles_success")
                else:
                    print("init_angles_error")
                    continue
                break
            except Exception as a:
                print(a)
        cobot.set_color(255, 0, 0)
        time.sleep(0.50)

   


    def paly_robot_Asynchronous(self):                                  #异步运行函数
        global wait_time
        # for cobot in mc:
        #     threading.Thread(target=self.robot_init, args=[cobot]).start()
        # # time.sleep(5)
        # mc[0].release_all_servos()
        # mc[1].release_all_servos()
        # self.echo("Released")
        

        mc[1].send_angles(init_angles_L, 50)
        mc[0].send_angles(init_angles_R, 50)
        time.sleep(5)
        for _ in range(1):
            for cobot in mc:
                print(cobot, cobot.get_angles())
        
        time.sleep(2)
        mc[1].send_coords(init_coords_L, 50, 2)
        mc[0].send_coords(init_coords_R, 50, 2)
        
        print(init_coords_R)
        print(init_coords_L)
        time.sleep(15)
        for _ in range(2):
            for cobot in mc:
                print(cobot, cobot.get_coords())

    def Get_Coords_parameters(self, mc, offset):                       #判断坐标误差范围是否符合
        AMDR = 10
        sum = 0
        while True:
            now_pos_1 = mc.get_coords()
            time.sleep(1)
            if now_pos_1:
                offset_angles = [round(i - j, 3) for i,j in zip(offset, now_pos_1)]
                ar = np.array(offset_angles)
                if (abs(ar) < AMDR).all():
                    return 1
                else:
                    sum += 1
            else:
                sum += 1
            if sum > 5:
                return 0

                            
    def Get_Angles_parameters(self, mc, offset):                       #判断角度误差范围是否符合
        AMDR = 3
        sum = 0
        while True:
            now_pos_1 = mc.get_angles()
            time.sleep(1)
            if now_pos_1:
                offset_angles = [round(i - j, 3) for i,j in zip(offset, now_pos_1)]
                ar = np.array(offset_angles)
                if (abs(ar) < AMDR).all():
                    return 1
                else:
                    sum += 1
            else:
                sum += 1
            if sum > 5:
                return 0

if __name__ == "__main__":                                      #主函数
    # setup()
    mc.append(MyCobot('COM9', 115200))
    mc.append(MyCobot('COM8', 115200))
    print(mc)
    t = TeachingTest(mc)
    
    try:
        # play_robot_Synchronous()                            #同步执行，不启动电机
        t.paly_robot_Asynchronous()                           #异步执行，启动电机
        # motor_control(mc[0])                                #io测试
    except Exception as e:
        print(e)
