# Configuration eth1
# RAPPEL: eth0 est à vagrant, ne pas y toucher

## Désactivation de network-manager
NetworkManager:
  service:
    - dead
    - enable: False
    
## Suppression de la passerelle par défaut
ip route del default:
  cmd:
    - run

##Configuration de VM3-6
eth1:
  network.managed:
    - enabled: True
    - type: eth
    - proto: none
    - enable_ipv4: false
    - ipv6proto: static
    - enable_ipv6: true
    - ipv6_autoconf: no
    - ipv6ipaddr: fc00:1234:2::36
    - ipv6netmask: 64

## Configuration de la route vers LAN2-6 via VM2-6
routes:
  network.routes:
    - name: eth1
    - routes:
      - name: LAN1-6
        ipaddr: fc00:1234:1::/64
        gateway: fc00:1234:2::26

## Set new default route
ip route add default via 10.0.2.2:
  cmd:
    - run

## Installation du service ECHO sur VM3
inetutils-inetd:
    pkg:
      - installed

## update inetid
update-inetd --add "echo stream tcp nowait nobody internal":
  cmd:
    - run

## Start inet (test)
service inetutils-inetd start:
  cmd:
    - run

## Resart inet (for sure)
service inetutils-inetd restart:
  cmd:
    - run

