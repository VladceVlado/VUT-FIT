#!/usr/bin/env python3

"""
Run classification on data in DATA_DIR for all the models in MODELS_DIR and for every model
generate required output in the "out/" directory.
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
MODELS_DIR = 'good_models_1/models_p3/'
DATA_DIR = '../data_img_eval/'
CATEGORIES = ['target', 'nontarget']
IMG_SIZE = 80


def main():

    try:
        os.mkdir('out/')
    except FileExistsError:
        pass

    # For every trained model
    for model_dir in os.listdir(MODELS_DIR):
        print(model_dir)
        model_path = os.path.join(MODELS_DIR, model_dir + '/')

        # Loads a model saved via "save_model"
        model = tf.keras.models.load_model(model_path)

        # Compute the statistics, how model is succesful
        target_ok = 0
        target_wrong = 0
        nontarget_ok = 0
        nontarget_wrong = 0

        with open('out/{}.txt'.format(model_dir), 'w') as output:

            # For every category
            for category in CATEGORIES:
                category_path = os.path.join(DATA_DIR, category + '/')

                # For every image
                for img in os.listdir(category_path):
                    img_path = os.path.join(category_path, img)
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

                    if category == 'target':
                        if prediction == 'target':
                            target_ok += 1
                        elif prediction == 'nontarget':
                            target_wrong += 1
                        else:
                            raise Exception
                    elif category == 'nontarget':
                        if prediction == 'nontarget':
                            nontarget_ok += 1
                        elif prediction == 'target':
                            nontarget_wrong += 1
                        else:
                            raise Exception
                    else:
                        raise Exception

            print('    target recognized: {}/{}\n    nontarget recognized: {}/{}\n    success rate: {:.3f}, {:.3f}\n'
                  .format(target_ok, target_ok + target_wrong, nontarget_ok, nontarget_ok +
                          nontarget_wrong, target_ok / (target_ok + target_wrong),
                          nontarget_ok / (nontarget_ok + nontarget_wrong))
                  )


if __name__ == '__main__':
    main()
