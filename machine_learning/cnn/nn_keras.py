# Gabriel de Sa 1001676832
import numpy as np
from tensorflow import keras
from uci_data import read_uci_file
import sys

def classification(model, testData, testLabels):
    classificationAccuracy = 0
    testDataIterable = np.expand_dims(testData, 1)
    for i, object in enumerate(testDataIterable):
        output = model.predict(object).flatten()
        predClass = np.argmax(output)
        actualClass = testLabels[i]
        (ind,) = np.nonzero(output == output[predClass])
        ties = np.prod(ind.shape)
        if(predClass == actualClass):
            accuracy = 1 / ties
            classificationAccuracy = classificationAccuracy + accuracy
        else: 
            accuracy = 0 
        print("ID=%5d, Predicted Class = %10s, True Class = %10s, Accuracy = %4.2f" % 
        (i+1, predClass, actualClass[0], accuracy))
    print("Classification Accuracy: %6.4f\n" % (classificationAccuracy / len(testData)))
    return classificationAccuracy

def nn_keras(datasets, layers, units, epochs, activationFunction):
    inputShape = datasets["trainingData"][0].shape
    numClasses = np.max([np.max(datasets["trainingLabels"]), np.max(datasets["testingLabels"])]) + 1
    modelArray = [keras.Input(shape = inputShape)]
    for layer in range(layers): 
        modelArray.append(keras.layers.Dense(units, activation = activationFunction))
    modelArray.append(keras.layers.Dense(numClasses, activation = "softmax"))
    model = keras.Sequential(modelArray)
    model.compile(optimizer = 'adam', loss = keras.losses.SparseCategoricalCrossentropy(), metrics=['accuracy'])
    model.summary()
    model.fit(datasets["trainingData"], datasets["trainingLabels"], epochs = epochs)
    return classification(model, datasets["testingData"], datasets["testingLabels"])

def main():
    labels_to_ints = {}
    ints_to_labels = {}
    (training_data, training_labels) = read_uci_file(sys.argv[1], labels_to_ints, ints_to_labels)
    (testing_data, testing_labels) = read_uci_file(sys.argv[2], labels_to_ints, ints_to_labels)
    training_data = training_data / max(training_data.min(), training_data.max(), key=abs)
    testing_data = testing_data / max(testing_data.min(), testing_data.max(), key=abs)
    dataDict = {
    "trainingData": training_data, 
    "trainingLabels": training_labels,
    "testingData": testing_data,
    "testingLabels": testing_labels
    }
    return nn_keras(
        dataDict,
        int(sys.argv[3]),
        int(sys.argv[4]), 
        int(sys.argv[5]), 
        sys.argv[6])

main()
