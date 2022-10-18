# Gabriel de Sa 1001676832
from tensorflow import keras
import tensorflow as tf
import numpy as np
import os 
import shutil


def create_train_files(train_files):
    dataset = []
    tokenizer = keras.layers.TextVectorization(max_tokens=25000, output_mode="binary", ngrams=2)
    num_authors = 0
    for author in train_files:
        temp_set = []
        for text in author:
            with open(text, "rb") as file:
                temp_set.append(file.read().decode('ISO-8859-1'))
        dataset.append((temp_set, num_authors))
        num_authors = num_authors + 1
    dataset = np.array(dataset)
    dataset = np.array(["".join(text) for text in dataset[:, 0]])
    tokenizer.adapt(dataset)
    # Segment Data into trainable strings. 
    segmented_data = [np.array([string for string in dataset[i].split(".")]) for i in range(len(dataset))]
    segmented_data = np.array(list(map(lambda x: np.expand_dims(x, axis=1), segmented_data)), dtype=object)
    train_data = np.array([])
    # Create training and labels set
    # Create directory paths
    if not os.path.exists("train_ds"):
        os.mkdir("train_ds")
        for i in range(num_authors):
            directory_string = "train_ds/{}".format(i)
            os.makedirs(directory_string)
    else:
        shutil.rmtree("train_ds")
        os.mkdir("train_ds")
        for i in range(num_authors):
            directory_string = "train_ds/{}".format(i)
            os.makedirs(directory_string)
    # Save files to directories
    max_train_examples = min(list(map(lambda x : len(x), segmented_data)))
    for i in range(num_authors):
        np.random.shuffle(segmented_data[i])
        train_data = segmented_data[i][0:max_train_examples]
        # Write to files
        for k, excerpt in enumerate(train_data):
            excerpt_path = "train_ds/{}/example_{}.txt".format(i, k)
            with open(excerpt_path, "w") as file: 
                file.write(excerpt[0])
    return tokenizer

def learn_model(train_files):
    # Create Tokenizer and training files
    tokenizer = create_train_files(train_files)
    # Tokenize Inputs 
    batch_size = 32
    train_ds = keras.utils.text_dataset_from_directory("train_ds/",  batch_size=batch_size)
    train_data = train_ds.map(lambda x, y: (tokenizer(x), y))
    num_authors = len(train_ds.class_names)
    # Configure Cache 
    train_data = train_data.cache().prefetch(buffer_size=tf.data.AUTOTUNE)
    # Train Model
    max_tokens = 25000
    # Bag Of Words Model
    model = keras.Sequential([
        keras.layers.Dense(32),
        keras.layers.Dropout(0.2),
        keras.layers.Dense(16)
        ])
    model.compile(optimizer='adam', loss=keras.losses.SparseCategoricalCrossentropy(from_logits=True), metrics=['accuracy'])
    model.fit(train_data, epochs=10)
    # Export model
    export_model = keras.Sequential(
        [tokenizer, model, 
        keras.layers.Activation("sigmoid")])
    export_model.compile(optimizer = 'adam', loss=keras.losses.SparseCategoricalCrossentropy(from_logits=False), metrics=['accuracy'])
    return export_model

