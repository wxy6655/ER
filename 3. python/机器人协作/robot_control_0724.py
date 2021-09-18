from re import A
import time
import threading
from pymycobot.mycobot import MyCobot
import serial.tools.list_ports
import numpy as np

mc = []
ports = []

#初始坐标定义，基础末端姿态，只可修改 X, Y, Z， x为前后，y为左右， z为上下， 前加后减，左加右减，上加下减
INIT_X = 135.10
INIT_Y = -45.10
INIT_Z = 90.10

#推荐15-30，速度过快会卡顿
Speed = 15        

#基础坐标
Coord_int_1 = [INIT_X, INIT_Y, INIT_Z, -180.00, 0.10, -90.1]        

#异步执行等待时间
wait_time = 10

#提起高度50mm
Z_data_1 = 5

#下降距离50mm
Z_data_2 = -5

#左移距离150mm
Y_data_1 = 15

#右移距离300mm
Y_data_2 = -30

#定义直线运动轴向
X = 0
Y = 1
Z = 2

#定义末端执行器电机最大速度运行时间
motor_run_time = 3

# #定义末端执行器电机控制引脚为23，VCC接23，GND接GND
# motor_io = 23

# #定义末端执行器电机控制引脚输出频率，范围1000-30000；
# motor_frequency = 10000

# #定义末端执行器电机启动速度为0，不可调节
# start_motor_speed = 0

# #定义末端执行器电机最大速度为80%，范围0-10
# motor_speed = 8




def detected_port():                                            #检测链接设备数量
    port_list = list(serial.tools.list_ports.comports())
    for i in range(len(port_list)):
        ports.append(port_list[i][0])
    print(ports)

def set_line_runing(cobot, axis, step):                         #控制末端做直线移动，cobot是实例化后的串口指向，axis是轴向：X,Y,Z, step是步数，步长是10mm。
    checksum = 0
    while True:
        pos_1 = cobot.get_coords()
        time.sleep(0.2)
        if pos_1 == []:
            checksum += 1
            if checksum > 10:
                print("can not get coords, plase check connect")
                break
            continue
        else:
            break    
    if step > 0:
        for _ in range(step):
            pos_1[axis] += 10
            cobot.send_coords(pos_1, Speed, 2)
            time.sleep(0.05)
    else:
        step = 0 - step
        for _ in range(step):
            pos_1[axis] -= 10
            cobot.send_coords(pos_1, Speed, 2)
            time.sleep(0.05)
    time.sleep(2)

def robot_init(cobot):                                          #机械臂姿态初始化
    init_angles = [0,0,0,0,0,0]
    while True:
        try:
            print("init_angles_start")
            time.sleep(0.2)
            cobot.send_angles(init_angles, 50)
            time.sleep(0.10)
            cobot.set_color(0, 255, 0)
            time.sleep(0.10)
            if(Get_Angles_parameters(cobot, init_angles)):                       #判断角度误差范围是否符合
                print("init_angles_success")
            else:
                print("init_angles_error")
                continue
            cobot.send_coords(Coord_int_1, 50, 2)
            # if(Get_Coords_parameters(cobot, Coord_int_1)):
            #     print("init_coords_success")
            break
            # else:
            #     print("init_coords_error")
            #     continue
        except Exception as a:
            print(a)
    cobot.set_color(255, 0, 0)
    time.sleep(0.50)
    cobot.set_basic_output(2, 1)
    time.sleep(0.50)
    cobot.set_basic_output(5, 0)
    time.sleep(5)

def cobot_runing(cobot):                                        #运动控制函数
    global Y, Z, Z_data_1, Z_data_2, Y_data_1, Y_data_2
    time.sleep(1)
    cobot.set_color(0, 255, 0)
    time.sleep(0.50)
    set_line_runing(cobot, Z, Z_data_1)                              
    time.sleep(1)
    set_line_runing(cobot, Y, Y_data_1)
    time.sleep(1)
    set_line_runing(cobot, Z, Z_data_2)
    time.sleep(1)
    set_line_runing(cobot, Y, Y_data_2)
    time.sleep(1)
    set_line_runing(cobot, Z, Z_data_1)
    time.sleep(1)
    set_line_runing(cobot, Y, Y_data_1)
    time.sleep(1)
    cobot.set_color(255, 0, 0)
    time.sleep(0.50)

def cobot_1_runing(cobot):                                      #运动控制函数1
    global Y, Z, Z_data_1, Z_data_2, Y_data_1, Y_data_2
    time.sleep(1)
    cobot.set_color(0, 255, 0)
    time.sleep(0.50)
    set_line_runing(cobot, Z, Z_data_1)                              
    time.sleep(1)
    set_line_runing(cobot, Y, Y_data_1)
    time.sleep(1)
    set_line_runing(cobot, Z, Z_data_2)
    time.sleep(1)
    set_line_runing(cobot, Y, Y_data_2)
    time.sleep(1)
    cobot.set_color(255, 0, 0)
    time.sleep(0.50)
    motor_control(cobot)
    time.sleep(1)
    cobot.set_color(0, 255, 0)
    time.sleep(0.50)
    time.sleep(1)
    set_line_runing(cobot, Z, Z_data_1)
    time.sleep(1)
    set_line_runing(cobot, Y, Y_data_1)
    time.sleep(1)
    cobot.set_color(255, 0, 0)
    time.sleep(0.50)

def cobot_2_runing(cobot):                                      #运动控制函数2
    global Y, Z, Z_data_1, Z_data_2, Y_data_1, Y_data_2
    time.sleep(1)
    cobot.set_color(0, 255, 0)
    time.sleep(0.50)
    set_line_runing(cobot, Z, Z_data_1)                              
    time.sleep(1)
    set_line_runing(cobot, Y, Y_data_1)
    time.sleep(1)
    set_line_runing(cobot, Z, Z_data_2)
    time.sleep(1)
    cobot.set_color(255, 0, 0)
    time.sleep(0.50)
    motor_control(cobot)
    time.sleep(1)
    cobot.set_color(0, 255, 0)
    time.sleep(0.50)
    set_line_runing(cobot, Y, Y_data_2)
    time.sleep(1)
    cobot.set_basic_output(2, 0)
    time.sleep(0.50)
    cobot.set_basic_output(2, 1)
    time.sleep(0.50)
    set_line_runing(cobot, Z, Z_data_1)
    time.sleep(1)
    set_line_runing(cobot, Y, Y_data_1)
    time.sleep(1)
    cobot.set_color(255, 0, 0)
    time.sleep(0.50)

def motor_control(cobot):                                       #电机控制函数，变频启动与关闭
    # global start_motor_speed, motor_io, motor_frequency
    # for _ in range(motor_speed):
    #     cobot.set_pwm_output(motor_io, motor_frequency, start_motor_speed)
    #     start_motor_speed += 10
    #     time.sleep(0.1)
    # time.sleep(motor_run_time)
    # for _ in range(motor_speed):
    #     cobot.set_pwm_output(motor_io, motor_frequency, start_motor_speed)
    #     start_motor_speed -= 10
    #     time.sleep(0.1)
    cobot.set_pin_mode(23, 1)
    time.sleep(0.1)
    cobot.set_pin_mode(19, 1)
    time.sleep(0.1)
    cobot.set_digital_output(23, 0)
    time.sleep(0.1)
    cobot.set_digital_output(19, 0)
    time.sleep(0.1)
    print("servo motor start")
    cobot.set_digital_output(23, 1)
    time.sleep(0.1)
    cobot.set_digital_output(19, 1)
    time.sleep(motor_run_time)
    cobot.set_digital_output(23, 0)
    time.sleep(0.1)
    cobot.set_digital_output(19, 0)
    time.sleep(0.1)
    print("servo motor stop")

def Threading_cobot(mc):                                        #运行控制函数
    if len(mc) == 2:    
        threading.Thread(target=cobot_1_runing, args=[mc[0]]).start()
        time.sleep(wait_time)
        threading.Thread(target=cobot_2_runing, args=[mc[1]]).start()
    else:
        for cobot in mc:
            threading.Thread(target=cobot_1_runing, args=[cobot]).start()
            time.sleep(wait_time)

def paly_robot_Asynchronous():                                  #异步运行函数
    global wait_time
    for cobot in mc:
        threading.Thread(target=robot_init, args=[cobot]).start()
    time.sleep(6)
    Threading_cobot(mc)

def play_robot_Synchronous():                                   #同步运行函数
    for cobot in mc:
        threading.Thread(target=robot_init, args=[cobot]).start()
    time.sleep(6)
    for cobot in mc:
        threading.Thread(target=cobot_runing, args=[cobot]).start()
  
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

def Get_Coords_parameters(mc, offset):                       #判断坐标误差范围是否符合
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

                        
def Get_Angles_parameters(mc, offset):                       #判断角度误差范围是否符合
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
    setup()
    print(mc)
    try:
        # play_robot_Synchronous()                            #同步执行，不启动电机
        paly_robot_Asynchronous()                           #异步执行，启动电机
        # motor_control(mc[0])                                #io测试
    except Exception as e:
        print(e)
