# -*- coding: utf-8 -*-
import os
import TableHome
import webbrowser
import ECG
#Fonction récupérant les fichiers ECGx-info.txt se trouvant dans 
#le répertoire /data.

def fichiers():
    listeNameFiles=[]
    listeIndicatif=[]
    listeN_I=[]
    for c in os.listdir("./data/"):
        #Effectuer un split sur '-' nous permet de différencier les fichiers
        #ECGx.txt des fichiers ECGx-info.txt: étant stocker 
        #sous la forme d'une liste les premiers ont une taille de 1 et les
        #second de deux
        i=c.split('-')
        if len(i)==2:
            listeNameFiles.append(c)
    #La boucle while permet de récupérer l'indicatif, en effet les fichiers ne
    #sont pas récupérés selon un odre numérique,
    # il faut donc récupérer la numérotation associée au fichier. 
    # ex: ECG13-info.txt ->13
            
    #Compteur=3 pour les trois lettres d'ECG nous permettant de commencer à
    #compter à partir de l'indice correspondant à la numérotation du fichier.
            
    compteur=3
    y=0
    stock=""
    while y!=len(listeNameFiles):
        #On récupère les chiffres depuis l'indice 3 de notre chaîne de caractère
        #jusqu'au tiret, ce qui nous permet d'avoir un nombre illimité d'ECGx.txt.
            while listeNameFiles[y][compteur]!="-":
                stock=stock+listeNameFiles[y][compteur]
                compteur+=1
            listeIndicatif.append(stock)
            stock=''
            y+=1
            compteur=3
    listeN_I=[listeNameFiles,listeIndicatif]
    return(listeN_I)



# Fonction appelant nos diverses fonctions de lectures de données 
# et d'écritures pour créer les fichiers .html correspondant à nos fichiers .txt de /data.

def genPre():
    listeNameFiles=fichiers()[0]
    listeIndicatif=fichiers()[1]
    
    for d in range (0,len(listeIndicatif)):
        DataLines=EcgData(listeNameFiles[d])    
        liste=dataHB(DataLines)  
        contenuPageHtml=dataTemplate()
        
        vMoy(DataLines,listeIndicatif[d])
        Pas=pas(DataLines) 
        contenuPageHtml2=writeGraph(liste,listeIndicatif[d],Pas,contenuPageHtml)
        
        listeVM=vMoy(DataLines,listeIndicatif[d])
        contenuPageHtml3=dispVmoy(listeVM, DataLines, listeIndicatif[d],contenuPageHtml2)
        writePage(listeIndicatif[d], contenuPageHtml3)
        title(listeIndicatif[d])

        alerteBPM(liste, listeIndicatif[d])

        
#Retourne les données de ECGx-data, et les stocke sous forme 
#d'une liste de lignes du fichier.
        
def EcgData(nameFile):
    InfoData=open("./data/"+nameFile,"r")
    lines=InfoData.readlines()
    InfoData.close()
    return(lines)



#Récupère les données du template sous forme liste de lignes.

def dataTemplate():
    template=open("./template.html","r")
    contenuT=template.readlines()
    template.close()
    return(contenuT)



#Ecrie les données pour la page html.

def writePage(indicatif,contenuPage):
    htmlPreset=open("./html/Preset"+indicatif+".html","w")
    for i in (contenuPage):
        htmlPreset.writelines(i)   
    htmlPreset.close() 
    
    

#Remplace nos balises ###Titre par le nom du fichier.

def title(indicatif):
    htmlPreset=open("./html/Preset"+str(indicatif)+".html","r")
    contenu=htmlPreset.read()
    htmlPreset.close()
    htmlPreset2=open("./html/Preset"+str(indicatif)+".html","w")
    htmlPreset2.write(contenu.replace("###Titre","ECG"+str(indicatif)))
    htmlPreset2.close()
    
    
#Récupère les données heart beat entre nos deux balises et les retourne sous
#forme de liste.

def dataHB(DataLines):
    flag=0
    listeHB=[]  
    for i in DataLines:
        if i=='###END DATA HEARTBEAT\n':
            flag=0
            
        if flag==1:
            listeHB.append(i.rstrip())  
            
        if i=='###BEGIN DATA HEARTBEAT\n':
            flag=1
            
    return(listeHB)


#A partir des données précédentes, écrie les lignes de codes avec 
#les points correspindant et
# permmettant ainsi de tracer le rythme cardiaque sur les pages .html

def writeGraph(liste,indicatif,pas,contenuPageHtml):
    flag=0
    compteur1=0
    compteur2=0
    compteur3=pas

    contenuPageHtmlBis=contenuPageHtml
    for i in contenuPageHtml:
        if i =='//END DRAW GRAPH\n':
            flag=0
        
        if flag==1:
           
            while compteur2 != len(liste): 
                a='lineGraph.addPoint('+str(compteur3)+','+liste[compteur2]+');\n'
                contenuPageHtmlBis.insert(compteur1+compteur2,a)
                compteur2+=1
                compteur3+=pas
        if i=='//BEGIN DRAW GRAPH\n':
            flag=1
        
        compteur1+=1
        
    return(contenuPageHtmlBis)


#Fonction permettant de récupérer le pas avec lequel à été calculer le rythme,
#nécéssaire pour la fonction précédente.

def pas(lines):
    flag=0
    pas=0 
    for i in lines:
        if i=='###END PAS\n':
            flag=0
        
        if flag==1:
            c=i.split('\n')
            pas=float(c[0])
            
        if i=='###BEGIN PAS\n':
            flag=1

    return(pas)



#Fonction permettant de récupérer les valeurs moyennes dans ECGx-info.txt.

def vMoy(lines,indicatif):
    flag=0
    listeVM=[] 
    for i in lines:
        
        #Tag servant de point de repère, 
        #apparaissant la ligne en-dessous de la dernière valeure moyenne.
        
        if i=='###BEGIN DATA WAVES\n':
            flag=0
            
        if flag==1 and i!='\n':
            c=i.split('\n')
            listeVM.append(c[0])
            
        #Elément servant de point de repère, apparaissant la ligne au-dessus 
        #de la première valeure moyenne.
            
        if i=='ECG'+str(indicatif)+'.txt\n':
            flag=1

    return(listeVM)    


#Fonction permettant d'écrire les valeurs moyennes sur nos pages html
#entre deux tagset d'afficher une alerte si nécéssaire.

def dispVmoy(listeVM,lines,indicatif,contenuPageHtml):
    flag=0
    compteur1=0
    compteur2=0
    contenuPageHtmlBis=contenuPageHtml
    alertQT=tabQT(indicatif, listeVM)
    listeName=["BPM moyen : ","Temps moyen QRS : ","Temps moyen QT : ","Temps moyen PQ : "]
    for i in contenuPageHtml:
        if i =='				<!--###EndVM-->\n':
            flag=0
        
        if flag==1:
            while compteur2 != len(listeVM):
                
                if compteur2==0 and float(listeVM[0])>120:
                    a='<li class="ALERT">'+listeName[compteur2]+str(listeVM[compteur2])+' &#9888;ALERTE: >120ms </li>\n'
                    contenuPageHtmlBis.insert(compteur1+compteur2,a)
                    compteur2+=1
  
                if compteur2==2 and alertQT=='ALERT':
                    a='<li class="ALERT">'+listeName[compteur2]+str(listeVM[compteur2])+' &#9888; ALERTE: intervalle QT de reference depasse</li>\n'
                    contenuPageHtmlBis.insert(compteur1+compteur2,a)
                    compteur2+=1
                else:
                    a='<li>'+listeName[compteur2]+str(listeVM[compteur2])+'</li>\n'
                    contenuPageHtmlBis.insert(compteur1+compteur2,a)
                    compteur2+=1

        if i=='				<!--###StartVM-->\n':
            flag=1
        compteur1+=1
    return(contenuPageHtmlBis)

#Fonction permettant de connaître le genre de l'individu correspondant à l'ECG.

def sex(indice):
    idText=4+int(indice)
    description=open("./data/description.txt","r")
    contenu=description.readlines()
    description.close()
    sex=contenu[idText].split(' ')[2]
    return(sex)



#Fonction permettant de vérifier si l'intervalle QT moyen est dans la norme
#par rapport au bpm et de retourner une alerte sinon.

def tabQT(indice,listeVM):
    counter=0
    QtHomme={40:[0.41,0.50],44:[0.39,0.47],50:[0.36,0.45],55:[0.35,0.44],60:[0.33,0.42],71:[0.31,0.40],80:[0.30,0.38],92:[0.28,0.37],100:[0.27,0.36]}
    QtFemme={40:[0.42,0.51],44:[0.40,0.49],50:[0.37,0.46],55:[0.36,0.45],60:[0.34,0.43],71:[0.32,0.41],80:[0.31,0.39],92:[0.29,0.38],100:[0.28,0.37]}
    BPMtab=[40,44,50,55,60,71,80,92,100]
    BPM=float(listeVM[0])
    QT=float(listeVM[2])/1000
    counterStock=0
    Genre=sex(indice)
    flag=0
    flagAlert='RAS'
    if Genre=='H':
        for i in BPMtab:
            
            if i>BPM and flag==0:
                counterStock=counter
                flag=1
                
            counter+=1 
        
        if QtHomme[BPMtab[counterStock]][1]<QT:
            flagAlert='ALERT'
            
        if QtHomme[BPMtab[counterStock-1]][0]>QT:
            flagAlert='ALERT'
            
        return(flagAlert)
    
    if Genre=='F':
        for i in BPMtab:
            
            if i>BPM and flag==0:
                counterStock=counter
                flag=1
                
            counter+=1 
        
        if QtFemme[BPMtab[counterStock]][1]<QT:
            flagAlert='ALERT'
            
        if QtFemme[BPMtab[counterStock-1]][0]>QT:
            flagAlert='ALERT'
            
        return(flagAlert)

def bpmCheck(listBPM):
    pre = 1000000 # Valeur très supérieurs a ce que l'on peut attendre 
    etat = False
    for i in listBPM:
        if float(pre) < float(i) :
            etat = True
            pre = i
        else : 
            pre = i
    
    return etat


def alerteBPM(listBPM, indice):
    
    file = open("./html/Preset" + str(indice) + ".html", "r")
    code = file.readlines()
    file.close()
    test = bpmCheck(listBPM)
    codeV2 = []
    for i in code:
        if i == "                <!--BaliseBPM-->\n" and test==True:
            codeV2.append(i)
            codeV2.append('                <div class="ALERT"> les BPM ne suivent pas une baisse progressive &#9888;</div>\n')
        else:
            codeV2.append(i)
    
    file = open("./html/Preset" + str(indice) + ".html", "w")
    for i in codeV2:
        file.writelines(i)
        
    
    file.close()




"------------------------------------------------------------------------"

genPre()
TableHome.modifyUrl()
ECG.main()
webbrowser.open("file://" + os.path.realpath("./html/Home.html"), new=2, autoraise=True) # Permet d'ouvrir le web browser utiliser par l'utilisateur et d'y afficher la page Home