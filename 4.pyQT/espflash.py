from esptool import main as esptool
import os

command = ['--chip', 'esp32', '--port', '', '--baud', '',
            '--before', 'default_reset', '--after', 'hard_reset', 'write_flash', '-z', '--flash_mode', 'dio', '--flash_freq', '80m', '--flash_size', 'detect',
            '0xe000', '',
            '0x1000', '',
            '0x10000', '',
            '0x8000', '']

path = os.getcwd()
print('')
port_number = input("plase input serial port mumber!")
print('')
baud = input("plase input serial port buad!")
print('')
file_name = input("plase input file _name!")

_command = command.copy()
_command[3]  = str(port_number)
_command[5]  = baud
_command[-7] = path + '/boot/boot_app0.bin'
_command[-5] = path + '/boot/bootloader_qio_80m.bin'
_command[-3] = path + '/ino/' + file_name +'.bin'
_command[-1] = path + '/boot/default.bin'


while True:
    num = 0
    try:
        esptool(_command)
        break
    except Exception as a:
        if num > 20:
            print(a)
            break
        else:
            num+=1
            time.sleep(0.02)
            continue