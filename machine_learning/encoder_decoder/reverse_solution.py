# Gabriel de Sa 1001676832
from json import decoder
import tensorflow as tf
from tensorflow import keras 
from keras import layers
import numpy as np
import string
import re

from transformers_common import PositionalEmbedding, TransformerEncoder

# From reverse_common don't want to risk importing and the professor not using it. 
def standardize(strings):
    tv = layers.TextVectorization(output_mode="int")
    tv.adapt(strings)
    vocab = tv.get_vocabulary()
    vectors = tv(strings).numpy()
    
    result = []
    for vector in vectors:
        ints = vector[vector > 0]
        words = [vocab[i] for i in ints]
        text = " ".join(words)
        result = result + [text]
    
    return result

def tokenizerStand(text):
    strip_chars = string.punctuation
    strip_chars = strip_chars.replace("[", "")
    strip_chars = strip_chars.replace("]", "")
    lowercase = tf.strings.lower(text)
    return tf.strings.regex_replace(lowercase, f"[{re.escape(strip_chars)}]", "")

def rand_reverse(input):
    if np.random.randint(0,2) == 0:
        return input
    else: 
        return " ".join(list(reversed(input.split())))

def replace3(x):
    position3 = np.where(x==3)[0][0]
    x[position3] = 0
    return x

def create_datasets(data, targets, source, target):
    vectorized_targets = target(targets)   # Create tokenized version of target
    np_edited_targets = vectorized_targets.numpy() 
    replaced3s = list(map(lambda x : replace3(x), np_edited_targets)) # remove threes
    reversed_inputs = list(map(rand_reverse, data)) # Reverse some inputs
    inputs = source(reversed_inputs) # create tokenized version of inputs
    dataset = tf.data.Dataset.from_tensor_slices(({
                                                "inputs": inputs, 
                                                "targets": np.array(replaced3s)[:, :-1]},
                                                vectorized_targets[:, 1:]))
    dataset = dataset.batch(64)
    return dataset.shuffle(2048).prefetch(buffer_size=tf.data.AUTOTUNE).cache()

def train_enc_dec(train_sentences, validation_sentences, epochs):
    """Train an encoder-decoder RNN model for finding grammatically correct sentences."""
    # Generate and adapt textvectorization layers
    source_vect = layers.TextVectorization(
        max_tokens=250, 
        output_mode="int")
    target_vect = layers.TextVectorization(
        max_tokens=252,
        output_mode="int",
        standardize=tokenizerStand)
    targets = standardize(train_sentences)
    edited_targets = list(map(lambda x : "[start] " + x + " [end]", targets))
    source_vect.adapt(targets)
    target_vect.adapt(edited_targets)
    # Create datasets and adapt tokenizer
    validation_targets = standardize(validation_sentences)
    val_edited_targets = list(map(lambda x : "[start] " + x + " [end]", validation_targets))
    inputs = create_datasets(targets, edited_targets, source_vect, target_vect)
    validation = create_datasets(validation_targets, val_edited_targets, source_vect, target_vect)
    # Create Model
    embed_dims = 64
    latent_dims = 64
    # Encoder
    encoder_input = keras.Input(shape=(None,), dtype="int64", name="inputs")
    x1 = layers.Embedding(250, embed_dims, mask_zero=True)(encoder_input)
    encoded_source = layers.Bidirectional(layers.GRU(latent_dims), merge_mode="sum")(x1)
    # Decoder 
    decoder_input = keras.Input(shape=(None,), dtype="int64", name="targets")
    x2 = layers.Embedding(252, embed_dims, mask_zero=True)(decoder_input)
    decoder_gru = layers.GRU(latent_dims, return_sequences=True)
    x3 = decoder_gru(x2, initial_state=encoded_source)
    x4 = layers.Dropout(0.5)(x3)
    tns = layers.Dense(252, activation="softmax")(x4)
    model = keras.Model([encoder_input, decoder_input], tns)
    # Train model
    file = "enc_dec_rnn.keras"
    callbacks = [keras.callbacks.ModelCheckpoint(file, save_best_only=True)]
    model.compile(
        optimizer="rmsprop", 
        loss="sparse_categorical_crossentropy", 
        metrics=["accuracy"])
    model.fit(inputs, epochs=epochs, validation_data=validation, callbacks=callbacks)
    return model, source_vect, target_vect

def evaluate(sentence, model, target):
    recurrent_sentence = "[start]"
    ret_sentence = ""
    decoded_output = ""
    vocab = target.get_vocabulary()
    for i in range(9):
        decoder_input = target([recurrent_sentence])
        latest_output = model.predict([sentence, decoder_input])
        decoded_output = vocab[np.argmax(latest_output[0, i, :])]
        recurrent_sentence = recurrent_sentence + " " + decoded_output
        if decoded_output != "[end]":
            ret_sentence = ret_sentence + " " + decoded_output
        else:
            break
    return ret_sentence

def get_enc_dec_results(model, test_sentences, source_vec_layer, target_vec_layer):
    """Generate output given input sentence."""
    predictions = []
    predictions = list(map(lambda x : evaluate(source_vec_layer([x]), model, target_vec_layer), test_sentences))
    return predictions

def segmentInputs(reversed, regular, source):
    inputs = source(reversed)
    targets = [] # Where 0 is in correct format and 1 is in not correct format. 
    for input, target in zip(reversed, regular):
        if input == target:
            targets.append(0)
        else:
            targets.append(1)
    return (inputs, tf.convert_to_tensor(targets))

def train_best_model(train_sentences, validation_sentences, epochs):
    # Create a transformer model that measures whether or not the output is in the correct orientation. 
    # If not then simply put no. So labels will be either yes or no. If no then manually reverse the input.
    # 8 ---- Hidden ----- 2-> 
    # Standardize inputs and outputs. 
    # Create dataset consisting of "yes", "no" that contains the correct inputs or not. 
    # Generate and adapt textvectorization layers
    source_vect = layers.TextVectorization(
        max_tokens=250, 
        output_mode="int")
    targets = standardize(train_sentences)
    source_vect.adapt(targets)
    reversed = list(map(lambda x : rand_reverse(x), targets))
    train_dataset = tf.data.Dataset.from_tensors(segmentInputs(reversed, targets, source_vect))
    validation_targets = standardize(validation_sentences)
    reversed_validation = list(map(lambda x : rand_reverse(x), validation_targets))
    validation_dataset = tf.data.Dataset.from_tensors(segmentInputs(reversed_validation, validation_targets, source_vect))
    # Create transformer model (using transformer common module from professor. )
    train_dataset.shuffle(2048).prefetch(buffer_size=tf.data.AUTOTUNE).cache()
    validation_dataset.shuffle(2048).prefetch(buffer_size=tf.data.AUTOTUNE).cache()
    embed_dims = 80
    latent_dims = 80
    sequence_length = 8
    heads = 3
    inputs = keras.Input(shape=(None,), dtype="int64")
    x = PositionalEmbedding(sequence_length, 250, embed_dims)(inputs)
    x = TransformerEncoder(embed_dims, latent_dims, heads)(x)
    x = layers.GlobalAveragePooling1D()(x)
    x = layers.Dropout(0.3)(x)
    outputs = layers.Dense(1, activation="sigmoid")(x)
    model = keras.Model(inputs, outputs)
    # Train model
    callbacks = [keras.callbacks.ModelCheckpoint("best_model.keras", save_best_only=True)]
    model.compile(
        optimizer="rmsprop", 
        loss="binary_crossentropy", 
        metrics=["accuracy"])
    model.fit(train_dataset, epochs=epochs, validation_data=validation_dataset, callbacks=callbacks)
    return model, source_vect, None

def get_best_model_results(model, test_sentences, source_vec_layer, target_vec_layer):
    # Standardize input sentences. 
    inputs = standardize(test_sentences)
    vec_inputs = source_vec_layer(inputs)
    outputs = model.predict(vec_inputs)
    retArray = []
    for sentence, output in zip(inputs, outputs): 
        if (output[0] < .5):
            retArray.append(sentence)
        else:
            retArray.append(" ".join(list(reversed(sentence.split()))))
    return retArray