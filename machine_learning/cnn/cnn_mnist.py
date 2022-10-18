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

def createAndTrain(blocks, filterSize, filterNumber, regionSize, epochs, cnnActivation):
    (trainingData, trainingLabels), (testingData, testingLabels) = mnist.load_data()    # load mnist dataset 
    trainingData = np.expand_dims(trainingData, -1).astype("float32")                   # Make dataset 3 dimensional 
    testingData = np.expand_dims(testingData, -1).astype("float32")                     # Make dataset 3 dimensional 
    trainingData = trainingData / max(trainingData.min(), trainingData.max(), key=abs)  # Normalize dataset between 0 and 1
    testingData = testingData / max(testingData.min(), testingData.max(), key=abs)
    inputShape = trainingData[0].shape                                                  # Get the input shape 
    numClasses = 10                                                                     # Mnist dataset has an output shape of 10. 
    # Create Model
    modelArray = [keras.Input(shape=inputShape)]
    for block in range(blocks):
        modelArray.append(keras.layers.Conv2D(filterNumber, kernel_size=(filterSize, filterSize), activation=cnnActivation))
        modelArray.append(keras.layers.MaxPooling2D(pool_size=(regionSize, regionSize)))
    modelArray.append(keras.layers.Flatten())
    modelArray.append(keras.layers.Dropout(0.5))
    modelArray.append(keras.layers.Dense(numClasses, activation="softmax"))
    model = keras.Sequential(modelArray)
    model.summary()
    batchSize = 128
    model.compile(optimizer = 'adam', loss = keras.losses.SparseCategoricalCrossentropy(), metrics=['accuracy'])
    # Train the model 
    model.fit(trainingData, trainingLabels, epochs=epochs, batch_size=batchSize)
    # Test Model 
    return classification(model, testingData, testingLabels)

def main():
    return createAndTrain(
        int(sys.argv[1]), 
        int(sys.argv[2]),
        int(sys.argv[3]),
        int(sys.argv[4]),
        int(sys.argv[5]),
        sys.argv[6]) 

main()