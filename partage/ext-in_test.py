#-*- coding: utf-8 -*-


'''
import * from extremite

ext_in()
'''
import socket, sys
PORT = 123
HOST = '172.16.2.163'

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
    client_socket.bind((HOST,PORT))
except:
    print("Erreur bind socket port %s" % PORT)
    sys.exit()
print("Connexion établie")

ser_rsp = client_socket.recv(1024).encdoe("Utf8")

while 1:
    if ser_rsp.upper() == "FIN" or ser_rsp == "":
        break
    print("Serveur >>")

    msg_cli = input("Client >>")
    client_socket.send(msg_cli.encode("Utf8"))
    ser_rsp = client_socket.recv(1024).decode("Utf8")

print("Connexion interrompue")
client_socket.close()