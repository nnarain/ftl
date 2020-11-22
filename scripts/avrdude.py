# avrdude flash helper

import os
import re
import shlex
import subprocess

from argparse import ArgumentParser

def main(args):
    hex_file = find_matching_hex_file(args.project, args.mcu, args.search_root)

    cmd = build_cmd(args, hex_file)
    run_cmd(cmd)

def run_cmd(cmd):
    avrdude_process = subprocess.Popen(shlex.split(cmd))
    return avrdude_process.communicate()

def find_matching_hex_file(project_name, mcu, search_root):
    for parent, dirs, files in os.walk(search_root):
        for f in files:
            if re.match(f'{project_name}-{mcu}.*\.hex', f):
                return os.path.join(parent, f)
    return None

def build_cmd(args, hex_file):
    mcu = args.mcu
    programmer = args.programmer
    port = args.port
    baud = args.baud

    return f'avrdude -v -p{mcu} -c{programmer} -P{port} -b{baud} -D -U flash:w:"{hex_file}"'

if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument('project')
    parser.add_argument('-m', '--mcu', required=True, help='processor')
    parser.add_argument('-c', '--programmer', default='wiring', help='Programmer type')
    parser.add_argument('-p', '--port', required=True, help='Serial port')
    parser.add_argument('-b', '--baud', default=115200, type=int, help='Baud rate')
    parser.add_argument('-s', '--search-root', default='.', help='Starting search path to find hex files')

    args = parser.parse_args()

    main(args)
