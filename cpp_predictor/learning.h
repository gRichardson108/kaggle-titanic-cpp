//
// Created by Garret on 11/29/2024.
//
#ifndef CPP_PREDICTOR_LEARNING_H
#define CPP_PREDICTOR_LEARNING_H
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>

extern const char* INPUT_PATH;
extern const char* OUTPUT_PATH;

struct training_data {
    int passengerId;
    bool survived;
    int passengerClass;
    std::string name;
    bool isMale;
    int age;
    int siblingsSpousesAboard;
    int parentsChildrenAboard;
    std::string ticketNumber;
    float fareCost;
    std::string cabinNumber;
    std::string embarkedLocation;
};

struct output {
    training_data input;
    bool predictionSurvived;
};

void ConvSex(const std::string& pStr, bool& pVal);
void ConvIntNegativeIfMissing(const std::string& pStr, int& pVal);
void writeOutputData(const std::vector<output>& outputData, std::string filepath);
std::vector<training_data> readTrainingDataset();
std::vector<float> createEmbedding(const training_data& input);
std::vector<output> createTrainingOutput(const std::vector<training_data>& input_train_set);
void writeOutputData(const std::vector<output> &outputData, std::string filepath);

#endif //CPP_PREDICTOR_LEARNING_H
