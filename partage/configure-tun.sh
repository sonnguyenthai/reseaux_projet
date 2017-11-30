#!/usr/bin/env bash

echo "Type the name of the tun device, followed by [ENTER]:"

read dev_name

/sbin/ifconfig $dev_name inet6 add fc00:1234:ffff::1/64

/sbin/ifconfig $dev_name

echo "Configured successfully"