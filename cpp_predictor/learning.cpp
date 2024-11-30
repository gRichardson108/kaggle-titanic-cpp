#include "learning.h"
#include "rapidcsv.h"
#include "Perceptron.h"

const char* INPUT_PATH = R"(E:\Projects\Kaggle\Titanic\data\train.csv)";
const char* OUTPUT_PATH = R"(E:\Projects\Kaggle\Titanic\data\output.csv)";

void ConvSex(const std::string &pStr, bool &pVal) {
    pVal = pStr == "male";
}

void ConvIntNegativeIfMissing(const std::string &pStr, int &pVal) {
    if (pStr.empty())
    {
        pVal = -1;
    } else {
        pVal = std::stoi(pStr);
    }
}

std::vector<training_data> readTrainingDataset()
{
    rapidcsv::Document doc(INPUT_PATH);

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

std::vector<float> createEmbedding(const training_data& input)
{
    std::vector<float> convertedSet = {};
    // for each training data element, we need to convert to a float for the input.
    // The approach for each element is going to change the result. Ie, how do you convert
    // name into a float? We could create some kind of embedding, but this will use a naive approach
    convertedSet.push_back(input.passengerClass);//int passengerClass;
    convertedSet.push_back(input.name.length());//std::string name;
    convertedSet.push_back(input.isMale? 1.0f: 0.0f);//bool isMale;
    convertedSet.push_back(input.age);//int age;
    convertedSet.push_back(input.siblingsSpousesAboard);//int siblingsSpousesAboard;
    convertedSet.push_back(input.parentsChildrenAboard);//int parentsChildrenAboard;
    // TODO: split the ticket number up in a useful fashion
    convertedSet.push_back(input.ticketNumber.length());//std::string ticketNumber;
    convertedSet.push_back(input.fareCost);//float fareCost;
    convertedSet.push_back(input.cabinNumber.length());//std::string cabinNumber;
    convertedSet.push_back(input.embarkedLocation.length());//std::string embarkedLocation;
    return convertedSet;
}

std::vector<output> createTrainingOutput(const std::vector<training_data>& input_train_set)
{
    const float LEARNING_RATE = 0.1f;
    const int EPOCHS = 32;
    Perceptron perceptron(LEARNING_RATE, EPOCHS);
    
    // convert input data into vectors of weights
    std::vector<std::vector<float>> inputWeights;
    std::vector<float> y;
    for (const training_data& i : input_train_set)
    {
        // save expected for each datapoint
        y.push_back(i.survived? 1.0f : -1.0f);
        std::vector<float> convertedSet = createEmbedding(i);
        inputWeights.push_back(convertedSet);
    }
    
    // feed data into perceptron
    perceptron.fit(inputWeights, y);
  
    // use perceptron to generate output
    std::vector<output> output_train_set = {};
    for (const training_data& i : input_train_set)
    {
        auto embedding = createEmbedding(i);
        bool survived = (perceptron.predict(embedding) > 0);
        output o = {
                i,
                survived
        };
        output_train_set.push_back(o);
    }
    return output_train_set;
}


void writeOutputData(const std::vector<output> &outputData, std::string filepath) {
    std::ofstream file(filepath, std::ios::out);
    if (!file.is_open()) {
        std::cerr << "Couldn't open output filepath: " << filepath << std::endl;
        return;
    }

    file << "PassengerId,Survived\n";
    for (const output& o : outputData)
    {
        file << o.input.passengerId << "," << (o.predictionSurvived? "1\n" : "0\n");
    }
    file.close();
}

// old main function
int run_learning() {
    std::filesystem::path cwd = std::filesystem::current_path();
    std::cout << "Working directory: " << cwd.string() << std::endl;
    
    std::vector<training_data> dataset = readTrainingDataset();
    
    auto outdata = createTrainingOutput(dataset);
    writeOutputData(outdata, OUTPUT_PATH);

    return 0;
}
