import numpy as np
from ikrlib import wav16khz2mfcc, logistic_sigmoid, train_gauss, train_gmm, logpdf_gmm, gellipse
import matplotlib.pyplot as plt
import scipy.linalg
from numpy.random import randint

class_index = 0
train_sample = [None] * 2
test_sample = [None] * 2
real_test_sample = [None] * 2

for speech_sample in train_sample:
    train_sample[class_index] = wav16khz2mfcc('data/train/' + str(class_index + 1)).values()
    for rec in train_sample[class_index]:
        rec = rec[100:len(rec) - 300, :]
    class_index += 1

P_c1 = len(train_sample[0]) * 1.0 / len(train_sample[1])
P_c = [P_c1, 1 - P_c1]

real_test_sample_dict = wav16khz2mfcc('data/eval/')

real_test_sample = real_test_sample_dict.values()
real_test_name = real_test_sample_dict.keys()


# Cutting silence of a record - to get better results
for rec in real_test_sample:
    rec = rec[100:len(rec) - 300, :]

for class_index in xrange(len(train_sample)):
    train_sample[class_index] = np.vstack(train_sample[class_index])
    train_sample[class_index] = np.vstack(train_sample[class_index])

# GMM
mu = [None] * len(train_sample)
cov = [None] * len(train_sample)
for index in xrange(len(train_sample)):
    mu[index], cov[index] = train_gauss(train_sample[index])

# Train and test with GMM models with full covariance matrices
# Decide for number of gaussian mixture components used for the model
m = 8

mus = [[None] * m] * len(train_sample)
covs = [None] * len(train_sample)
ws = [None] * len(train_sample)

for index in xrange(len(train_sample)):
    mus[index] = train_sample[index][randint(1, len(train_sample[index]), m)]
    # Initialize all covariance matrices to the same covariance matrices computed using
    # all the data from the given class
    covs[index] = [cov[index]] * m
    ws[index] = np.ones(m) / m

ttl = [None] * len(train_sample)
# Run 110 iterations of EM algorithm to train GMM models
for i in range(110):
    for index in xrange(len(train_sample)):
        ws[index], mus[index], covs[index], ttl[index] = train_gmm(train_sample[index], ws[index], mus[index], covs[index])

ll_c = [None] * 2
ll_c_new = [None] * 2

f = open("gmm_speech.txt", "w")

for test in xrange(len(real_test_sample)):

    for index in xrange(len(train_sample)):
        ll_c[index] = sum(logpdf_gmm(real_test_sample[test], ws[index], mus[index], covs[index])) + np.log(P_c[index])

    ll_index_max = np.argmax(ll_c)

    prob_1 = 1 - (ll_c[0] / (ll_c[0] + ll_c[1]))

    real_test_name[test] = real_test_name[test].replace(".wav", "")
    f.write(real_test_name[test].replace("data/eval/", "") + ' ' + str(prob_1) + ' ' + ("1" if ll_index_max == 0 else "0") + "\n")

f.close()