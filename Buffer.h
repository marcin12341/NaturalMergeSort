#pragma once
#define SIZE 4096
#include "Probability.h"
#include "File.h"
#include "globals.h"
#include <fstream>
class Buffer
{
private:
protected:
    Probability* buffer;
    int current;
    int bufferSize;
    File* file{};
public:
	Buffer();
	~Buffer();
};

