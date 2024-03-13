import serial

import serial.tools.list_ports as SLP


import time

arduino_port = 'COM4'
baud_rate = 9600

def show_ports():
    ports = SLP.comports() #recuperer les ports utilisés

    print(ports)
    portlist = []

    for port in ports :
        portlist.append(str(port))


    print(portlist)

#show_ports()
##


"""
val = str(input("select port : COM"))

for x in range(len(portlist)):
    if portlist[x].startswith("COM" + val):
        portVar = "COM" + str(val)
        print(portlist[x])

"""






def show_settings(ser_object):
    if ser_object.isOpen():
        print(ser_object.name + ' is open...\n')
        print(ser_object.get_settings())


##
def e_is_in_text(t):
    assert(len(t)>=1)
    #print(t[0]=="E")
    return t[0]=="E"





def read_and_write_data(chemin="./test.txt",arduino_port='COM4',baud_rate=9600):
    #show_ports()

    ser=serial.Serial(arduino_port,baud_rate)
    #show_settings(ser)
    myfile = open(chemin+".erun","w",encoding ="utf-8")
    myfile.close()

    i=0

    test = e_is_in_text("a")
    while not(test) :
        myfile = open(chemin,"a",encoding ="utf-8")
        if(i==0):
            debut = time.time()
        if ser.in_waiting :
            packet = ser.readline()
            decode = packet.decode('utf').rstrip('\n')

            if e_is_in_text(decode):
                test = True
            else :

                myfile.write(decode)
                print("written in file : ",decode)
                i+=1
                #print("i=",i)

        myfile.close()

    ser.close()#il faut le fermer si on veut voir depuis arduino
    #print("fermeture du port série")
    return





#read_and_write_data("conduite_maison_quartir_libre_23_02")
