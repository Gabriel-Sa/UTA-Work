#%%
# Gabriel de Sa 1001676832
import numpy as np
import tensorflow as tf
from tensorflow import keras
from keras import layers
from keras.layers import TextVectorization
import random
import string
import re

from reverse_common import *
from reverse_solution import *

tf.config.set_visible_devices([], 'GPU')
tf.device('/cpu:0')

train_sentences = load_strings("dataset/reverse_train.txt")
validation_sentences = load_strings("dataset/reverse_validation.txt")
(test_sources, test_targets) = load_pairs("dataset/reverse_test.txt")
                                                      
print("read %d training sentences" % (len(train_sentences)))
print("read %d validation sentences" % (len(validation_sentences)))
print("read %d test examples" % (len(test_sources)))

#%%
# here we are calling the train_enc_dec function that you need to write.
(model, source_vec_layer, target_vec_layer) = train_enc_dec(train_sentences, 
                                                            validation_sentences,
                                                            150)

number = len(test_sources) # use this line to test on the whole test set.
#number = 100 #  use this line to test on only 100 test objects, for quicker results.
(small_test_sources, small_test_targets) = random_samples(test_sources, 
                                                          test_targets, 
                                                          number)
                                                    
# here we are calling the get_enc_dec_results function that you need to write.
results = get_enc_dec_results(model, small_test_sources,
                              source_vec_layer, target_vec_layer)

wa = word_accuracy(results, small_test_targets)
print("Encoder-decoder word accuracy = %.2f%%" % (wa * 100))

#%%
# here we are calling the train_best_model function that you need to write.
(model, source_vec_layer, target_vec_layer) = train_best_model(train_sentences, 
                                                               validation_sentences, 150)


number = len(test_sources) # use this line to test on the whole test set.
#number = 100 #  use this line to test on only 100 test objects, for quicker results.
(small_test_sources, small_test_targets) = random_samples(test_sources, 
                                                          test_targets, 
                                                          number)

# here we are calling the get_best_model_results function that you need to write.
results = get_best_model_results(model, small_test_sources,
                                 source_vec_layer, target_vec_layer)

wa = word_accuracy(results, small_test_targets)
print("Best model word accuracy = %.2f%%" % (wa * 100))
# %%
