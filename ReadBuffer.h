#pragma once

#include "Buffer.h"

class ReadBuffer : public Buffer {
private:
    int counter;
    bool EndOfFile;
public:
    ReadBuffer();

    explicit ReadBuffer(File *file);

    ~ReadBuffer() = default;

    Probability *getRecord();

    Probability *nextRecord();
};

