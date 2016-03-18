import logging
import itertools
import platform
import serial
import time
from serial.tools import list_ports
if platform.system() == 'Windows':
    import _winreg as winreg
else:
    import glob



"""

/*
 * Original: https://github.com/thearn/Python-Arduino-Command-API
 * Modified to remove functions un-needed for this project
 */


LICENSE:

Copyright (c) 2012-2013 Tristan A. Hearn <tristanhearn@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

"""



log = logging.getLogger(__name__)

def enumerate_serial_ports():
    """
    Uses the Win32 registry to return a iterator of serial
        (COM) ports existing on this computer.
    """
    path = 'HARDWARE\\DEVICEMAP\\SERIALCOMM'
    try:
        key = winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, path)
    except WindowsError:
        raise Exception
  
    for i in itertools.count():
        try:
            val = winreg.EnumValue(key, i)
            yield (str(val[1]))  # , str(val[0]))
        except EnvironmentError:
            break


def build_cmd_str(cmd, args=None):
    """
    Build a command string that can be sent to the arduino.

    Input:
        cmd (str): the command to send to the arduino, must not
            contain a % character
        args (iterable): the arguments to send to the command

    @TODO: a strategy is needed to escape % characters in the args
    """
    if args:
        args = '%'.join(map(str, args))
    else:
        args = ''
    return "@{cmd}%{args}$!".format(cmd=cmd, args=args)


def find_port(baud, timeout):
    """
    Find the first port that is connected to an arduino with a compatible
    sketch installed.
    """
    if platform.system() == 'Windows':
        ports = enumerate_serial_ports()
    elif platform.system() == 'Darwin':
        ports = [i[0] for i in list_ports.comports()]
    else:
        ports = glob.glob("/dev/ttyUSB*") + glob.glob("/dev/ttyACM*")
    for p in ports:
        log.debug('Found {0}, testing...'.format(p))
        try:
            sr = serial.Serial(p, baud, timeout=timeout)
        except (serial.serialutil.SerialException, OSError) as e:
            log.debug(str(e))
            continue
        time.sleep(2)
        version = get_version(sr)
        if version != 'version':
            log.debug('Bad version {0}. This is not a Shrimp/Arduino!'.format(
                version))
            sr.close()
            continue
        log.info('Using port {0}.'.format(p))
        if sr:
            return sr
    return None


def get_version(sr):
    cmd_str = build_cmd_str("version")
    try:
        sr.write(cmd_str)
        sr.flush()
    except Exception:
        return None
    return sr.readline().replace("\r\n", "")


class Arduino(object):

    def __init__(self, baud=9600, port=None, timeout=2, sr=None):
        """
        Initializes serial communication with Arduino if no connection is
        given. Attempts to self-select COM port, if not specified.
        """
        if not sr:
            if not port:
                sr = find_port(baud, timeout)
                if not sr:
                    raise ValueError("Could not find port.")
            else:
                sr = serial.Serial(port, baud, timeout=timeout)
        sr.flush()
        self.sr = sr

    def version(self):
        return get_version(self.sr)


    def close(self):
        if self.sr.isOpen():
            self.sr.flush()
            self.sr.close()

    def do(self, cmd, data):
        """
        Returns the value of a specified
        digital pin.
        inputs:
           pin : digital pin number for measurement
        returns:
           value: 0 for "LOW", 1 for "HIGH"
        """

        cmd_str = build_cmd_str(cmd, data)
        try:
            self.sr.write(cmd_str)
            self.sr.flush()
        except:
        	print "Command error: arduino.py"
        	pass
        rd = self.sr.readline().replace("\r\n", "")
        try:
            return int(rd)
        except:
            pass
        return 1
