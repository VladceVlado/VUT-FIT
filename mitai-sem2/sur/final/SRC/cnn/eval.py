#!/usr/bin/env python3

"""
Run classification on unknown data in DATA_DIR for all the models in MODELS_DIR and
for every model generate required output in the "out/" directory.
-------------------------------------------------------------------------------
Info:
- Bash variable for turn of tf's warnings:
    $ export TF_CPP_MIN_LOG_LEVEL='2'
-------------------------------------------------------------------------------
"""

import tensorflow as tf
import cv2
import os


# Data parameters:
MODELS_DIR = 'models/'
DATA_DIR = 'eval_img/'
IMG_SIZE = 80
CATEGORIES = ['target', 'nontarget']


def main():

    try:
        os.mkdir('out/')
    except FileExistsError:
        pass

    # For every trained model
    for model_dir in os.listdir(MODELS_DIR):
        print(model_dir)  # debug
        model_path = os.path.join(MODELS_DIR, model_dir + '/')

        # Loads a model saved via "save_model"
        model = tf.keras.models.load_model(model_path)

        with open('out/{}.txt'.format(model_dir), 'w') as output:

            # For every image
            for img in os.listdir(DATA_DIR):
                print(img)  # debug
                img_path = os.path.join(DATA_DIR, img)
                img_array = cv2.imread(img_path)
                img_array = img_array.reshape(-1, IMG_SIZE, IMG_SIZE, 3)

                # Do the prediction
                output.write('{} '.format(os.path.splitext(img)[0]))
                prediction = model.predict(img_array)
                output.write('{:.3f} '.format(1.0 - prediction[0][0]))
                prediction = CATEGORIES[int(prediction[0][0])]

                if prediction == 'target':
                    output.write('1\n')
                elif prediction == 'nontarget':
                    output.write('0\n')
                else:
                    raise Exception


if __name__ == '__main__':
    main()
