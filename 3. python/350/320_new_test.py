import time
from pymycobot.mycobot import MyCobot

mc = MyCobot('COM23',115200)
time.sleep(2)
print("plase hold robot")
time.sleep(2)
mc.release_all_servos()
time.sleep(1)


while True:
    print("angles: ", mc.get_angles())
    time.sleep(0.2)
    print("coords: ", mc.get_coords())
    time.sleep(0.2)
