#!/usr/bin/env bash

echo "Type the name of the tun device, followed by [ENTER]:"

read dev_name

if [[ -z "${dev_name// }" ]]
then
    dev_name="tun0"
fi

/sbin/ifconfig $dev_name inet6 add fc00:1234:ffff::1/64 up

/sbin/ifconfig $dev_name

echo "Configured successfully"