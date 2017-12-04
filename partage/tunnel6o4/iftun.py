import fcntl
import os
import struct
import sys
import subprocess
from array import array

# Some constants used to ioctl the device file
TUNSETIFF = 0x400454ca
TUNSETOWNER = TUNSETIFF + 2
IFF_TUN = 0x0001
IFF_TAP = 0x0002
IFF_NO_PI = 0x1000


def create_tun_dev(dev_name, ip="fc00:1234:ffff::1/64"):
    # Open file corresponding to the TUN device.
    tun = open('/dev/net/tun', 'r+b')
    ifr = struct.pack('16sH', dev_name, IFF_TUN | IFF_NO_PI)
    fcntl.ioctl(tun, TUNSETIFF, ifr)
    fcntl.ioctl(tun, TUNSETOWNER, 1000)
    subprocess.check_call('/sbin/ifconfig %s inet6 add %s up' % (dev_name, ip),
                          shell=True)
    return tun


if __name__ == "__main__":
    args = sys.argv
    dest = None
    if len(args) < 2:
        print ("You do not enter device name. Using default name tun0")
        dev_name = "tun0"
    else:
        dev_name = args[1]
        if len(args) == 3:
            dest = args[2]

    tun = create_tun_dev(dev_name)
    if not dest:
        dest = tun.fileno()

    print ("Created tun device %s" % dev_name)

    while True:
        # Read an IP packet been sent to this TUN device.
        packet = array('B', os.read(tun.fileno(), 2048))

        # Write the reply packet into TUN device.
        os.write(dest, bytes(packet))