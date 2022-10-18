# Gabriel de Sa 1001676832 
from pickletools import optimize
import numpy as np
from numpy import average, std
from tensorflow import keras


def data_normalization(raw_data, train_start, train_end):
    """
    Normalizes data so that the mean for each feature is 0 and the standard deviation for each feature is 1.\n
    Parameters:\n
    raw_data: the time series of weather observations in csv file format.\n
    train_start, train_end: specifiers, respectively, the start and end of the segment fo raw_data that should be used for training.\n
    Returns: \n
    normalized_data: time series of the raw data from train_start to train_end.\n
    """
    norm_data = raw_data - raw_data[:train_end].mean(axis=0)
    norm_data = norm_data / norm_data[:train_end].std(axis=0)
    return norm_data

def make_inputs_and_targets(data, months, size, sampling):
    """
    Creates a set of input objects and target values, that can be used as a training, validation or test set.\n
    Parameters:\n
    data: a time series dataset containing training, validation, and test segements.\n
    months: A time series of target values for data. So that months[i] is the correct month for the momemnt in time in which data[i] was recorded. months[0] = december months[11] = novemeber.\n
    size: this specifies the size of the resulting set of inputs and targets in thousands . Example: Size == 10, then 10,000 inputs and 10,000 target values.\n
    sampling: sepcifies how to sample the values in data, reducing length of input vector by a factor of sampling. \n
    Returns:\n 
    A new set dataset containing the adjusted inputs and targets.\n 
    inputs: A three-dimensional numpy array. The shape of the array is [size, observations/sampling, 14]. Each observation in input is a two-week collection of observations at the sampling rate. \n
    targets: These are the target values for the inputs. Month corresponding to the mid-point of observations at respective inputs index. 
    """
    observationsCount = 2016 
    monthsIncrementer = int((observationsCount/sampling)/2)
    if(size+observationsCount > len(data)):
        print("Data selection is too large, shrinking to match data shape.")
        size = len(data) - observationsCount
    inputs = []
    targets = np.empty(size)
    maxRand = len(data) - observationsCount
    for i in range(0, size):
        inputs.append([])
        rand = np.random.randint(0, maxRand)
        targets[i] = months[rand+monthsIncrementer]
        for k in range(0, observationsCount, sampling):
            inputs[i].append(data[rand+k])

    return np.array(inputs), targets

def build_and_train_dense(train_inputs, train_targets, val_inputs, val_targets, filename):
    """
    Trains a fully connected model using the given training inputs and training labels.\n
    Trains the model for 10 epochs using the "Adam optimizer", using default batch_size. 
    Parameters:\n
    train_inputs: The training data.\n 
    training_targets: the training data labels\n
    val_inputs: validation data. \n
    val_targets: validation data labels.\n
    filename: file path to save model to. \n 
    Returns:\n 
    Training history, obtained from model.fit(). 
    """
    inputShape = train_inputs[0].shape
    numClasses = 12
    modelArray = [
        keras.Input(shape = inputShape),
        keras.layers.Flatten(), 
        keras.layers.Dense(64, activation="tanh"), 
        keras.layers.Dense(32, activation="tanh"), 
        keras.layers.Dense(numClasses, activation="softmax")]
    model = keras.Sequential(modelArray)
    model.compile(optimizer = "adam", loss=keras.losses.SparseCategoricalCrossentropy(), metrics=['accuracy'])
    history = model.fit(train_inputs, train_targets, epochs=10, validation_data=(val_inputs, val_targets),\
        callbacks=[keras.callbacks.ModelCheckpoint(filename, save_best_only=True)], batch_size = 512)
    return history

def build_and_train_rnn(train_inputs, train_targets, val_inputs, val_targets, filename):
    """
    Trains a RNN model using the given training inputs and training labels.\n
    Trains the model for 10 epochs using the "Adam optimizer", using default batch_size. 
    Parameters:\n
    train_inputs: The training data.\n 
    training_targets: the training data labels\n
    val_inputs: validation data. \n
    val_targets: validation data labels.\n
    filename: file path to save model to. \n 
    Returns:\n 
    Training history, obtained from model.fit(). 
    """
    inputShape = train_inputs[0].shape
    numClasses = 12
    modelArray = [
        keras.Input(shape = inputShape),
        keras.layers.Bidirectional(keras.layers.LSTM(32)), 
        keras.layers.Dense(numClasses, activation="softmax")]
    model = keras.Sequential(modelArray)
    model.compile(optimizer = "adam", loss=keras.losses.SparseCategoricalCrossentropy(), metrics=['accuracy'])
    history = model.fit(train_inputs, train_targets, epochs=10, validation_data=(val_inputs, val_targets),\
        callbacks=[keras.callbacks.ModelCheckpoint(filename, save_best_only=True)], batch_size=512)
    return history

def test_model(filename, test_inputs, test_targets):
    """
    Computes classification accuracy of model contained at filename.\n
    Parameters:\n 
    filename: path to file containing desired model for testing.\n
    test_inputs: dataset of observations to test model.\n 
    test_targets: the labels of the respective dataset entries.\n 
    Returns:\n
    test_accuracy returned by Keras.model.evaluate(...)[1]
    """
    model = keras.models.load_model(filename)
    loss, accuracy = model.evaluate(test_inputs, test_targets, verbose=0)
    return accuracy

def confusion_matrix(filename, test_inputs, test_targets):
    """
    Computes the confusion matrix of the model saved on the given filename, using the given test_inputs, test_targets. 
    The confusion matrix is 12x12 matrix, where at row i and column j, stores the number of test objects that had true class label i and were classified by the model as having clas slabel j.\n
    Parameters:\n
    filename: contains the model that the confusion matrix should be generated from. \n
    test_inputs, test_targets: the data containing the observations, and their respective labels.\n
    Returns:\n
    confusion_matrix: A 12x12 confusion matrix.
    """
    confusion_matrix = np.zeros((12, 12))
    model = keras.models.load_model(filename)
    # confusion_matrix[True][Predicted]
    predictions = model.predict(test_inputs).argmax(axis=1)
    for true_class, predicted in zip(test_targets, predictions):
        confusion_matrix[int(true_class)][predicted] = confusion_matrix[int(true_class)][predicted] + 1
    return confusion_matrix
