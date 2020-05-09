#!/usr/bin/env python3

"""
Compiles training (DATA_TRAIN_DIR) and validation (DATA_VAL_DIR) data to the numpy arrays and
store them as a binary pickle data into the "pickle/" directory.
-------------------------------------------------------------------------------
Info:
- Bash variable for turn of tf's warnings:
    $ export TF_CPP_MIN_LOG_LEVEL='2'
-------------------------------------------------------------------------------
"""

import numpy as np
import cv2
import random
import os
import pickle


# Data parameters:
DATA_TRAIN_DIR = 'data_train_augmentated/'
DATA_VAL_DIR = 'data_val_augmentated/'
IMG_SIZE = 80
CATEGORIES = ['target', 'nontarget']


def main():

    data_train = []
    data_val = []

    # Get all the training and validation data as numpy arrays
    for category in CATEGORIES:
        class_idx = CATEGORIES.index(category)

        data_train_path = os.path.join(DATA_TRAIN_DIR, category + '/')
        print(data_train_path)  # debug

        # Data for training
        for img in os.listdir(data_train_path):
            img_path = os.path.join(data_train_path, img)
            print(img_path)  # debug
            img_array = cv2.imread(img_path)
            data_train.append([img_array, class_idx])

        data_val_path = os.path.join(DATA_VAL_DIR, category + '/')
        print(data_val_path)  # debug

        # Data for validation
        for img in os.listdir(data_val_path):
            img_path = os.path.join(data_val_path, img)
            print(img_path)  # debug
            img_array = cv2.imread(img_path)
            data_val.append([img_array, class_idx])

    # For the purpose of training, data should be shuffled
    random.shuffle(data_train)
    random.shuffle(data_val)

    # X_* - list of the features
    # y_* - list of the labels
    train_imgs = []
    train_lbls = []

    val_imgs = []
    val_lbls = []

    # Fill X_* and y_* lists
    for features, label in data_train:
        train_imgs.append(features)
        train_lbls.append(label)

    for features, label in data_val:
        val_imgs.append(features)
        val_lbls.append(label)

    # Data needs to be reshaped this way, to one huge numpy array
    train_imgs = np.array(train_imgs).reshape(-1, IMG_SIZE, IMG_SIZE, 3)
    train_lbls = np.array(train_lbls)

    val_imgs = np.array(val_imgs).reshape(-1, IMG_SIZE, IMG_SIZE, 3)
    val_lbls = np.array(val_lbls)

    # Store the training and validation data in the pickle format
    try:
        os.mkdir('pickle/')
    except FileExistsError:
        pass

    with open('pickle/train_imgs.pickle', 'wb') as out_file:
        pickle.dump(train_imgs, out_file)

    with open('pickle/train_lbls.pickle', 'wb') as out_file:
        pickle.dump(train_lbls, out_file)

    with open('pickle/val_imgs.pickle', 'wb') as out_file:
        pickle.dump(val_imgs, out_file)

    with open('pickle/val_lbls.pickle', 'wb') as out_file:
        pickle.dump(val_lbls, out_file)


if __name__ == '__main__':
    main()
