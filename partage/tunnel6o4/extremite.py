#!/usr/bin/env python
import os
from SocketServer import StreamRequestHandler, ThreadingTCPServer
import re
import socket
import sys
import time
import traceback
from threading import Thread
from array import array

import config
from iftun import create_tun_dev


def current_time():
    return time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(time.time()))


class EchoHandler(StreamRequestHandler):
    ftun = 0

    def log(self, peer, size):
        mapped = re.compile("^::ffff:", re.IGNORECASE)
        peer = re.sub(mapped, "", peer)
        sys.stdout.write("%s - %s - %i bytes\n" % (current_time(), peer, size))

    def handle(self):
        """ Echoes (sends back) whatever it reads """
        data = "DUMMY"
        size = 0
        peer = self.client_address[0]
        print ("FTUN %s" % self.ftun)
        while data != "":
            data = self.rfile.read(1)
            try:
                self.wfile.write(data)
                size = size + len(data)
                os.write(1, data) # Write data to the standard output
                packet = array('B', data)
                os.write(self.ftun, bytes(packet)) # Write data to the tun dev
            except socket.error:  # Client went away, do not take that data into account
                data = ""
        self.log(peer, size)


def ext_out(ftun=0):
    """
    Create a TCP server and return a TCPServer instance
    """
    ThreadingTCPServer.allow_reuse_address = True
    ThreadingTCPServer.address_family = socket.AF_INET6
    EchoHandler.ftun = ftun
    server = ThreadingTCPServer((config.INIP, config.INPORT), EchoHandler)
    return server


def ext_in(ftun):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0) # Internet
    while True:
        try:
            sock.connect((config.OUTIP, config.OUTPORT))
            break
        except:
            time.sleep(3)
    print "EXT-IN connected"
    while True:
        # Read an IP packet been sent to this TUN device.
        packet = os.read(ftun, 2048)
        # Write data from tun dev to the socket fsv
        try:
            sock.send(packet)
        except:
            traceback.print_exc()
            print "Writing error"
            # import pdb; pdb.set_trace()


if __name__ == '__main__':
    tun = create_tun_dev("tun0", config.TUNIP)
    ftun = tun.fileno()
    server = ext_out(ftun=ftun)
    t =Thread(target=ext_in,
                args=(ftun,))
    t.daemon = True
    t.start()
    server.serve_forever()

