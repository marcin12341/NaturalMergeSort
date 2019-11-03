#include "ReadBuffer.h"

ReadBuffer::ReadBuffer() {
    counter = 0;
    EndOfFile = false;
    current = bufferSize;
}

ReadBuffer::ReadBuffer(File *file) {
    counter = 0;
    EndOfFile = false;
    current = bufferSize;
    this->file = file;
}


Probability *ReadBuffer::getRecord() {
    if (current == bufferSize) {
        readCount++;
        double value1;
        double value2;
        double value3;
        Probability *newProbability;
        std::ifstream input(file->getFilename());
        if (!input.good()) {
            std::cout << "Couldn't read file\n";
            return nullptr;
        }
        input.seekg(file->getPlaceInFile(), std::ios::beg);
        counter = 0;

        while ((counter < bufferSize) && (!EndOfFile)) {
            if (input >> value1 >> value2 >> value3) {
                newProbability = new Probability(value1, value2, value3);
                buffer[counter] = *newProbability;
                std::cout << "P(A)=" << value1 << "\tP(B)=" << value2 << "\tP(AB)=" << value3 << '\n';
                counter++;
                delete newProbability;
            } else
                EndOfFile = true;
        }

        file->setPlaceInFile(input.tellg());
        input.close();
        current = 0;

    }
    if (current >= counter && EndOfFile) return nullptr;

    current++;
    return &(buffer[current - 1]);
}

Probability *ReadBuffer::nextRecord() {
    if (current == bufferSize) {
        readCount++;
        double value1;
        double value2;
        double value3;
        double value4;
        Probability *newProbability;
        std::ifstream input(file->getFilename());
        if (!input.good()) {
            std::cout << "Couldn't read file\n";
            return nullptr;
        }
        input.seekg(file->getPlaceInFile(), std::ios::beg);
        counter = 0;

        while ((counter < bufferSize) && (!EndOfFile)) {
            if (input >> value1 >> value2 >> value3 >> value4) {
                newProbability = new Probability(value1, value2, value3, value4);
                buffer[counter] = *newProbability;
                counter++;
                delete newProbability;
            } else
                EndOfFile = true;
        }

        file->setPlaceInFile(input.tellg());
        input.close();
        current = 0;

    }
    if (current >= counter && EndOfFile) return nullptr;

    current++;
    return &(buffer[current - 1]);

}
