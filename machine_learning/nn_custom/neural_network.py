# Gabriel de Sa 1001676832
import sys
import numpy as np

def openFile(fileName):
    returnArray = []
    classArray = []
    with open(fileName) as file:
        lines = file.readlines()
        for i, line in enumerate(lines):
            line = line.split()
            returnArray.append([])
            for x in line:
                if len(returnArray[i]) < len(line)-1:
                    returnArray[i].append(float(x))
                else:
                    classArray.append(x)
    return (np.array(returnArray), np.array(classArray))

def createClassLabels(classArray):
    classes = []
    retArray = np.zeros_like(classArray, dtype="object")
    classes = np.unique(classArray)
    for i, item in enumerate(classArray):
        retArray[i] = np.zeros_like(classes, dtype=int)
        position = np.where(classes == item)[0][0]
        retArray[i][position] = 1
    return classes, retArray

def createWeightsAndBiases(dataLength, classesLength, layers, units):
    # Create neural network matrices. Including Bias and Weights
    # for each layer in the nn have a matrices of weights connecting them.
    # Fully connected layers, means each layer has layer-1 * layer weights
    weights = [[0] for i in range(layers+1)]
    # if there are only two layers, then tie inputs to outputs.
    if layers == 2:
        weights[-1] = np.matrix(np.random.uniform(-0.05, 0.05, (dataLength, classesLength)))
    else:
        weights[2] = np.matrix(np.random.uniform(-0.05, 0.05, (dataLength, units)))
        for layer in range(3, layers):
            weights[layer] = np.matrix(np.random.uniform(-0.05, 0.05, (units, units)))
        weights[-1] = np.matrix(np.random.uniform(-0.05, 0.05, (units, classesLength)))
    # have a bias for each perceptron in each layer
    biases = [[1 for i in range(units)] for i in range(layers+1)]
    biases[-1] = [1 for i in range(classesLength)]
    return weights, biases

def activation(x):
    return 1/(1+np.exp(-x))

def train(data, classes, classLabels, tHotLabels, layers, units, epocs):
    # Normalize Data
    absMax = max(data.min(), data.max(), key=abs)
    normalizedData = data/absMax
    # Generate Weights and Biases
    weights, biases = createWeightsAndBiases(len(data[0]), len(classLabels), layers, units)
    lr = 1
    for epoc in range(epocs):
        for i, example in enumerate(normalizedData):
            # Feed forward
            z = [0 for x in range(layers+1)]
            z[1] = np.array(example)
            for k in range(2, layers+1):
                a = np.array(biases[k] + np.matmul(z[k-1], weights[k]))[0]
                z[k] = [activation(x) for x in a] # Give the whole array instead doing it iteratively
            # Back Propagation
            error = [0 for x in range(layers+1)]
            error[-1] = (np.array(z[-1] - tHotLabels[i]) * np.array(z[-1]) * np.array(1 - np.array(z[-1])))
            for l in range(layers-1, 1, -1):
                error[l] = (np.dot(error[l+1], np.array(weights[l+1].transpose())) * np.array(z[l]) * np.array(1 - np.array(z[l])))
            for l in range(2, layers+1):
                biases[l] = biases[l] - (lr * error[l])
                tempWeight = np.array(weights[l], dtype=object)
                tempZ = np.array(z[l-1], dtype=object)
                for i in range(weights[l].shape[1]):
                    for j in range(weights[l].shape[0]):
                        tempWeight[j][i] = tempWeight[j][i] - (lr * error[l][i] * tempZ[j])
                weights[l] = np.matrix(tempWeight)
        lr = lr * .98
    return weights, biases

def feedForward(inputs, classLabels, tHots, weights, biases, layers):
    # Normalize Data
    absMax = max(inputs.min(), inputs.max(), key=abs)
    normalizedData = inputs/absMax
    # Feed forward
    totalAccuracy = 0
    for i, example in enumerate(normalizedData):
        z = [0 for x in range(layers+1)]
        z[1] = np.array(example)
        for k in range(2, layers+1):
            a = np.array(biases[k] + np.matmul(z[k-1], weights[k]))[0]
            z[k] = [activation(x) for x in a]
        # Classification Checking
        guess = classLabels[np.argmax(z[-1])]
        trueClass = classLabels[np.argmax(tHots[i])]
        guessValue = max(z[-1])
        if guess == trueClass:
            # checks for guesses that are ties, if the lowest element is the correct guess.
            accuracy = 1/((z[-1] == guessValue).sum())
            totalAccuracy += accuracy
        elif (z[-1] == guessValue).sum() > 1:
            trueIndex = np.argmax(tHots[i])
            for i, val in enumerate(z[-1]):
                if val == guessValue and i == trueIndex:
                    accuracy = 1/((z[-1] == guessValue).sum())
                    break
                else:
                    continue
        else:
            accuracy = 0
        print("ID=5%d, predicted=%10s, true=%10s, accuracy=%4.2f" % (i+1, guess, trueClass, accuracy))
    print("Classification Accuracy = %6.4f" % (totalAccuracy/len(inputs)))
    return totalAccuracy/len(inputs)

def main(training_file, test_file, layers, units, epoc):
    trainingData, trainingClasses = openFile(training_file)
    testingData, testingClasses = openFile(test_file)
    trainingClassLabels, trainingTHot = createClassLabels(trainingClasses)
    testingClassLabels, testingTHot = createClassLabels(testingClasses)
    weights, biases = train(trainingData, trainingClasses, trainingClassLabels, trainingTHot, int(layers), int(units), int(epoc))
    return feedForward(testingData, testingClassLabels, testingTHot, weights, biases, int(layers))


if __name__ == '__main__':
    main(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5])
