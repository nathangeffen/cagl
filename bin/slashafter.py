#!/usr/bin/python3

import sys

def processInput(inputFile):
    for line in inputFile.readlines():
        if len(line) > 1 and line[-2] == '\\':
            line = line[:-2]
            line = line.rstrip()
            line = line + ' \\\n'
        print(line, end="")

if __name__ == '__main__':
    inputFile = None
    try:
        for arg in sys.argv[1:]:
            if arg[0:2] == "-i":
                inputFile = open(arg[2:], "r")
            else:
                print("Unknown option: ", arg)
    except Exception as e:
        print("Illegal option: ", sys.exc_info()[0], sys.exc_info()[1])
    else:
        if inputFile == None:
            inputFile = sys.stdin
        processInput(inputFile)
