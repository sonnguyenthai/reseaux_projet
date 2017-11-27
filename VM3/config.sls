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

## Configuration de VM3
eth1:
  network.managed:
    - enabled: True
    - type: eth
    - proto: none
    - ipaddr: 172.16.2.163
    - netmask: 28

## Configuration de la route vers LAN1 via VM2
routes:
  network.routes:
    - name: eth1
    - routes:
      - name: LAN1
        ipaddr: 172.16.2.128/28
        gateway: 172.16.2.162

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