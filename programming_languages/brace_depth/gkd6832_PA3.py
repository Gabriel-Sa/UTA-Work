""" Gabriel de Sa
UTA ID: 1001676832
Lab 3 Brace Depth April 9, 2020
Python Version 3.7.7 Mac Os Catalina(10.15.3) """


def main():
    fileName = "input.txt"
    findDepth(fileName) #calls series of functions that creates the output
    return 0


def findDepth(fileName):
    """Finds the number of braces in a Java file"""
    lineNum = 0
    cnt = 0
    setOfLines = []
    with open(fileName) as file: #opens a file as file
        for cnt, line in enumerate(file): #creates a list of each line iteraters through the line
            for line in enumerate(line): # iterators through each character in the line
                if '{' in line[1]:
                    setOfLines.append(lineNum)     #if the bracket exists add it to be tested for its conditions
                if '}' in line[1]:
                    setOfLines.append(-lineNum)
            lineNum += 1
    file.close()
    checkDef(setOfLines, fileName) #check for the conditions
    return 0


def checkDef(setOfLines, fileName):
    """From a bracket location list checks conditions"""
    validDepth = [] # an array for holding the depth numbers
    tempHold = [] # holds the temporary tokens
    with open(fileName) as file:
        for cnt, line in enumerate(file):
            metBracket = False # tests to see if it is a good bracket or not.
            metQuote = False # test to see if the bracket is inside quotes but can still be in same line.
            # example would be int
            #test("hello") {
            # sout("test");
            # } in order to allow this to work.
            if cnt in setOfLines or -cnt in setOfLines:
                tempHold = line.split(' ')
            for i in range(len(tempHold)):
                if metBracket is False:
                    if "//" in tempHold[i]: # if // are found and no brackets skip line
                        break
                    if '"' in tempHold[i]: # if quotes are found try to find next quotes
                        metQuote = True
                    if '"' in tempHold[i] and metQuote is True: #once found mark false
                        metQuote = False
                    elif '{' in tempHold[i] or '}' in tempHold[i] and metQuote is False:
                        if cnt in setOfLines or -cnt in setOfLines: #negative indicates closing brackets
                            if '}' in tempHold[i]:
                                validDepth.append(-cnt)
                            else:
                                validDepth.append(cnt)
                        metBracket = True
    file.close()
    printWithDepth(validDepth, fileName) # call print
    return 0


def printWithDepth(validDepth, fileName):
    """Prints file with depth indicators"""
    depthIndicator = 0 #depth indicator as in the example
    with open(fileName) as file: # open file to print lines
        for cnt, line in enumerate(file):
            if cnt in validDepth:
                depthIndicator += 1 #add bracket depth
            print("{} {}".format(depthIndicator, line))
            if -cnt in validDepth:
                depthIndicator -= 1 #close bracket depth
    file.close()
    return 0


if __name__ == '__main__':
    main()
