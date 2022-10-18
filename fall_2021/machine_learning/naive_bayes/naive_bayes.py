# Gabriel de Sa 1001676832
import sys as sys
import numpy as np
import math


def computeRow(row):
    stdDev = 0
    avg = 0
    sum = 0
    length = len(row)
    for num in row:
        sum += num
    average = sum / length
    sum = 0
    for num in row:
        sum += ((num - average)**2)
    stdDev = math.sqrt(sum/(length-1))
    if stdDev < 0.01:
        stdDev = 0.01
    return average, stdDev


def naive_bayes(training_file, test_file):
    training_data = []
    test_data = []
    # open training and test files and organize each row into an array
    with open(training_file, "r") as file:
        lines = file.readlines()
        for line in lines:
            training_data.append(line.split())
    with open(test_file, "r") as file:
        lines = file.readlines()
        for line in lines:
            test_data.append(line.split())
    # Get the length of the array and get the class index (assumed to be last rwo)
    classIndex = len(training_data[0]) - 1
    attributeClassList = {}  # create a dictionary to hold the class, index keys
    meanAndStd = {}
    pOfC = {}
    # Training
    for training_row in training_data:
        trainingClass = int(training_row[classIndex])  # get the class
        # iterate through the training data up to the final row (class)
        for attributeNumber, attributeValue in enumerate(training_row[0:classIndex]):
            if (trainingClass, attributeNumber+1) not in attributeClassList:  # add array for key
                attributeClassList[(trainingClass, attributeNumber+1)] = []
                attributeClassList[(trainingClass, attributeNumber+1)].append(float(attributeValue))
            else:
                attributeClassList[(trainingClass, attributeNumber+1)].append(float(attributeValue))
    total = 0
    runningTotal = 0
    Class = list(attributeClassList.keys())[0][0]
    # find the standard deviation and mean for each class, attribute
    for key in sorted(attributeClassList.keys()):
        meanAndStd[key] = computeRow(attributeClassList[key])
        if Class != key[0]:
            pOfC[Class] = total
            Class = key[0]
            total = 0
        total = total + len(attributeClassList[key])
        runningTotal = runningTotal + len(attributeClassList[key])
        # using these calculate the gaussian density
        print("Class %d, Attribute %d, Mean = %.2f, Std Dev. = %.2f" %
              (key[0], key[1], meanAndStd[key][0], meanAndStd[key][1]))
    else:
        pOfC[Class] = total
        # compute P(C)
        for key in pOfC:
            pOfC[key] = pOfC[key]/runningTotal
    # When training we need to use bayes rule to compute P(C | x), us POfC, POfX and P(x | C) is the
    # gaussian of each attribute class producted together. P(x) can be found using sum rule
    # testing phase
    overAllAccuracy = 0
    count = 1
    for testing_row in test_data:
        accurateClass = int(testing_row[classIndex])
        pOfXCDict = {}
        pOfX = 0
        for currentClass in pOfC.keys():
            pOfXC = 1
            for attributeNumber, attributeValue in enumerate(testing_row[0:classIndex]):
                # compute probability using gaussian
                mean = meanAndStd[currentClass, attributeNumber+1][0]
                std = meanAndStd[currentClass, attributeNumber+1][1]
                pOfXC = pOfXC * 1/(std*math.sqrt(2*np.pi)) * \
                    np.exp(-((float(attributeValue)-mean)**2)/(2*(std**2)))
            pOfXCDict[currentClass] = pOfXC
        for c in pOfXCDict:
            pOfX = (pOfX + (pOfXCDict[c]*pOfC[c]))
        max = 0
        classGuess = []
        accuracy = 0
        for c in pOfC.keys():
            pOfCX = (pOfXCDict[c]*pOfC[c])/pOfX
            if max < pOfCX:
                max = pOfCX
                classGuess = [c]
            elif max == pOfCX:
                classGuess.append(c)
        if accurateClass in classGuess:
            guesses = len(classGuess)
            if guesses > 1:
                accuracy = 1 / guesses
                classGuess[0] = accurateClass
            else:
                accuracy = 1
        else:
            accuracy = 0
        print("ID = %5d, Guessed Class = %3d, Confidence = %.4f, Actual Class = %3d, Accuracy = %4.2f" %
              (count, classGuess[0], max, accurateClass, accuracy))
        overAllAccuracy = overAllAccuracy + accuracy
        count = count + 1
    return overAllAccuracy/count


accuracy = naive_bayes(sys.argv[1], sys.argv[2])
print("Classification Accuracy = %6.4f" % accuracy)
