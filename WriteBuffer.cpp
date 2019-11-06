#include "WriteBuffer.h"

WriteBuffer::WriteBuffer(File *file) {
    print = 'n';
    this->file = file;
    this->file->removeFile();
    this->current = 0;
}

WriteBuffer::WriteBuffer(File *file, char print) {
    this->print = print;
    this->file = file;
    this->file->removeFile();
    this->current = 0;
}

WriteBuffer::~WriteBuffer() {
    if (count)
        writeCount++;
    if (print == 'y') {
        std::cout << "File after next run: \n";
    }
    std::ofstream output(file->getFilename(), std::ios::out | std::ios::app);
    for (int i = 0; i < current; i++) {
        output << buffer[i].eventA << ' ' << buffer[i].eventB << ' ' << buffer[i].intersectionAB << std::endl;
        if (print == 'y') {
            std::cout << "P(A)=" << buffer[i].eventA << "\tP(B)=" << buffer[i].eventB << "\tP(AB)="
                      << buffer[i].intersectionAB << "\tP(A|B)=" << buffer[i].conditionalProbability << std::endl;
        }
    }
    output.close();
}

bool WriteBuffer::writeRecord(Probability *record) {
    if (current == bufferSize) {
        if (count)
            writeCount++;
        std::ofstream output(file->getFilename(), std::ios::out | std::ios::app);
        for (current = 0; current < bufferSize; current++) {
            output << buffer[current].eventA << ' ' << buffer[current].eventB << ' ' << buffer[current].intersectionAB
                   << std::endl;
            if (print == 'y') {
                std::cout << "P(A)=" << buffer[current].eventA << "\tP(B)=" << buffer[current].eventB << "\tP(AB)="
                          << buffer[current].intersectionAB << "\tP(A|B)=" << buffer[current].conditionalProbability
                          << std::endl;
            }
        }
        output.close();
        current = 0;
    }
    if (record == nullptr) return false;
    buffer[current] = *record;
    current++;

    return true;
}

bool WriteBuffer::writeValues(Probability *record) {
    if (current == bufferSize) {
        if (count)
            writeCount++;
        std::ofstream output(file->getFilename(), std::ios::out | std::ios::app);
        for (current = 0; current < bufferSize; current++) {
            output << buffer[current].eventA << ' ' << buffer[current].eventB << ' ' << buffer[current].intersectionAB
                   << std::endl;
            std::cout << "P(A)=" << buffer[current].eventA << "\tP(B)=" << buffer[current].eventB << "\tP(AB)="
                      << buffer[current].intersectionAB << std::endl;
        }
        output.close();
        current = 0;
    }
    if (record == nullptr) return false;
    buffer[current] = *record;
    current++;

    return true;
}

void WriteBuffer::writeRemainingValues() {
    if (count)
        writeCount++;
    std::ofstream output(file->getFilename(), std::ios::out | std::ios::app);
    for (int i = 0; i < current; i++) {
        output << buffer[i].eventA << ' ' << buffer[i].eventB << ' ' << buffer[i].intersectionAB << std::endl;
        std::cout << "P(A)=" << buffer[i].eventA << "\tP(B)=" << buffer[i].eventB << "\tP(AB)="
                  << buffer[i].intersectionAB << std::endl;
    }
    output.close();
    current = 0;
}

