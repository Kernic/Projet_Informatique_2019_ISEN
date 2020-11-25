# -*- coding: utf-8 -*-
def infoFile():
    listLecture = lecture()
    listInfo =[]
    for i in listLecture:
        if i[0] != '#':
            _ = []
            t = i.split(' ')
            _.append(t[0])
            if t[1] == '1':
                _.append('Etat: Au repos')
            elif t[1] == '2':
                _.append(u'Etat: Apres effort')
            else : 
                _.append('NA')
            
            if t[2] == 'F':
                _.append('Sexe:Feminin')
            elif t[2] == 'H':
                _.append('Sexe:Masculin')
            else:
                _.append('NA')
            
            if t[3] == 'N':
                _.append('Activite Physique: non sportif')
            elif t[3] == 'O':
                _.append('Activite Physique: sportif')
            else:
                _.append('NA')
            
            if t[4] == 'N\n' or t[4] == 'N':
                _.append('Respiration: normale')
            elif t[4] == 'P\n' or t[4] == 'P':
                _.append('Respiration: profonde')
            else:
                _.append('NA')
            
            listInfo.append(_)
    return listInfo

def lecture():
    listLecture = []
    info = open("./data/description.txt", 'r')
    listLecture = info.readlines()
    info.close()
    return listLecture
