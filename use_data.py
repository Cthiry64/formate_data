from math import acos,cos,sin,pi
import os
os.chdir("C:/Users/cleme/Bureau/clement/travail/mpi/TIPE/sketch/officiel_scripts/exploitation_données")



r =6378.137e3 #rayon de la terre (m)

##
def convert_degrees_to_radians(a):
    return a*pi/180


def convert_rad_to_degrees(a):
    return a*180/pi



def distance(lata,lnga,latb,lngb,r):
    return r*acos(cos(convert_degrees_to_radians(lata))*cos(convert_degrees_to_radians(latb))*cos(convert_degrees_to_radians(lnga-lngb))+sin(convert_degrees_to_radians(lata))*sin(convert_degrees_to_radians(latb)))

def calcule_dt(ha,ma,sa,hb,mb,sb):
    dt = (hb-ha)*3600+(mb-ma)*60+sb-sa
    if(abs(dt)<=1):
        print("durée nulle :",dt)
        debug = str(ha)+":"+str(ma)+":"+str(sa)+"----->"+str(hb)+":"+str(mb)+":"+str(sb)
        print(debug)
        return None
    else:
        return dt

def calcule_v(d,dt):
    return d/dt

def cherche_max(l):
    emax = l[0]
    for e in l:
        if e>emax:
            emax=e
    return emax

"""def get_new_data(chemin):
    f     = open(chemin,mode='r',encoding='utf-8')
    lines = f.readlines()
    f.close()
    global r
    n     = len(lines)
    #print(lines[n-1])
    v = []
    d = 0.0
    l1 = []
    l2 = []
    ipbtq = []
    for i in range(n-1):
        l1 = lines[i].split(";")
        l2 = lines[i+1].split(";")
        #print(l1)
        #print(l2)
        dprim = distance(float(l1[0]),float(l1[1]),float(l2[0]),float(l2[1]),r)
        d = d + dprim
        dt = calcule_dt(int(l1[2]),int(l1[3]),int(l1[4]),int(l2[2]),int(l2[3]),int(l2[4]))
        if(dt==None):
            ipbtq.append(i)
        else:
            v.append(calcule_v(dprim,dt))
    for ind in ipbtq:
        lines.pop(ind)
        print("suppression de la ligne",ind)
    #print(lines)
    l0=lines[0].split(";")
    lnm1=lines[n-1].split(";")
    duree=calcule_dt(int(l0[2]),int(l0[3]),int(l0[4]),int(lnm1[2]),int(lnm1[3]),int(lnm1[4]))
    vmax = cherche_max(v)
    return (d,vmax,v,lines,duree)"""


def get_new_data(chemin):
    f     = open(chemin,mode='r',encoding='utf-8')
    lines = f.readlines()
    f.close()
    global r
    n     = len(lines)
    #print(lines[n-1])
    v = []
    d = 0.0
    l1 = []
    l2 = []

    i=0
    while(i<n-1):
        l1 = lines[i].split(";")
        l2 = lines[i+1].split(";")
        #print(l1)
        #print(l2)
        dprim = distance(float(l1[0]),float(l1[1]),float(l2[0]),float(l2[1]),r)
        d = d + dprim
        dt = calcule_dt(int(l1[2]),int(l1[3]),int(l1[4]),int(l2[2]),int(l2[3]),int(l2[4]))
        if(dt==None):
            suppr = lines.pop(i)
            print("suppression de :",suppr)
            n=n-1
        else:
            v.append(calcule_v(dprim,dt))
            i=i+1

    #print(lines)
    l0=lines[0].split(";")
    lnm1=lines[n-1].split(";")
    duree=calcule_dt(int(l0[2]),int(l0[3]),int(l0[4]),int(lnm1[2]),int(lnm1[3]),int(lnm1[4]))
    vmax = cherche_max(v)
    return (d,vmax,v,lines,duree)

def get_name_file(chemin):
    c=""
    for i in range(len(chemin)):
        if(chemin[i]=="."):
            return c+".erun"
        else:
            c=c+chemin[i]
    return "nameless.erun"



def update_file(chemin, v_max,dtot,v,lines,duree):
    #print(get_name_file(chemin))
    f =  open(get_name_file(chemin),"w",encoding="utf-8")
    n = len(lines)
    for i in range(n-1):
        theline = ""
        l=lines[i].split(";")
        for j in range(2):
            theline = theline + l[j] + ";"
        theline = theline+ str(v[i]) + "\n"
        #print(theline)
        print("written in erun : ",theline)
        f.write(theline)
    l = lines[n-1].split(";")
    theline = l[0] + ";" + l[1] + ";" + "V404" +"\n" + str(dtot) +  ";" + str(v_max)+";"+str(duree)+"\n"
    f.write(theline)
    f.close()




##
def creer_fichier_formate(chemin="test21_02.txt"):
    dtot,v_max,v,lines,duree=get_new_data(chemin)
    #print(len(v),len(lines))
    update_file(chemin,v_max,dtot,v,lines,duree)




#creer_fichier_formate("conduite_maison_quartier_libre_23_02.txt")