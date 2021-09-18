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

#定义末端执行器电机控制引脚为23，VCC接23，GND接GND
motor_io = 23

#定义末端执行器电机控制引脚输出频率，范围1000-30000；
motor_frequency = 10000

#定义末端执行器电机启动速度为0，不可调节
start_motor_speed = 0

#定义末端执行器电机最大速度为80%，范围0-10
motor_speed = 8

#定义末端执行器电机最大速度运行时间
motor_run_time = 2


def detected_port():                                            #检测链接设备数量
    port_list = list(serial.tools.list_ports.comports())
    for i in range(len(port_list)):
        ports.append(port_list[i][0])
    print(ports)

def set_line_runing(cobot, axis, step):                         #控制末端做直线移动，cobot是实例化后的串口指向，axis是轴向：X,Y,Z, step是步数，步长是10mm。
    pos_1 = cobot.get_coords()
    time.sleep(0.05)
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
    cobot.send_angles([0,0,0,0,0,0], 50)
    print("init")
    time.sleep(0.50)
    cobot.send_angles([0,0,0,0,0,0], 50)
    print("init")
    time.sleep(0.50)
    cobot.send_angles([0,0,0,0,0,0], 50)
    print("init")
    time.sleep(0.50)
    cobot.set_color(0, 255, 0)
    time.sleep(3)
    print(cobot.get_angles())
    time.sleep(1)
    cobot.send_coords(Coord_int_1, 50, 2)
    time.sleep(3)
    cobot.set_color(255, 0, 0)
    time.sleep(0.50)

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
    set_line_runing(cobot, Z, Z_data_1)
    time.sleep(1)
    set_line_runing(cobot, Y, Y_data_1)
    time.sleep(1)
    cobot.set_color(255, 0, 0)
    time.sleep(0.50)

def motor_control(cobot):                                       #电机控制函数，变频启动与关闭
    global start_motor_speed, motor_io, motor_frequency
    cobot.set_pin_mode(23, 1)
    time.sleep(0.1)
    cobot.set_pin_mode(19, 1)
    time.sleep(0.1)
    cobot.set_digital_output(23, 0)
    time.sleep(0.1)
    cobot.set_digital_output(19, 0)
    time.sleep(0.1)
    print("servo motor start")
    # for _ in range(motor_speed):
    #     cobot.set_pwm_output(motor_io, motor_frequency, start_motor_speed)
    #     start_motor_speed += 10
    #     time.sleep(0.1)
    # time.sleep(motor_run_time)
    cobot.set_digital_output(23, 1)
    time.sleep(0.1)
    cobot.set_digital_output(19, 1)
    time.sleep(3)
    cobot.set_digital_output(23, 0)
    time.sleep(0.1)
    cobot.set_digital_output(19, 0)
    time.sleep(0.1)
    # for _ in range(motor_speed):
    #     cobot.set_pwm_output(motor_io, motor_frequency, start_motor_speed)
    #     start_motor_speed -= 10
    #     time.sleep(0.1)
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

if __name__ == "__main__":                                      #主函数
    setup()
    print(mc)
    time.sleep(1)
    # play_robot_Synchronous()                            #同步执行，不启动电机
    paly_robot_Asynchronous()                           #异步执行，启动电机
    # motor_control(mc[0])                                #io测试
