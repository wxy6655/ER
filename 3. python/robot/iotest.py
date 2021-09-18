import RPi.GPIO as GPIO
import time
import threading
import json
state = 0

def init_gpio(io_number, state):
    GPIO.setmode(GPIO.BCM)
    if state == 0:
        st = GPIO.OUT
    elif state == 1:
        st = GPIO.IN
    GPIO.setup(io_number, st)
    
    
def gpio_read(io_number):
    init_gpio(io_number, 1)
    time_1 = time.time()
    print("read state")
    while True:
        print(GPIO.input(io_number))
        time.sleep(0.2)
        time_2 = time.time() - time_1
        if time_2 >10:
            print("read state end")
            break
        
        
def gpio_send(io_number):
    init_gpio(io_number, 0)
    time_1 = time.time()
    print("out 0/1 by 1s")
    while True:
        GPIO.output(io_number, GPIO.HIGH)
        time.sleep(1)
        GPIO.output(io_number, GPIO.LOW)
        time.sleep(1)
        time_2 = time.time() - time_1
        if time_2 >10:
            print("out end")
            break
    GPIO.output(io_number, GPIO.HIGH)
    time.sleep(1)
    
def pump_state(state):
    init_gpio(20, 0)
    init_gpio(21, 0)
    GPIO.output(20, state)
    GPIO.output(21, state)

def main():
    while True:
        _in = input("input number and state，like ‘19,0’:")
        if _in == ' ':
            print("exit")
            exit(0)
        try:
            number, state = _in.split(',')
        except Exception:
            print(_in)
            continue
        number, state = int(number.strip()), int(state.strip())
        
        if state == 1:
            gpio_read(number)
        else:
            gpio_send(number)
        

if __name__ == "__main__":
    main()
