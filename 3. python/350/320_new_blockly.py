from pymycobot.mycobot import MyCobot
import time


mc = MyCobot('COM23',115200)
mc.power_on()
time.sleep(2)
mc.send_angles([(-0.08),(-0.17),(-0.17),0.17,0.08,0.17],46)
time.sleep(2)
mc.send_angles([(-0.08),(-44.12),(-90.87),44.82,89.73,0.17],47)
time.sleep(2)
mc.send_coords([173.5,(-88.3),124.6,170,(-0.17),(-90.26)],45,1)
time.sleep(2)
mc.send_coords([173.5,(-188.3),124.6,170,(-0.17),(-90.26)],45,1)
time.sleep(2)