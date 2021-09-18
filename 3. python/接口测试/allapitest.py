import time
from pymycobot.mycobot import MyCobot

data_id = [21, 22, 23, 24, 26, 27]
data    = [10, 0, 1, 0, 3, 3]

mc = MyCobot('/dev/ttyAMA0', 1000000)
# mc = MyCobot('COM27', 115200)

print("Open communication")
mc.power_on()
time.sleep(2)

print("Communication state : " + str(mc.is_power_on()))
time.sleep(2)

print("Atom Led Color Test")
mc.set_color(0,0,255)
time.sleep(2)
mc.set_color(255,0,0)
time.sleep(2)
mc.set_color(0,255,0)
time.sleep(2)

print("SetEncoder to 2048 for every Servo motor")
for i in range(1,7):
    mc.set_encoder(i,2048)
    time.sleep(0.2)
time.sleep(5)

