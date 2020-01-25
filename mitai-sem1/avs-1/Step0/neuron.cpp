/*
 * Architektury výpočetních systémů (AVS 2019)
 * Projekt c. 1 (ANN)
 * Login: xdusek27
 */

#include <cstdlib>
#include "neuron.h"


float evalNeuron(size_t inputSize, size_t neuronCount, const float *input, const float *weight, float bias, size_t neuronId)
{
    float y, x = 0;

    for (size_t i = 0; i < inputSize; i++) {
        x += input[i] * weight[i * neuronCount + neuronId];
    }

    y = x < 0 ? 0 : x;

    return y;
}
