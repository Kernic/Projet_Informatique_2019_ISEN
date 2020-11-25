# -*- coding: utf-8 -*-

# Ce script python est utiliser dans le script agregateur 

import os
import info

# Cette fonction permet de modifier les liens URL dans le tableau HTML pour les lier au pages html d'annalyse de fichier
# Elle ne prend rien en entré et ne renvoie rien 

def modifyUrl():
    listPages = recupNom()
    listEcriture = []
    flag = 0
    listHtml = ecritureFichier(listPages)
    # ouverture et lecture du fichier
    home = open(os.path.realpath("./html/Home.html"), "r")
    listLignes = home.readlines()
    home.close()

    for i in range(0, len(listLignes)):
        if (listLignes[i] == "<!--balise-->\n"):
            flag = 1
            listEcriture.append("<!--balise-->\n")
            for code in listHtml:
                listEcriture.append(code)
        
        if flag == 0:
            listEcriture.append(listLignes[i])

        if flag == 1 and listLignes[i] == "<!--finBalise-->\n":
            flag = 0
            listEcriture.append("<!--finBalise-->\n")
    home = open("./html/Home.html", "w")

    for i in listEcriture:
        home.writelines(i)
    
    home.close()

            

# Cette fonction permet de créé une list qui vas écrire le code html pour créé le tableau
# elle prend en entrée la liste contenant les noms des fichier HTML et renvoie une list contenant le code HTML

def ecritureFichier(listPages):
    codeHtml = []
    infoTable = info.infoFile()
    for i in range(0, len(listPages)):
        if listPages[i] != "Home.html":
            codeHtml.append('<tr class="body' + str(i%2 + 1) + '">\n')
            codeHtml.append('<td class="url"><a href="./' + listPages[i] + '">ECG' + str(i) + '</a></td>\n')
            codeHtml.append('<td>\n')
            codeHtml.append('<ul>\n')

            codeHtml.append('<li>' + infoTable[i - 1][1] + '</li>\n')
            codeHtml.append('<li>' + infoTable[i - 1][2] + '</li>\n')
            codeHtml.append('<li>' + infoTable[i - 1][3] + '</li>\n')
            codeHtml.append('<li>' + infoTable[i - 1][4] + '</li>\n')
            
            codeHtml.append('</ul>\n')
            codeHtml.append('</td>\n')
            codeHtml.append('</tr>\n')
    
    return codeHtml

# Cette fonction pemet de récupérer les noms des fichiers .html dans le dossier html
# Elle ne prend rien en entré et renvois une list contenant les noms des fichiers .html

def recupNom ():
    listPages = []

    for i in os.listdir("./html/"):
        if (i[-4:] == "html"): # Permet de garder que les fichier avec les extantions .html
            listPages.append(i)
    
    return sortList(listPages) # Permet de renvoyer une list avec les fichiers dans leurs ordre d'indice


def sortList(listA):
    _ = []
    number = ""
    sortedList = list(listA)
    for i in listA:
        _ = i.split('t') # le template des pages est Preset##.html le chiffre viens donc apprès le t 
        number = _[1].split('.')[0] # le template
        try : # permet d'évité le cas ou l'on récupaire une chane de caractère et non un chiffre 
            number = int(number)
            sortedList[number] = i
        except:
            pass
    
    return sortedList
