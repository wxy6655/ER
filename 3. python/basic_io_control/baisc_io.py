from pymycobot.mycobot import MyCobot
import time
PORT = 'COM16'    #修改为设备连接串口
BAUD = 115200
# 初始化一个MyCobot对象
mc = MyCobot(PORT, BAUD)

# 开启
def grip_on():
    mc.set_basic_output(2, 0)
    mc.set_basic_output(5, 0)

# 停止
def grip_off():
    mc.set_basic_output(2, 1)
    mc.set_basic_output(5, 1)


grip_off()
time.sleep(3)
grip_on()
time.sleep(3)
grip_off()
time.sleep(3)