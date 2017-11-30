import fcntl
import os
import struct
import sys

# Some constants used to ioctl the device file
TUNSETIFF = 0x400454ca
TUNSETOWNER = TUNSETIFF + 2
IFF_TUN = 0x0001
IFF_TAP = 0x0002
IFF_NO_PI = 0x1000


def create_tun_dev(dev_name):
    # Open file corresponding to the TUN device.
    tun = open('/dev/net/tun', 'r+b')
    ifr = struct.pack('16sH', dev_name, IFF_TUN | IFF_NO_PI)
    fcntl.ioctl(tun, TUNSETIFF, ifr)
    fcntl.ioctl(tun, TUNSETOWNER, 1000)

    # Bring it up and assign addresses.
    # /sbin/ifconfig tun0 inet6 add fc00:1234:ffff::1/64 
    # subprocess.check_call('ifconfig tun0 192.168.7.1 pointopoint 192.168.7.2 up', shell=True)
    return tun


if __name__ == "__main__":
    args = sys.argv
    if len(args) < 2:
        print ("You do not enter device name. Using default name tun0")
        dev_name = "tun0"
    else:
        dev_name = args[1]
    tun = create_tun_dev(dev_name)
    print ("Created tun device %s" % dev_name)

    while True:
        # Read an IP packet been sent to this TUN device.
        print ("In loop")
        packet = list(os.read(tun.fileno(), 2048))

        # Modify it to an ICMP Echo Reply packet.
        #
        # Note that I have not checked content of the packet, but treat all packets
        # been sent to our TUN device as an ICMP Echo Request.

        # Swap source and destination address.
        packet[12:16], packet[16:20] = packet[16:20], packet[12:16]

        # Under Linux, the code below is not necessary to make the TUN device to
        # work. I don't know why yet. If you run tcpdump, you can see the
        # difference.
        # Write the reply packet into TUN device.
        print (''.join(packet))
        os.write(1, ''.join(packet))