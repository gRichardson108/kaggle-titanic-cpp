//
// Created by Garret on 11/29/2024.
//

#ifndef CPP_PREDICTOR_PERCEPTRON_H
#define CPP_PREDICTOR_PERCEPTRON_H

#include <vector>


/// A perceptron that performs binary classification.
class Perceptron {
public:
    Perceptron(float learningRate, int epochs);
    float netInput(std::vector<float> input);
    int predict(std::vector<float> input);
    void fit(std::vector<std::vector<float>> dataset, std::vector<float> bias);
    void fit_start(std::vector<std::vector<float>> dataset, std::vector<float> bias);
    void fit_step(int numSteps);
    
    float learningRate;
    int epochs;
    unsigned int currentStep;
    unsigned int totalSteps;
    std::vector<float> weights;
    std::vector<std::vector<float>> currentDataset;
    std::vector<float> currentBias;
    float currentMaxWeight;
    float currentMinWeight;
};


#endif //CPP_PREDICTOR_PERCEPTRON_H
