
from pymycobot.mycobot import MyCobot
import time
mc = None

def callback():
    global mc
    # rospy.loginfo(rospy.get_caller_id() + "%s", data.position)
    data_list = [-2.522929436945715, 2.4219357614604657, 0.5093358059806676, -2.2708235605581315, -0.9862997921804783, 2.000297199627284]

    mc.send_radians(data_list, 80)
    print(data_list)
    # time.sleep(0.5)


def listener():
    global mc
    port = "COM23"
    baud = 115200
    print(port, baud)
    mc = MyCobot(port, baud)
    time.sleep(2)
    print(mc.get_radians())
    # spin() simply keeps python from exiting until this node is stopped

if __name__ == "__main__":
    listener()
    callback()