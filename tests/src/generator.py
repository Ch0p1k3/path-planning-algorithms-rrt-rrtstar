import sys
import os
from io import TextIOWrapper
import lxml.etree as ET
from const import ALGORITHMS
from random import shuffle as randomsh

def generate_object(file: TextIOWrapper, filepath: str, typealgorithm: str):
    arr = file.readlines()
    root = ET.Element('root')
    map_ = ET.SubElement(root, 'map')
    height = ET.SubElement(map_, 'height')
    height.text = arr[1].split()[1]
    width = ET.SubElement(map_, 'width')
    width.text = arr[2].split()[1]
    startx = ET.SubElement(map_, 'startx')
    starty = ET.SubElement(map_, 'starty')
    finishx = ET.SubElement(map_, 'finishx')
    finishy = ET.SubElement(map_, 'finishy')
    grid = ET.SubElement(map_, 'grid')
    tree = ET.ElementTree(root)
    freeceils = []
    for i in range(4, len(arr)):
        tmp = []
        l = arr[i].strip()
        for j in range(len(l)):
            e = l[j]
            if e == '.':
                tmp.append('0')
                freeceils.append((j, i))
            else:
                tmp.append('1')
        row = ET.SubElement(grid, 'row')
        row.text = ' '.join(tmp)
    randomsh(freeceils)
    startx.text, starty.text = str(.5 + freeceils[0][0]), str(.5 + freeceils[0][1])
    randomsh(freeceils)
    finishx.text, finishy.text = str(.5 + freeceils[0][0]), str(.5 + freeceils[0][1])
    algorithm = ET.SubElement(root, 'algorithm')
    searchtype = ET.SubElement(algorithm, 'searchtype')
    searchtype.text = typealgorithm
    numberofiterations = ET.SubElement(algorithm, 'numberofiterations')
    numberofiterations.text = str(1000000)
    tree.write(filepath, pretty_print=True)

def main():
    folder = sys.argv[-1]
    os.chdir('..')
    if f'{folder}_js' in os.listdir():
        raise Exception(f'There is folder named {folder}_xml')
    os.mkdir(f'{folder}_xml')
    os.chdir(os.path.join('.', folder))
    print()
    print(os.getcwd())
    for algo in ALGORITHMS:
        for file in os.listdir():
            name = file.split('.')[0]
            with open(os.path.join(file), 'r') as inpfile:
                generate_object(inpfile, os.path.join(os.getcwd(), '..', f'{folder}_xml', f'{name}_{algo}.xml'), algo)
    
if __name__ == '__main__':
    main()
