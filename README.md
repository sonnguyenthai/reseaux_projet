

## Create TUN device

In your VM (VM1-6)

~~~~
$ cd /mnt/partage
$ sudo python iftun.py [device_name]
$ chmod 755 configure-tun.sh
$ ./configure-tun.sh
~~~~

The script `configure-tun.sh` will ask you to enter the device name and it will config the device with
IP v6: `fc00:1234:ffff::1/64`


## Updated 1 Dec

Now you don't need to run `configure-tun.sh` to config device IP. Everything will
be done in the code


## For running part3

Open VM1, VM2, VM3

In VM1,
~~~~
$ cd /mnt/partage/tunnel6o4
$ sudo python extremite.py | hexdump -C
~~~~

In VM3,
~~~~
$ cd /mnt/partage/tunnel6o4_3
$ sudo python extremite.py
$ ping6 fc00:1234:ffff::1
~~~~
See the output in VM1. Check the packets captured by `wireshark`

## TODO
Filter packets: convert packets from `ipv6 in ipv4` to `ipv6`