# Gabriel de Sa
# UTA ID: 1001676832
# April 23, 2020 Lab 5 Python 3.7.7
# Mac OS Catalina Version 10.15.4

def checkValid(operator):
    """This function tests to see if the char is valid for this program returns a 1 for a operator, 2 for a digit, and 3 for a invisible"""
    setAllowed = {'+', '-', '*', '/'} # each of these are a test to see if it is a valid RPN equation
    miscAllowed = {' ', '\n', '\t', '\r', '\r\n', '\n\r'}
    if operator in setAllowed:
        return 1
    elif operator.isdigit():
        return 2
    elif operator in miscAllowed:
        return 3
    return -1

def doCalculation(numArray, operator):
    """This function will do the calculation of the array given to it"""
    end = len(numArray) - 1 # gets length of array and does stack calcs using an array
    if operator == '+':
        hold = numArray[end-1] + numArray[end]
        numArray.pop(end) # pop the last off two off the stack and add the new answer
        numArray.pop(end-1)
        numArray.append(hold)
    elif operator == '-':
        hold = numArray[end-1] - numArray[end]
        numArray.pop(end)
        numArray.pop(end-1)
        numArray.append(hold)
    elif operator == '*':
        hold = numArray[end-1] * numArray[end]
        numArray.pop(end)
        numArray.pop(end-1)
        numArray.append(hold)
    elif operator == '/':
        hold = numArray[end-1] / numArray[end]
        numArray.pop(end)
        numArray.pop(end-1)
        numArray.append(hold)
    return 0

def doRPN(line):
    """Goes through each line, makes a stack and does the RPN calculation"""
    numArray = []
    ansArray = []
    hitOp = False
    string = line.split()
    for char in string:
        test = checkValid(char)
        if test == 1:
            if len(numArray) == 1: # if there is one in the stack add it to the final answer stack
                ansArray.append(numArray[0])
                doCalculation(ansArray, char)
            else:
                doCalculation(numArray, char) # else do a regular on stack, this allows us to have multiple steps in one bracket
            hitOp = True
        elif test == 2:
            if hitOp == True:
                ansArray.append(numArray[0]) # does the same thing adds the last num the answer stack
                numArray.clear()
                hitOp = False
            numArray.append(int(char)) # converts a string to a int or float
        elif test == 3:
            continue
        else:
            print("Invalid Syntax Structure for this line, skipping.")
            return 0
    print(ansArray)
    return 0

def getInputRPN(fileName):
    """Reads file and prints out answer line by line"""
    with open(fileName) as file:
        for cnt, line in enumerate(file):
            doRPN(line) # calls for each line the file
    return 0


def main():
    fileName = "input_RPN.txt"
    getInputRPN(fileName) 
    return 0


if __name__ == '__main__':
    main()
