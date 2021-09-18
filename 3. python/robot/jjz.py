import time
import os
import sys
from pymycobot.mycobot import MyCobot
from port_setup import setup
import RPi.GPIO as GPIO


Speed = 30
mode = 1

xxq = [ [0.01, 0.01, 0.01, 0.01, -85.01, -26.1],
        [0.01, 45.82, 0.0, -0.02, -40.24, -26.1],
        [0.01, 0.01, 0.01, 0.01, -85.01, -26.1],
        [85.01, 0.01, 0.01, 0.01, -85.01, -26.1],
        [85.01, 45.82, 0.0, -0.02, -40.24, -26.1],
        [85.01, 0.01, 0.01, 0.01, -85.01, -26.1]]

pcba = [[  0.01,  0.01, 0.01,  0.01, -85.01, -26.1],
        [ -1.58, 83.49, -45.52, -0.7, -44.29, -25.75],
        [-0.79, 52.91, -22.06, -1.84, -47.98, -25.66],
        [-85.01, 52.91, -22.06, -1.84, -47.98, -25.66],
        [-85.01, 83.49, -45.52, -0.7, -44.29, -25.75],
        [-85.01,  0.01, 0.01,  0.01, -85.01, -26.1]]


io_control = [[-0.52, -82.79, 62.66, -1.31, -32.16, -30.23],
              [0.01, -41.13, -15.01, 0.06, -50.97, -30.23],
              [0.7, -45.43, -13.88, -50.8, -68.29, -4.04],
              [0.01, -41.13, -15.01, 0.06, -50.97, -30.23],
              [0.7, -45.43, -13.88, 50.8, -68.29, -48.04],
              [40.7, -45.43, -13.88, 50.8, -68.29, -48.04],
              [90.01, -41.13, -15.01, 0.06, -50.97, -30.23],
              [88.5, 35.59, 18.01, -5.27, 62.22, -25.66],
              [88.76, 30.32, -79.89, 4.57, -45.35, -31.28],
              [0.01, -41.13, -15.01, 0.06, -50.97, -30.23]
              ]

start = [[-0.52, -82.79, 62.66, -1.31, -32.16, -30.23],
         [ 0, 0, -40, 0, -40, 0],
         [-90.64, 37.08, 20.91, -0.17, -19.24, 21.09],         
         [-87.71, -45.08, -177.27, 0.55, 58.62, 142.55],
         [-87.71, 45.08, -177.27, 0.55, 58.62, 142.55],
         [85.71, 45.08, -177.27, 0.55, 58.62, 142.55],
         [90.64, 37.08, 20.91, -0.17, -19.24, 21.09],         
         [ 0, 0, -40, 0, -40, 0],
         [-0.52, -82.79, 62.66, -1.31, -32.16, -30.23]
         ]


def test(mycobot):
    print("\nStart check basic options\n")
    time.sleep(3)
    angles = [0, 0, 0, 0, 0, -26.0]
    mycobot.send_angles(angles, 100)
    time.sleep(3)
    for i in range(1,6):
        mycobot.send_angles(pcba[i], 100)
        time.sleep(3)
        if i == 1:
            pump_state(0)
            time.sleep(2)
        if i == 4:
            pump_state(1)
            time.sleep(2)
    mycobot.send_angles(angles, 100)
   
   
def io_control_ai():
    print("\nIO _ CONTROL\n")
    mycobot.send_angles(io_control[0], 30)
    time.sleep(3)
    mycobot.send_angles(io_control[1], 50)
    time.sleep(3)
    mycobot.send_angles(io_control[2], 70)
    time.sleep(1.5)
    mycobot.send_angles(io_control[3], 70)
    time.sleep(1.5)
    mycobot.send_angles(io_control[4], 40)
    time.sleep(0.8)
    mycobot.send_angles(io_control[5], 40)
    time.sleep(1.0)
    mycobot.send_angles(io_control[6], 70)
    time.sleep(3.0)
    mycobot.send_angles(io_control[7], 30)
    time.sleep(3.0)
    mycobot.send_angles(io_control[8], 50)
    time.sleep(3.0)
    mycobot.send_angles(io_control[9], 20)
    time.sleep(4.0)
    mycobot.send_angles(io_control[0], 20)
    time.sleep(3)

            
def gpio_read(input_pin, output_pin):
    GPIO.setmode(GPIO.BCM)          #将GPIO引脚设置为BCM引脚方式
    GPIO.setup(input_pin,GPIO.IN,pull_up_down=GPIO.PUD_UP)
    GPIO.setup(output_pin,GPIO.OUT)
    GPIO.setup(output_pin,1)
    while 1:
        if GPIO.input(input_pin):
            print('Input is High')
            GPIO.setup(output_pin,1)
        else:
            print('Input is Low')
            GPIO.setup(output_pin,0)
            io_control_ai()
            break
        time.sleep(1)
    GPIO.cleanup()

def start_show():
    print("\START_SHOW\n")
    for i in range(len(start)):
        mycobot.send_angles(start[i], 50)
        time.sleep(3)


def init_gpio(io_number, state):
    GPIO.setmode(GPIO.BCM)
    if state == 0:
        st = GPIO.OUT
    elif state == 1:
        st = GPIO.IN
    GPIO.setup(io_number, st)
        
        
def pump_state(state):
    init_gpio(20, 0)
    init_gpio(21, 0)
    GPIO.output(20, state)
    GPIO.output(21, state)        
        
        
def set_rectangle(mycobot):
    angles = [0, 0, 0, 0, -90, 0]
    mycobot.send_angles(angles, 100)
    time.sleep(3)
#     set_line_runing(mycobot, 0, 5)
#     time.sleep(3)
    set_line_runing(mycobot, 1, 2)
    time.sleep(3)
    set_line_runing(mycobot, 0, -5)
    time.sleep(3)
    set_line_runing(mycobot, 1, -5)
    time.sleep(3)
    set_line_runing(mycobot, 0, 5)
    time.sleep(3)
    set_line_runing(mycobot, 1, 2)
    time.sleep(3)
    mycobot.send_angles(angles, 100)
    time.sleep(3)
        
        
def set_line_runing(mycobot, axis, step):                              #控制末端做直线移动，cobot是实例化后的串口指向，axis是轴向：X,Y,Z, step是步数，步长是10mm。
    pos_1 = mycobot.get_coords()
    time.sleep(0.5)
    pos_1 = mycobot.get_coords()
    time.sleep(0.5)
    if step > 0:
        for _ in range(step):
            pos_1[axis] += 10
            mycobot.send_coords(pos_1, Speed, mode)
            print(pos_1)
            time.sleep(0.50)
    else:
        step = 0 - step
        for _ in range(step):
            pos_1[axis] -= 10
            mycobot.send_coords(pos_1, Speed, mode)
            print(pos_1)
            time.sleep(0.50)
    time.sleep(2)
    

if __name__ == "__main__":

#     mycobot = setup()
    mycobot = MyCobot('/dev/ttyAMA0', 1000000)
    start_show()
    test(mycobot)
    io_control_ai()
#     set_rectangle(mycobot)

    # gpio_read(21, 20)
