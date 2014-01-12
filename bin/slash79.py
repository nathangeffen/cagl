#!/usr/bin/python3

import sys

def processInput(inputFile, slashcol):
    for line in inputFile.readlines():
        incount = 0
        outcount = 0
        line = line.rstrip()
        l = len(line)
        if l > 0 and line[-1] == "\\":
            if l < slashcol:
                line = line[:-1] + " "
                line = line + ' ' * (slashcol - l)
                line = line[:-1] + "\\"
            else:
                linetmp = line[:-1]
                linetmp = linetmp.rstrip()
                if len(linetmp) < slashcol:
                    line = line[0:slashcol]
                    line = line[:-1] + "\\"
        print(line)

if __name__ == '__main__':
    slashcol = 79
    inputFile = None
    try:
        for arg in sys.argv[1:]:
            if arg[0:2] == "-t":
                slashcol = int(arg[2:])
            elif arg[0:2] == "-i":
                inputFile = open(arg[2:], "r")
            else:
                print("Unknown option: ", arg)
    except Exception as e:
        print("Illegal option: ", sys.exc_info()[0], sys.exc_info()[1])
    else:
        if inputFile == None:
            inputFile = sys.stdin
        processInput(inputFile, slashcol)
