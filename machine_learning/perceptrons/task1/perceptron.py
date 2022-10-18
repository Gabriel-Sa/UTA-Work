# Gabriel de Sa 1001676832
import sys
import numpy as np


def getWeights(weights):
    weight = []
    with open(weights) as file:
        for line in file.readlines():
            weight.append(float(line))
    return weight[1:], weight[0]


def getInputs(input):
    inputs = []
    with open(input) as file:
        for line in file.readlines():
            inputs.append(float(line))
    return inputs


def sigmoid(x):
    return 1/(1+np.exp(-x))


def step(x):
    return 0 if (x < 0) else 1


def generateGuess(weights, bias, inputs, activation):
    a = bias + np.matmul(np.array(weights).transpose(), np.array(inputs))
    z = activation(a)
    return a, z


def perceptron(weight, input, activation):
    weights, bias = getWeights(weight)
    inputs = getInputs(input)
    if "sigmoid" in activation:
        a, z = generateGuess(weights, bias, inputs, sigmoid)
    else:
        a, z = generateGuess(weights, bias, inputs, step)
    return a, z


a, z = perceptron(sys.argv[1], sys.argv[2], sys.argv[3])
print("a = %.4f\nz = %.4f" % (a, z))
