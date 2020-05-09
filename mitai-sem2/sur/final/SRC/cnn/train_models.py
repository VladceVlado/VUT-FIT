#!/usr/bin/env python3

"""
Load pickled training and validation data and use them for training many models specified by
"Training parameters" below. Successful models are saved in the "model/" directory.
-------------------------------------------------------------------------------
Info:
- Bash variable for turn of tf's warnings:
    $ export TF_CPP_MIN_LOG_LEVEL='2'
-------------------------------------------------------------------------------
ToDo:
- Let's try to use 'cosine_similarity'
"""

import tensorflow as tf
import matplotlib.pyplot as plt
import numpy as np
import pickle

from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Dropout, Activation, Flatten, InputLayer
from tensorflow.keras.layers import Conv2D, MaxPooling2D
from tensorflow.keras.callbacks import TensorBoard, Callback
from sklearn.utils import class_weight


# Training parameters:
CONV_LAYERS = [1, 2, 3]
CONV_SIZES = [32, 64, 128]

DENSE_LAYERS = [0, 1, 2]
DENSE_SIZES = [256, 512]

DROPOUT = [1]
DROPOUT_PROB = 0.35

IMG_SIZE = 80


# According to my training and evaluation the best parameters are:
#   - 1x conv 128
#   - 2x dense 512
#   - 1x dropout 0.35
#   - 10 epochs, 16 batch_size
# on augmentated data


class SaveGoodModel(Callback):
    """
    Callback for saving the "good models" during training.
    "good models" can be defined via logs (etc. val_accuracy, ...).
    """

    def __init__(self, name):
        self.name = name

    # When model is good save it!
    def on_epoch_end(self, epoch, logs=None):
        if logs['val_accuracy'] > 0.885:
            self.model.save('models/{}_epoch-{}_valacc-{:.3f}_acc-{:.3f}'.format(
                self.name, epoch + 1, logs['val_accuracy'], logs['accuracy']
            ))


def main():
    # Load the training and validation data
    with open('pickle/train_imgs.pickle', 'rb') as in_file:
        train_imgs = pickle.load(in_file)

    with open('pickle/train_lbls.pickle', 'rb') as in_file:
        train_lbls = pickle.load(in_file)

    with open('pickle/val_imgs.pickle', 'rb') as in_file:
        val_imgs = pickle.load(in_file)

    with open('pickle/val_lbls.pickle', 'rb') as in_file:
        val_lbls = pickle.load(in_file)

    # Normalize the features (X_*) from (0,255) to (0,1) scale
    train_imgs = train_imgs / 255.0
    val_imgs = val_imgs / 255.0

    # Compute the class weights because of unbalanced dataset
    class_weights = class_weight.compute_class_weight('balanced', np.unique(train_lbls), train_lbls)

    cnt = 1
    cnt_max = len(CONV_LAYERS) * len(CONV_SIZES) * \
        len(DENSE_LAYERS) * len(DENSE_SIZES) * len(DROPOUT)

    for conv_layer in CONV_LAYERS:
        for conv_size in CONV_SIZES:
            for dense_layer in DENSE_LAYERS:
                for dense_size in DENSE_SIZES:
                    for dropout in DROPOUT:
                        # Name of the model
                        NAME = "{}-conv{}_{}-dense{}_{}-dropout{}".format(
                            conv_layer, conv_size, dense_layer, dense_size, dropout, DROPOUT_PROB
                        )

                        print('*****************************************************************\n'
                              'Model: {}/{}\n{}\n'.format(cnt, cnt_max, NAME))

                        # Create a sequential model
                        model = Sequential()

                        # Add first layer -> conv layer with input shape
                        model.add(Conv2D(conv_size, (3, 3), activation='relu',
                                         input_shape=train_imgs.shape[1:]))
                        model.add(MaxPooling2D(pool_size=(2, 2)))

                        for _ in range(dropout):
                            model.add(Dropout(DROPOUT_PROB))

                        # Add another conv layers
                        for _ in range(conv_layer - 1):
                            model.add(Conv2D(conv_size, (3, 3), activation='relu'))
                            model.add(MaxPooling2D(pool_size=(2, 2)))
                            for _ in range(dropout):
                                model.add(Dropout(DROPOUT_PROB))

                        # Add first conv layer with shaping the input
                        model.add(Flatten())

                        # Add dense layers
                        for _ in range(dense_layer):
                            model.add(Dense(dense_size, activation='relu'))
                            for _ in range(dropout):
                                model.add(Dropout(DROPOUT_PROB))

                        # Final dense layer with 1 neuron and sigmoid activation function
                        # for binary classification
                        model.add(Dense(1, activation='sigmoid'))

                        # Store Tensorboard logs
                        tensorboard = TensorBoard(log_dir="logs/{}".format(NAME))

                        # Configures the model for training
                        model.compile(optimizer='adam', loss='binary_crossentropy',
                                      metrics=['accuracy'])

                        # Shows the model
                        model.summary()

                        save_good_model = SaveGoodModel(NAME)

                        # Trains the model for a fixed number of epochs
                        model.fit(x=train_imgs, y=train_lbls, validation_data=(val_imgs, val_lbls),
                                  batch_size=16, epochs=13, class_weight=class_weights,
                                  callbacks=[tensorboard, save_good_model])

                        # Saves the model to Tensorflow SavedModel after final epoch
                        # model.save('models/{}'.format(NAME))

                        cnt += 1


if __name__ == '__main__':
    main()
