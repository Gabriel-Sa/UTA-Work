# Gabriel de Sa 1001676832

import numpy as np
from tensorflow import keras, pad
from tensorflow.keras import layers

def train_model(model, training_inputs, training_labels, batch_size, epochs):
    model.compile(optimizer='adam', loss=keras.losses.SparseCategoricalCrossentropy())
    model.fit(training_inputs, training_labels, batch_size=batch_size, epochs=epochs)   

def load_and_refine(file, training_inputs, training_labels, batch_size, epochs):
    # load model 
    model = keras.models.load_model(file)
    # Make new model with new output layer
    num_layers = len(model.layers)
    # Reshape inputs 
    num_inputs = len(training_inputs)
    inputs = np.zeros(shape=[num_inputs, 32, 32, 3])
    inputs[:, :, :, :] = np.expand_dims(pad(training_inputs, paddings=[[0, 0], [2, 2], [2, 2]]), -1)
    input_shape = inputs[0].shape
    print(input_shape)
    # Create new model
    num_classes = np.max(training_labels) + 1
    refined_model = keras.Sequential([keras.Input(shape=input_shape)]+
                                    model.layers[0:num_layers-1]+
                                    [layers.Dense(num_classes, activation="softmax")])
    # disable alteration of hidden layer weightsnum
    for layer in refined_model.layers[0:num_layers-1]:
        layer.trainable = False
    # Refine the model
    refined_model.summary()
    refined_model.compile(optimizer='adam', loss=keras.losses.SparseCategoricalCrossentropy(), metrics=['accuracy'])
    refined_model.fit(inputs, training_labels, batch_size=batch_size, epochs=epochs)   
    # Return the model
    return refined_model

def evaluate_my_model(model, test_inputs, test_labels):
    num_inputs = len(test_inputs)
    inputs = np.zeros(shape=[num_inputs, 32, 32, 3])
    inputs[:, :, :, :] = np.expand_dims(pad(test_inputs, paddings=[[0, 0], [2, 2], [2, 2]]), -1)
    loss, accuracy = model.evaluate(inputs, test_labels, verbose=0)
    return accuracy