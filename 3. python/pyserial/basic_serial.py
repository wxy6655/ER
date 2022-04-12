from asyncio.windows_events import NULL
import serial #导入模块
import time
import serial.tools.list_ports
import struct

PORT = ''
BAUD = ''
mySerial = []

def setup():                                                    #选择正确的串口
    global PORT,BAUD
    print("")

    plist = list(serial.tools.list_ports.comports())
    idx = 0
    for port in plist:
        print("{} : {}".format(idx, port))
        idx += 1
    if idx == 0:
        print("The connected device was not detected. Please try reconnecting.")
        exit(1)
    _in = input("\nPlease input 0 - {} to choice':".format(idx))
    idxes = _in.split(',')
    try:
        idxes = [int(i) for i in idxes]
    except Exception:
        print('Error: Input format error.')
        exit(1)

    # PORT = [str(plist[i]).split(' - ')[0].strip() for i in idxes]
    PORT = str(plist[0]).split(' - ')[0].strip()
    print(PORT)
    print("")

    BAUD = '115200'
    _BAUD = input("Please input BAUD(default:115200):")
    try:
        BAUD = int(_BAUD)
    except Exception as _e:
        print(_e)
        pass
    print(BAUD)
    print("")

def Openseial(_PORT,_BAUD):
    global mySerial
    try:
        _mySerial = serial.Serial(_PORT, _BAUD, bytesize=8, parity='N', stopbits=1, timeout=0.1, xonxoff=0, rtscts=1)
        if _mySerial.isOpen():
            print(_mySerial)
    except Exception as e:
        print("---error---：",e)
        exit(1)
    mySerial = _mySerial

def _Closeserial(_mySerial):
    _mySerial.close()

def Flushserial(_mySerial):
    _mySerial.flush()




class DataProcessor(object):
    # Functional approach
    def _encode_int8(self, data):
        return struct.pack("b", data)

    def _encode_int16(self, data):
        if isinstance(data, int):
            return list(struct.pack(">h", data))
        else:
            return sum([list(struct.pack('>h', elem)) for elem in data], [])

    def _decode_int8(self, data):
        return struct.unpack("b", data)[0]

    def _decode_int16(self, data):
        return struct.unpack(">h", data)[0]

    def _angle2int(self, angle):
        return int(angle * 100)

    def _coord2int(self, coord):
        return int(coord * 10)

    def _int2angle(self, _int):
        return round(_int / 100.0, 3)

    def _int2coord(self, _int):
        return round(_int / 10.0, 2)

    def _flatten(self, _list):
        return sum(
            ([x] if not isinstance(x, list) else self._flatten(x)
             for x in _list), []
        )

    def _process_data_command(self, args):
        if not args:
            return []

        return self._flatten(
            [
                [self._encode_int16(int(i))
                 for i in x] if isinstance(x, list) else x
                for x in args
            ]
        )

    def _is_frame_header(self, data, pos):
        return data[pos] == ProtocolCode.HEADER and data[pos + 1] == ProtocolCode.HEADER

    def _process_received(self, data, genre):
        if not data:
            return []

        data = bytearray(data)
        data_len = len(data)
        # Get valid header: 0xfe0xfe
        for idx in range(data_len - 1):
            if self._is_frame_header(data, idx):
                data_len = data[idx + 2] - 2
                if data_len > 0:
                    break
        else:
            return []

        # compare send header and received header
        cmd_id = data[idx + 3]
        if cmd_id != genre:
            return []
        data_pos = idx + 4
        valid_data = data[data_pos: data_pos + data_len]

        # process valid data
        res = []
        if data_len == 12 or data_len == 8:
            for idx in range(0, len(valid_data), 2):
                one = valid_data[idx: idx + 2]
                res.append(self._decode_int16(one))
        elif data_len == 2:
            if genre in [ProtocolCode.IS_SERVO_ENABLE]:
                return [self._decode_int8(valid_data[1:2])]
            res.append(self._decode_int16(valid_data))
        else:
            res.append(self._decode_int8(valid_data))
        return res

    def _process_single(self, data):
        return data[0] if data else -1

def Readserial(_mySerial):
    if _mySerial.inWaiting() > 0:
        data = _mySerial.read(_mySerial.inWaiting())
        print("_read: {}".format(data))
    else:
        print("_read: no data can be read")
        data = None
    return data

def Wriaterial(_mySerial,data):
     #十六进制的发送
    result = NULL
    for _data in data:
        _mySerial.write(chr(_data).encode("utf-8"))#写数据
        result += 1
    print("write sum:",result)



setup()
Openseial(PORT,115200)
try:
    # while(True):
    Wriaterial(mySerial,[0XBA,0XAA])
    Readserial(mySerial)
    _Closeserial(mySerial)
except Exception as _e:
        print('Error: ',_e)
        exit(1)