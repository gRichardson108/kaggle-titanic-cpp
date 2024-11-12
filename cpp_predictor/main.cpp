#include <iostream>
#include <vector>
#include <filesystem>

#include "rapidcsv.h"

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

void ConvSex(const std::string& pStr, bool& pVal)
{
    pVal = pStr == "male";
}

void ConvIntNegativeIfMissing(const std::string& pStr, int& pVal)
{
    if (pStr.empty())
    {
        pVal = -1;
    } else {
        pVal = std::stoi(pStr);
    }
}

std::vector<training_data> readTrainingDataset()
{
    rapidcsv::Document doc("E:\\Projects\\Kaggle\\Titanic\\data\\train.csv");

    // read the entire file into structured memory
    std::vector<training_data> dataset = {};
    dataset.reserve(doc.GetRowCount());
    for (int row = 0; row < doc.GetRowCount(); row++) {
        int col = 0;
        training_data d = {};
        try {
            d.passengerId = doc.GetCell<int>(col++, row);
            d.survived = doc.GetCell<int>(col++, row);
            d.passengerClass = doc.GetCell<int>(col++, row);
            d.name = doc.GetCell<std::string>(col++, row);
            d.isMale = doc.GetCell<bool>(col++, row, ConvSex);
            d.age = doc.GetCell<int>(col++, row, ConvIntNegativeIfMissing);
            d.siblingsSpousesAboard = doc.GetCell<int>(col++, row);
            d.parentsChildrenAboard = doc.GetCell<int>(col++, row);
            d.ticketNumber = doc.GetCell<std::string>(col++, row);
            d.fareCost = doc.GetCell<float>(col++, row);
            d.cabinNumber = doc.GetCell<std::string>(col++, row);
            d.embarkedLocation = doc.GetCell<std::string>(col++, row);
        } catch (...)
        {
            std::cerr << "Error on line " << row << " col " << col << std::endl;
        }

        dataset.push_back(d);
    }
    
    return dataset;
}


int main() {
    std::filesystem::path cwd = std::filesystem::current_path();
    std::cout << "Working directory: " << cwd.string() << std::endl;
    
    std::vector<training_data> dataset = readTrainingDataset();
    
    // print out the survivors
    std::cout << "Names of survivors: \n";
    for (const training_data& d : dataset)
    {
        if (d.survived)
        {
            std::cout << d.name << "\n";
        }
    }
    std::cout << std::endl; 

    return 0;
}
