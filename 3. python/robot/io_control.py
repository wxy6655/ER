#！/usr/bin/python
import RPi.GPIO as GPIO     #导入GPIO
import time
from pymycobot.mycobot import MyCobot

# GPIO.setmode(GPIO.BOARD) #将GPIO引脚设置为BOARD模式



# GPIO.setup(input_pin,GPIO.IN)  #设置GPIO引脚通道 作为输入
# GPIO.setup(input_pin,GPIO.OUT) #作为输出
# GPIO.setup(input_pin,GPIO.OUT,initial=GPIO.HIGH) #设置初始化为高电平

#多通道设置方式
# input_pinlist=[input_pin1,input_pin2,input_pin3,input_pinx]
# GPIO.setup(input_pinlist,GPIO.OUT)
# GPIO.setup(input_pinlist,GPIO.IN)




input_pin = 20
# GPIO.setup(input_pin,GPIO.IN,pull_up_down=GPIO.PUD_DOWN)
mc = MyCobot('/dev/ttyAMA0', 1000000)

def __init__():
    global input_pin
    
    GPIO.setmode(GPIO.BCM)           #将GPIO引脚设置为BCM引脚方式
    GPIO.setup(input_pin,GPIO.IN,pull_up_down=GPIO.PUD_UP)


def io_read():
    if GPIO.input(input_pin):
        print('Input is High')
        return 0
    else:
        print('Input is Low')
        return 1

def runing():
    while 1:
        if io_read():
            mc.send_angles([0,0,0,0,0,0], 30)
        else:
                print('no find input')
        time.sleep(1)

if __name__ == "__main__":
    __init__()
    runing()
