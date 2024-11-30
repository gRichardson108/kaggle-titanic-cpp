//
// Created by Garret on 11/29/2024.
//

#include <cmath>
#include <iostream>
#include "Perceptron.h"

Perceptron::Perceptron(float learningRate, int epochs) {
    this->learningRate = learningRate;
    this->epochs = epochs;
}

int Perceptron::predict(std::vector<float> input) {
    return netInput(input) > 0 ? 1 : -1;
}

float Perceptron::netInput(std::vector<float> input) {
    // multiplies the input vector by the transpose of the weights vector
    // ie, each element of the input dataset is multiplied by the corresponding weight, summed, and bias is added

    // bias first
    float probabilities = weights[0];
    for (int i = 0; i < input.size(); i++) {
        probabilities += input[i] * weights[i + 1];
    }
    return probabilities;
}

void Perceptron::fit(std::vector<std::vector<float>> dataset, std::vector<float> bias) {
    // the 0th weight is used to store the bias term, so we have one extre weight in the array
    for (int i = 0; i < dataset[0].size() + 1; i++) {
        this->weights.push_back(0.0f);
    }

    for (int i = 0; i < this->epochs; i++) {
        for (int j = 0; j < dataset.size(); j++) {
            float deltaWeight = learningRate * (bias[j] - predict(dataset[j]));
            for (int w = 1; w < weights.size(); w++) {
                weights[w] += deltaWeight * dataset[j][w - 1];
                weights[0] = deltaWeight;
                if (std::isnan(weights[w]) || std::isinf(weights[w])) {
                    std::cerr << "Bad weight index: " << w << std::endl;
                }
            }
        }
    }
}

void Perceptron::fit_start(std::vector<std::vector<float>> dataset, std::vector<float> bias) {
    this->weights.clear();
    // the 0th weight is used to store the bias term, so we have one extra weight in the array
    for (int i = 0; i < dataset[0].size() + 1; i++) {
        this->weights.push_back(0.0f);
    }
    currentDataset = dataset;
    currentBias = bias;
    currentStep = 0;
    currentMaxWeight = 0.0f;
    currentMinWeight = 0.0f;
    totalSteps = this->epochs * currentDataset.size();
}

void Perceptron::fit_step(int numSteps) {
    for (int step = 0; step < numSteps; step++) {
        if (currentStep >= totalSteps) {
            break;
        }
        int i = currentStep / this->epochs;
        int j = currentStep % currentDataset.size();
        float deltaWeight = learningRate * (currentBias[j] - predict(currentDataset[j]));
        for (int w = 1; w < weights.size(); w++) {
            weights[w] += deltaWeight * currentDataset[j][w - 1];
            if (weights[w] < currentMinWeight) {
                currentMinWeight = weights[w];
            }
            if (weights[w] > currentMaxWeight) {
                currentMaxWeight = weights[w];
            }
            weights[0] = deltaWeight;
            if (std::isnan(weights[w]) || std::isinf(weights[w])) {
                std::cerr << "Bad weight index: " << w << std::endl;
            }
        }
        currentStep++;
    }
}
