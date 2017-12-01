

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