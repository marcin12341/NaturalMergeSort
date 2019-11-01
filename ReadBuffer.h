#pragma once
#include "Buffer.h"
class ReadBuffer : public Buffer
{
public:
	int counter;
	bool EndOfFile;

	ReadBuffer();
	explicit ReadBuffer(File* file);
	~ReadBuffer()=default;

	Probability* getRecord();
	Probability* nextRecord();
};

