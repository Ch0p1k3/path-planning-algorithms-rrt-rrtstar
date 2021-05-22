import sys
import os
import lxml.etree as ET
import random

def process(binaryRRT: str, 
            binaryAstar: str, 
            pathFileRRT: str, 
            pathFileAstar: str, 
            pathFolderInputRRT: str, 
            pathFolderOutputRRT: str, 
            pathFolderInputAstar: str, 
            pathFolderOutputAstar: str, index: int):
    pathFileRRT = os.path.normpath(pathFileRRT)
    pathFolderInputRRT = os.path.normpath(pathFolderInputRRT)
    pathFolderOutputRRT = os.path.normpath(pathFolderOutputRRT)
    tree = ET.parse(pathFileRRT)
    root = tree.getroot()
    log = root.find('log')
    path = log.find('path')
    map_ = root.find('map')
    minDist = max(int(map_.find('width').text), int(map_.find('height').text))
    minDist //= 2
    grid =  map_.find('grid')
    startX = map_.find('startx')
    startY = map_.find('starty')
    finishX = map_.find('finishx')
    finishY = map_.find('finishy')
    freeCeils = []
    for i, row in enumerate(grid):
        for j, e in enumerate(row.text.split(' ')):
            if e == '0':
                freeCeils.append((j, i))
    random.shuffle(freeCeils)
    startX_, startY_ = .5 + freeCeils[0][0], .5 + freeCeils[0][1]
    random.shuffle(freeCeils)
    finishX_, finishY_ = .5 + freeCeils[0][0], .5 + freeCeils[0][1]
    while (startX_ - finishX_) ** 2 + (startY_ - finishY_) ** 2 < minDist ** 2:
        random.shuffle(freeCeils)
        startX_, startY_ = .5 + freeCeils[0][0], .5 + freeCeils[0][1]
        random.shuffle(freeCeils)
        finishX_, finishY_ = .5 + freeCeils[0][0], .5 + freeCeils[0][1]
    startX.text, startY.text = str(startX_), str(startY_)
    finishX.text, finishY.text = str(finishX_), str(finishY_)
    nameFile = pathFileRRT.split(os.sep)[-1].split('.')[0]
    pathFileOutput = os.path.join(pathFolderOutputRRT, f'{nameFile}_log_{index}.xml')
    path.text = pathFileOutput
    pathFileInput = os.path.join(pathFolderInputRRT, f'{nameFile}_{index}.xml')
    tree.write(pathFileInput, pretty_print=True)
    os.system(f'{binaryRRT} {pathFileInput}')
    pathFileAstar = os.path.normpath(pathFileAstar)
    pathFolderInputAstar = os.path.normpath(pathFolderInputAstar)
    pathFolderOutputAstar = os.path.normpath(pathFolderOutputAstar)
    tree = ET.parse(pathFileAstar)
    root = tree.getroot()
    map_ = root.find('map')
    startX = map_.find('startx')
    startY = map_.find('starty')
    finishX = map_.find('finishx')
    finishY = map_.find('finishy')
    startX.text, startY.text = str(int(startX_)), str(int(startY_))
    finishX.text, finishY.text = str(int(finishX_)), str(int(finishY_))
    options = root.find('options')
    logPath = options.find('logpath')
    logFileName = options.find('logfilename')
    nameFile = pathFileAstar.split(os.sep)[-1].split('.')[0]
    logPath.text = pathFolderOutputAstar
    logFileName.text = f'{nameFile}_log_{index}.xml'
    pathFileInput = os.path.join(pathFolderInputAstar, f'{nameFile}_{index}.xml')
    tree.write(pathFileInput, pretty_print=True)
    os.system(f'{binaryAstar} {pathFileInput}')

def main():
    binaryRRT = sys.argv[-5]
    binaryAstar = sys.argv[-4]
    pathFileRRT = sys.argv[-3]
    pathFileAstar = sys.argv[-2]
    pathFolder = sys.argv[-1]
    pathFolder = os.path.normpath(pathFolder)
    pathFolderInputRRT = os.path.join(pathFolder, 'RRTinput')
    pathFolderOutputRRT = os.path.join(pathFolder, 'RRToutput')
    pathFolderInputAstar = os.path.join(pathFolder, 'Astarinput')
    pathFolderOutputAstar = os.path.join(pathFolder, 'Astaroutput')
    try:
        os.mkdir(pathFolderInputRRT)
    except Exception:
        print('Problems with creation of the input folder RRT. Maybe it is creted.')
    try:
        os.mkdir(pathFolderInputAstar)
    except Exception:
        print('Problems with creation of the input folder Astar. Maybe it is creted.')
    try:
        os.mkdir(pathFolderOutputRRT)
    except Exception:
        print('Problems with creation of the output folder RRT. Maybe it is creted.')
    try:
        os.mkdir(pathFolderOutputAstar)
    except Exception:
        print('Problems with creation of the output folder Astar. Maybe it is creted.')
    for i in range(100):
        process(binaryRRT, binaryAstar, pathFileRRT, pathFileAstar, pathFolderInputRRT, pathFolderOutputRRT, pathFolderInputAstar, pathFolderOutputAstar, i)


if __name__ == '__main__':
    main()