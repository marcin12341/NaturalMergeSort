#include "Buffer.h"

Buffer::Buffer() {
    bufferSize = SIZE;
    buffer = new Probability[bufferSize];
    current = 0;
}


Buffer::~Buffer() {
    delete[]buffer;
}