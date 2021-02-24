import os
import serial

def get_serial_port_iterator():
    try:
        if os.name == 'nt':
            from serial.tools.list_ports_windows import comports
            return comports
        elif os.name == 'posix':
            from serial.tools.list_ports_posix import comports
            return comports
        else:
            raise ImportError('No serial implementation for this platform found')
    except ImportError as e:
        return None

for (port, _, _) in get_serial_port_iterator()():
    print(port)
