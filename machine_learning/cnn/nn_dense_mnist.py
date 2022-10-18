# Gabriel de Sa 1001676832
import numpy as np
from tensorflow import keras
from keras.datasets import mnist
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
        (i+1, predClass, actualClass, accuracy))
    print("Classification Accuracy: %6.4f\n" % (classificationAccuracy / len(testData)))
    return classificationAccuracy

def feedForward(layers, units, epochs, hiddenActivation):
    (trainingData, trainingLabels), (testingData, testingLabels) = mnist.load_data()         # load mnist dataset 
    trainingData = trainingData.reshape((trainingData.shape[0], 28*28)).astype("float32")    # Make dataset 1 dimensional 
    testingData = testingData.reshape((testingData.shape[0], 28*28)).astype("float32")
    trainingData = trainingData / max(trainingData.min(), trainingData.max(), key=abs)       # Normalize dataset between 0 and 1
    testingData = testingData / max(testingData.min(), testingData.max(), key=abs)
    inputShape = trainingData[0].shape                                                       # Get the input shape 
    numClasses = 10                                                                          # Mnist dataset has an output shape of 10. 
    modelArray = [keras.Input(shape = inputShape)]                                           # Create the model shape based off user input
    for layer in range(layers):
        modelArray.append(keras.layers.Dense(units, activation=hiddenActivation))
    modelArray.append(keras.layers.Dense(numClasses, activation="softmax"))
    model = keras.Sequential(modelArray)                                                     # Create the model 
    model.compile(optimizer = 'adam', loss = keras.losses.SparseCategoricalCrossentropy(), metrics=['accuracy'])
    model.fit(trainingData, trainingLabels, epochs = epochs)                                 # Train the model
    return classification(model, testingData, testingLabels)

def main():
    return feedForward(
        int(sys.argv[1]),
        int(sys.argv[2]),
        int(sys.argv[3]),
        sys.argv[4]) 

main()