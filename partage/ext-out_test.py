#-*- coding: utf-8 -*-

'''
import ext_out from extremite

ext_out()
'''
import socket, sys

PORT = 123
HOST = ''


def ext_out():
    is_connected = 0

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        server_socket.bind((HOST,PORT))
    except:
        print("Error bind socket port %s" % PORT)
        sys.exit()

    while 1:
        print("Serveur IPv4 est prêt, attente active...")
        server_socket.listen(2)

        connect, adresse = server_socket.accept()
        is_connected += 1

        print("Client connecté, adress IP: %s, sur le port %s" % (adresse[0], adresse[1]))

        response = "Vous êtes connecté sur le serveur VM1. Envoyez des paquets !"
        connect.send(response.encode("Utf8"))
        cli_rsp = connect.recv(1024).decode("Utf8")

        while 1:
            print("Client >> ", cli_rsp)
            if cli_rsp.upper() == "FIN" or cli_rsp == "":
                break
            #response = input("Server >>")
            #connect.send(response.encode("Utf8"))
            cli_rsp = connect.recv(1024).decode("Utf8")

        connect.send("Fin de la connexion".encode("Utf8"))
        print("Connexion interrompue")
        connect.close()

ext_out()