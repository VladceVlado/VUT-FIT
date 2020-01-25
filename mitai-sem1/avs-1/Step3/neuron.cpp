/*
 * Architektury výpočetních systémů (AVS 2019)
 * Projekt c. 1 (ANN)
 * Login: xdusek27
 */

#include <cstdlib>
#include "neuron.h"


float evalNeuron(size_t inputSize, const float *input, const float *weight, float bias)
{
    float y, x = 0;

    // Dalsi optimalizace mozna s uzitim dovetku aligned(input, weight), nicmene zarovnani se resi az v kroku 4
    #pragma omp simd reduction(+:x)
    for (size_t i = 0; i < inputSize; i++) {
        x += input[i] * weight[i];
    }

    x += bias;

    y = x < 0 ? 0 : x;

    return y;
}
