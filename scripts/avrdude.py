# avrdude flash helper

import os
import re
import shlex
import subprocess
from time import time, sleep

from argparse import ArgumentParser

import serial

USB_RESET_REQUIRED = ['atmega32u4']

BOARDS = {
    'mega': {
        'mcu': 'atmega2560',
        'programmer': 'wiring',
        'usb_reset': False
    },
    'uno': {
        'mcu': 'atmega328p',
        'programmer': 'arduino',
        'usb_reset': False
    },
    'promicro': {
        'mcu': 'atmega32u4',
        'programmer': 'avr109',
        'usb_reset': True
    }
}

def main(args):
    hex_file = find_matching_hex_file(args["project"], args["mcu"], args["search_root"])

    if not hex_file:
        print('No hex file matches the criteria')
        return

    try:
        if args['mcu'] in USB_RESET_REQUIRED and not args['user_override']:
            # Some boards need to be reset to enter the bootloader
            flash_method_usb_reset(args, hex_file)
        else:
            flash_method_normal(args, hex_file)
    except Exception as e:
        print(f'Failed to flash device: {e}')

def flash_method_usb_reset(args, hex_file):
    # This flash method will reset the USB device first
    # This causes the mcu to enter the bootloader
    # However, it will come up as a new serial port (NOT the specified target port)
    # The new port will be detected and assigned when running the flash command

    target_port = args['port']

    initial_ports = list_serial_ports()

    # sanity check that the target port is among the initial ports
    if target_port not in initial_ports:
        raise RuntimeError('The target port does not exist!')

    # Reset the usb device
    usb_reset(target_port)
    # Detect when the new port
    new_port = detect_new_serial_port(initial_ports)
    print(f'Detected new port: {new_port}')
    args.update({'port': new_port})
    # Build the flash command with the updated port
    cmd = build_cmd(args, hex_file)
    # Flash the device
    run_cmd(cmd)

def flash_method_normal(args, hex_file):
    cmd = build_cmd(args, hex_file)
    run_cmd(cmd)

def list_serial_ports():
    ports = set([])
    serial_ports_iterator = get_serial_port_iterator()()
    for (port, _, _) in serial_ports_iterator:
        ports.add(port)

    return ports

def detect_new_serial_port(initial_ports, timeout=60.0, wait_interval=0.10):
    c_time = time()
    end_time = c_time + timeout
    while c_time < end_time:
        ports = list_serial_ports() - initial_ports

        if ports:
            new_port = list(ports)[0]
            return new_port

        sleep(wait_interval)
        c_time = time()

    return None

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

def run_cmd(cmd):
    print(f'{cmd}')
    avrdude_process = subprocess.Popen(shlex.split(cmd))
    return avrdude_process.communicate()

def find_matching_hex_file(project_name, mcu, search_root):
    for parent, dirs, files in os.walk(search_root):
        for f in files:
            if re.match(f'{project_name}-{mcu}.*\.hex', f):
                return os.path.join(parent, f)
    return None

def build_cmd(args, hex_file):
    mcu = args['mcu']
    programmer = args['programmer']
    port = args['port']
    baud = args['baud']

    return f'avrdude -v -p{mcu} -c{programmer} -P{port} -b{baud} -D -U flash:w:"{hex_file}"'

def usb_reset(port):
    try:
        # open the port at 1200 and set DTR to false
        # https://github.com/arduino/Arduino/blob/master/arduino-core/src/processing/app/Serial.java#L101-L104
        with serial.Serial(port=port, baudrate=1200, stopbits=serial.STOPBITS_ONE, parity=serial.PARITY_NONE) as ser:
            ser.setDTR(False)
    except Exception as e:
        print(f'Error resetting USB device: {e}')


def process_args(args):
    out_args = dict(args)

    if args['board'] and (args['mcu'] or args['programmer']):
        raise RuntimeError('Specific either board or mcu, not both')

    if args['board']:
        board = args['board']
        try:
            config = BOARDS[board]
            out_args.update(config)
        except KeyError:
            raise RuntimeError(f'Could not find configuration for {board}')
    elif not args['mcu'] and not args['programmer']:
        raise RuntimeError('Both mcu and programmer must be specified together')

    return out_args

if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument('project')
    parser.add_argument('-B', '--board', required=False, choices=list(BOARDS.keys()), help='Dev board type')
    parser.add_argument('-m', '--mcu', required=False, help='processor')
    parser.add_argument('-c', '--programmer', required=False, help='Programmer type')
    parser.add_argument('-p', '--port', required=True, help='Serial port')
    parser.add_argument('-b', '--baud', default=115200, type=int, help='Baud rate')
    parser.add_argument('-s', '--search-root', default='.', help='Starting search path to find hex files')
    parser.add_argument('-o', '--user-override', default=False, action='store_true', help='Auto reset override')

    args = vars(parser.parse_args())
    args = process_args(args)

    main(args)
