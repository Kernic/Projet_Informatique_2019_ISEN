# -*- coding: utf-8 -*-

import os
import TableHome

def recupData():
    listFile = [] 
    for i in os.listdir("./data"):
        listFile.append(i)
    
    return listFile

def fileSep(listFile):
    tListFile = []
    for i in listFile:
        _ = i.split("-")
        if len(_) == 1:
            tListFile.append(i)
    
    return tListFile

def orderFile(listFile):
    orderedFiles = list(listFile)
    for i in listFile:
        _ = i.split(".")
        __ = _[0].split('G')
        try:
            number = int(__[1])
            orderedFiles[number] = i 
        except:
            pass
    
    return orderedFiles[1:]

def makeTable(fileName):
    file = open("./data/" + fileName, "r")
    listLine = file.readlines()
    file.close()
    Data=[]
    for i in listLine:
        _ = float(i.split(";")[0])
        __ = float((i.split(";")[1]).split("\n")[0])
        Data.append([_ , __])
    
    return Data


def makeCode(listData):
    codeList = []
    for i in listData[int(len(listData)/2) : int(len(listData)/2) + 3000]:
        codeList.append("lineGraph2.addPoint(" + str(i[0]) + "," + str(i[1]) + ");\n")

    return codeList


def makeGraph(listHtml, listData):
    for i in range(0, len(listHtml)):
        codeList = []
        flag = 0
        
        file = open("./html/"+ listHtml[i], "r")
        fileData = file.readlines()
        file.close()
        
        for y in fileData:
            
            if flag == 0:
                codeList.append(y)
            
            if y == "//BEGIN DRAW ECG\n":
                flag=1
                _ = makeCode(makeTable(listData[i]))
                for z in _:
                    codeList.append(z)
            
            if y == "//END DRAW ECG\n" and flag == 1:
                flag = 0
                codeList.append(y)
        
        file = open("./html/"+ listHtml[i], "w")
        for y in codeList:
            file.writelines(y)

        file.close()


def main():

    test = fileSep(recupData())
    _ = orderFile(test)
    listHtml = TableHome.recupNom()[1:]

    makeGraph(listHtml, _)

main()