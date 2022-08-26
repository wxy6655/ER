from re import A
import time
import threading
from pymycobot.mycobot import MyCobot
import serial.tools.list_ports
import numpy as np
import os

mc = []
ports = []

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
        self.recording = False
        self.recording_L = False
        self.recording_R = False
        self.playing = False
        self.playing_L = False
        self.playing_R = False
        self.record_list_L = []
        self.record_list_R = []
        self.record_t = None
        self.record_t_L = None
        self.record_t_R = None
        self.play_t = None
        self.play_t_L = None
        self.play_t_R = None

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


    def record_L(self, cobot,):
        self.record_list_L = []
        self.recording_L = True

        def _record_L():
            start_t = time.time()

            while self.recording_L:
                angles = cobot.get_angles()
                if angles:
                    self.record_list_L.append(angles)
                    time.sleep(0.1)
                    print("\r {}".format(time.time() - start_t), end="")

        self.echo("Start recording_L.")
        self.record_t_L = threading.Thread(target=_record_L, daemon=True)
        self.record_t_L.start()

    def stop_record_L(self):
        if self.recording_L:
            self.recording_L = False
            self.record_t_L.join()
            self.echo("Stop record_l")

    def play_L(self, cobot):
        self.echo("Start play")
        for angles in self.record_list_L:
            print("l : ", angles)
            cobot.send_angles(angles, 30)
            time.sleep(0.1)
        self.echo("Finish play")

    def record_R(self, cobot):
        self.record_list_R = []
        self.recording_R = True

        def _record_R():
            start_t = time.time()

            while self.recording_R:
                angles = cobot.get_angles()
                if angles:
                    self.record_list_R.append(angles)
                    time.sleep(0.1)
                    print("\r {}".format(time.time() - start_t), end="")

        self.echo("Start recording_R.")
        self.record_t_R = threading.Thread(target=_record_R, daemon=True)
        self.record_t_R.start()

    def stop_record_R(self):
        if self.recording_R:
            self.recording_R = False
            self.record_t_R.join()
            self.echo("Stop record_r")

    def play_R(self, cobot):
        self.echo("Start play")
        for angles in self.record_list_R:
            print("r : ", angles)
            cobot.send_angles(angles, 30)
            time.sleep(0.1)
        self.echo("Finish play")

    def record(self, cobot1, cobot2):
        self.record_list_L = []
        self.record_list_R = []

        self.recording = True

        def _record():
            start_t = time.time()

            while self.recording:
                angles1 = cobot1.get_angles()
                angles2 = cobot2.get_angles()
                print("all : ", angles1 , angles2)
                if angles1:
                    if angles2:
                        self.record_list_L.append(angles1)
                        self.record_list_R.append(angles2)
                        #time.sleep(0.05)
                        print("\r {}".format(time.time() - start_t), end="")

        self.echo("Start recording.")
        self.record_t = threading.Thread(target=_record, daemon=True)
        self.record_t.start()

    def stop_record(self):
        if self.recording:
            self.recording = False
            self.record_t.join()
            self.echo("Stop record_all")

    def play(self, cobot1, cobot2):
        self.echo("Start play")
        for i in range(len(self.record_list_L)):
            angles1 = self.record_list_L[i]
            angles2 = self.record_list_R[i]
            print("all : ", angles1 , angles2)
            if angles1:
                if angles2:
                    print("all : ", angles1 , angles2)
                    cobot1.send_angles(angles1, 40)
                    time.sleep(0.1)
                    cobot2.send_angles(angles2, 40)
            time.sleep(0.1)
        self.echo("Finish play")

   


    def paly_robot_Asynchronous(self):                                  #异步运行函数
        global wait_time
        for cobot in mc:
            threading.Thread(target=self.robot_init, args=[cobot]).start()
        time.sleep(5)
        mc[0].release_all_servos()
        mc[1].release_all_servos()
        self.echo("Released")
        time.sleep(2)
        # threading.Thread(target=self.record, args=[mc[0],mc[1]]).start()
        self.record(mc[0], mc[1])
        # self.record_L(mc[0])
        # self.record_R(mc[1])
        time.sleep(10)
        self.stop_record()
        # threading.Thread(target=self.stop_record_R, ).start()
        # threading.Thread(target=self.stop_record_R, ).start()
        # self.stop_record_L()
        # self.stop_record_R()
        time.sleep(5)
        # threading.Thread(target=self.play_L, args=[mc[0]]).start()
        # threading.Thread(target=self.play_R, args=[mc[1]]).start()
        self.play(mc[0], mc[1])
        time.sleep(10)

        # for cobot in mc:
        #     threading.Thread(target=self.record, args=[cobot]).start()
        # time.sleep(10)

        # for cobot in mc:
        #     threading.Thread(target=self.stop_record, args=[cobot]).start()
        # time.sleep(10)

        # for cobot in mc:
        #     threading.Thread(target=self.play_t, args=[cobot]).start()
        # time.sleep(10)

    

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
    setup()
    print(mc)
    t = TeachingTest(mc)
    
    try:
        t.paly_robot_Asynchronous()                           #异步执行
    except Exception as e:
        print(e)
