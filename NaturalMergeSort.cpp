#include <utility>

#include "WriteBuffer.h"
#include "ReadBuffer.h"
#include <algorithm>
#include <iomanip>
#include <random>


void generateProbabilities(std::string filename) {
    count = false;
    int howMany = 0;
    File *output = new File(std::move(filename), 0);
    auto *tape = new WriteBuffer(output);
    Probability *record = nullptr;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    std::cout << "Insert number of records to generate: ";
    std::cin >> howMany;
    while (howMany--) {
        double A = 0, B = 0, AB = 0;
        while ((A == 0 || A == 1) && (B == 0 || B == 1)) {
            A = dist(mt);
            B = dist(mt);
        }
        double max = std::min(A, B);
        double min = A + B - 1.0;
        while (AB <= 0) {
            AB = (max - min) * dist(mt) + min;
        }
        record = new Probability(A, B, AB);
        tape->writeValues(record);
        delete record;
    }
    tape->writeRemainingValues();

    delete tape;
    delete output;
}

void keyboardInput(std::string filename) {
    count = false;
    double intersectionAB = 0;
    double eventB = 0;
    double eventA = 0;
    int howMany;
    File *output = new File(std::move(filename), 0);
    auto *tape = new WriteBuffer(output);
    Probability *record = nullptr;

    std::cout << "How many records to insert: ";
    std::cin >> howMany;
    while (howMany--) {
        std::cout << "P(A)=";
        std::cin >> eventA;
        std::cout << "\nP(B)=";
        std::cin >> eventB;
        std::cout << "\nP(A|B)=";
        std::cin >> intersectionAB;

        record = new Probability(eventA, eventB, intersectionAB);
        tape->writeValues(record);
        delete record;
    }
    tape->writeRemainingValues();
    delete tape;
    delete output;
}

void loadFile(std::string filename) {
    std::string str;
    std::cout << "File path: ";
    std::getline(std::cin, str);
    std::getline(std::cin, str);
    for (size_t i = 0; i < str.length(); i++)
        filename[i] = str[i];
}

bool menu(const std::string &filename) {
    char choice = 'q';
    std::cout << "Choose a method of creating record file:\n";
    std::cout << "1. Random generation\n";
    std::cout << "2. Keyboard input\n";
    std::cout << "3. Load file\n";
    std::cout << "Q. Quit\n";

    std::cin >> choice;

    switch (choice) {
        case '1':
            generateProbabilities(filename);
            break;
        case '2':
            keyboardInput(filename);
            break;
        case '3':
            loadFile(filename);
            break;
        case 'Q':
            return false;
        case 'q':
            return false;
        default:
            std::cout << "Unknown command, try again\n";
            menu(filename);
            break;
    }
    return true;
}

void divide() {
    count = true;
    dividePhases++;

    File *input = new File("copy.csv", 0);
    File *output1 = new File("1.csv", 0);
    File *output2 = new File("2.csv", 0);

    auto *inputTape = new ReadBuffer(input);
    auto *outputTape1 = new WriteBuffer(output1);
    auto *outputTape2 = new WriteBuffer(output2);

    Probability *record;
    double previousValue = 0;
    WriteBuffer *tape = outputTape1;

    while (true) {
        record = inputTape->nextRecord();
        if (!record) {
            break;
        }
        if (previousValue > record->conditionalProbability) {
            if (tape == outputTape1) {
                tape = outputTape2;
            } else {
                tape = outputTape1;
            }
        }
        tape->writeRecord(record);
        previousValue = record->conditionalProbability;
    }

    delete outputTape2;
    delete outputTape1;
    delete inputTape;
    delete output2;
    delete output1;
    delete input;
}

bool merge(char print) {
    count = true;
    mergePhases++;

    File *input1 = new File("1.csv", 0);
    File *input2 = new File("2.csv", 0);
    File *output = new File("copy.csv", 0);

    auto *inputTape1 = new ReadBuffer(input1);
    auto *inputTape2 = new ReadBuffer(input2);
    auto *outputTape = new WriteBuffer(output, print);

    double previous1 = 0, previous2 = 0;
    Probability *record1 = inputTape1->nextRecord();
    Probability *record2 = inputTape2->nextRecord();
    if (!record2) {
        delete outputTape;
        delete inputTape2;
        delete inputTape1;
        delete output;
        delete input2;
        delete input1;
        return false;
    }
    while (true) {
        if (record1 && record2) {
            if (record1->conditionalProbability < previous1) {
                while (record2 && record2->conditionalProbability > previous2) {
                    outputTape->writeRecord(record2);
                    previous2 = record2->conditionalProbability;
                    record2 = inputTape2->nextRecord();
                }
                previous1 = 0;
                previous2 = 0;
            } else if (record2->conditionalProbability < previous2) {
                while (record1 && record1->conditionalProbability > previous1) {
                    outputTape->writeRecord(record1);
                    previous1 = record1->conditionalProbability;
                    record1 = inputTape1->nextRecord();
                }
                previous1 = 0;
                previous2 = 0;
            } else {
                if (record1->conditionalProbability < record2->conditionalProbability) {
                    outputTape->writeRecord(record1);
                    previous1 = record1->conditionalProbability;
                    record1 = inputTape1->nextRecord();
                } else {
                    outputTape->writeRecord(record2);
                    previous2 = record2->conditionalProbability;
                    record2 = inputTape2->nextRecord();
                }
            }
        } else if (!record1) {
            while (record2) {
                outputTape->writeRecord(record2);
                record2 = inputTape2->nextRecord();
            }
            break;
        } else {
            while (record1) {
                outputTape->writeRecord(record1);
                record1 = inputTape1->nextRecord();
            }
            break;
        }
    }

    delete outputTape;
    delete inputTape2;
    delete inputTape1;
    delete output;
    delete input2;
    delete input1;
    return true;
}

void rewrite(std::string org, std::string cp) {
    count = false;

    File *input = new File(std::move(org), 0);
    File *output = new File(std::move(cp), 0);

    auto *original = new ReadBuffer(input);
    auto *copy = new WriteBuffer(output);
    std::cout << "File before sorting: \n\n";
    while (copy->writeRecord(original->getRecord()));
    std::cout << std::endl;
    delete copy;
    delete original;
    delete output;
    delete input;
}

void rewriteSorted() {
    count = false;
    File *input = new File("1.csv", 0);
    File *output = new File("sorted.csv", 0);

    auto *original = new ReadBuffer(input);
    auto *copy = new WriteBuffer(output);

    std::cout << "\nFile after sorting: \n\n";

    while (copy->writeValues(original->nextRecord()));
    copy->writeRemainingValues();

    remove("1.csv");
    remove("2.csv");
    delete copy;
    delete original;
    delete output;
    delete input;
}

int main() {

    std::string filename = "dataset.csv";
    std::string resultsFilename = "results.csv";
    auto results = new File(resultsFilename, 0);
    results->removeFile();
    bool sorted = false;
    char print = 'n';
    std::cout << std::fixed;
    std::cout << std::setprecision(6);

    while (menu(filename)) {
        unsigned long long int finalWrite = 0;
        unsigned long long int finalRead = 0;
        unsigned long long int finalDivide = 0;
        unsigned long long int finalMerge = 0;

        std::cout << "Do you want to print file after every sorting phase? y/n" << std::endl;
        std::cin >> print;

        for (int i = 0; i < 1; i++) {
            rewrite(filename, "copy.csv");

            writeCount = 0;
            readCount = 0;
            dividePhases = 0;
            mergePhases = 0;

            while (!sorted) {
                divide();
                sorted = !merge(print);
            }
            sorted = false;

            rewriteSorted();

            std::cout << std::endl;
            std::cout << "Finished sorting!" << std::endl;
            std::cout << "*****************************************" << std::endl;
            std::cout << "Disk writes:\t" << writeCount << std::endl;
            std::cout << "Disk reads: \t" << readCount << std::endl;
            std::cout << "Divide phases:\t" << dividePhases << std::endl;
            std::cout << "Merge phases:\t" << mergePhases << std::endl;
            std::cout << "*****************************************" << std::endl;

            finalWrite += writeCount;
            finalRead += readCount;
            finalDivide += dividePhases;
            finalMerge += mergePhases;
        }
        finalWrite /= 10;
        finalRead /= 10;
        finalDivide /= 10;
        finalMerge /= 10;

        std::ofstream output(resultsFilename, std::ios::out | std::ios::app);
        output << finalWrite << ',' << finalRead << ',' << finalDivide << ',' << finalMerge << std::endl;
        output.close();
    }
    return 0;
}


