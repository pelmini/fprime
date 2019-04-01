from __future__ import print_function
import sys
import time
import struct
import argparse
import serial

import gds_interface

class SerialTcp(object):
    """
    Serial to Tcp interface. Supplies two methods used read and write data back and forth from the interface. This
    naieve implementation is expected to run as part of two threads.
    """
    def __init__(self, device, baud, address, port):
        """
        Initialize the serial/Tcp device
        :param device: device path for use in pyserial
        :param baud: speed of the serial device
        :param address: address of the tcp server
        :param port: port of the tcp server
        """
        self.running = True
        self.device = device
        self.baud = baud
        self.gds_interface = gds_interface.TCPGDSInterface(address, port)
        self.gds_interface.register(self.to_uart)

    def open(self):
        """
        Opens the serial port and tcp socket
        """
        self.serial = serial.Serial(self.device, self.baud, timeout=0)
        self.gds_interface.open()
        print("[INFO] Opened socket and device.")

    def close(self):
        """
        Close the file handle
        """
        self.serial.close()
        self.gds_interface.close()

    def to_uart(self, data):
        """
        Handles the "to-uart" portion of the duplex communication. This sends data out the UART port that was taken
        directly off a socket read from the tcp server.
        :param data: data to send to the UART socket
        """
        sending = struct.pack(">I", 0xdeadbeef);
        sending += struct.pack(">I", len(data));
        sending += data
        print(" ".join(["{0:02x}".format(ord(byte)) for byte in data]))
        sending += struct.pack(">I", 0xfeebdaed);
        self.serial.write(sending)

    def from_uart(self):
        """
        Handles the "from-uart" portion of the duplex communcation. This sends data out the socket that was taken
        directly off the serial port read from the tcp server.
        """
        data = self.serial.read(1024)
        com = data[8:-4]
        if data:
            self.gds_interface.write(com)

    def run(self):
        """
        Run this method as part of the main loop of the program, or as part of a thread. It will handle the rest.
        """
        while self.running:
            self.from_uart()


def parse_args(args):
    """
    Parse the arguments to this application, then return the constructed namespace argument.
    :param args: list of arguments to parse
    :return: namespace argument
    """
    parser = argparse.ArgumentParser(description="Connects data from F prime flight software to the GDS tcp server")
    parser.add_argument("-a", "--addr", dest="addr", type=str, default="127.0.0.1",
                        help="Address of the tcp server. Default: %(default)s")
    parser.add_argument("-p", "--port", dest="port", type=int, default=50000,
                        help="Port tcp server is connected to. Default: %(default)s")
    parser.add_argument("-d", "--device", dest="device", type=str, default="/dev/ttyACM0",
                        help="UART device representing the FSW. Default: %(default)s")
    parser.add_argument("-b", "--baud", dest="baud", type=int, default=115200,
                        help="Baud rate of the serial device. Default: %(default)s")
    args = parser.parse_args(args)
    return args


def main(args=sys.argv):
    """
    Main program, degenerates into the run loop
    :param args: arguments
    :return: return code
    """
    parsed = parse_args(args[1:])
    duplexer = SerialTcp(parsed.device, parsed.baud, parsed.addr, parsed.port)
    duplexer.open()
    duplexer.run()
    duplexer.close()

if __name__ == "__main__":
    sys.exit(main())
