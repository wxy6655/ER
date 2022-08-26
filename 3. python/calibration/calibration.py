from re import A
import time
from pymycobot.mycobot import MyCobot

mc = MyCobot('COM3',115200)

for i in range(1,7):

    mc.set_servo_calibration(i)
    time.sleep(1)
# while True:
#     time.sleep(0.1)
#     mc.set_servo_calibration(1)
#     time.sleep(0.1)
#     print(mc.get_encoder(1))