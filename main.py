import os
os.chdir("repertoire courant")

from recup_serial_data import read_and_write_data
from use_data import creer_fichier_formate

def main(chemin,arduino_port='COM4',baud_rate=9600):
    read_and_write_data(chemin,arduino_port,baud_rate)
    creer_fichier_formate(chemin)
