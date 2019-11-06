#pragma once

#include "Buffer.h"

class WriteBuffer : public Buffer {
private:
    char print;
public:
    explicit WriteBuffer(File *file);

    WriteBuffer(File *file, char print);

    ~WriteBuffer();

    bool writeRecord(Probability *record);

    void writeRemainingValues();
};

