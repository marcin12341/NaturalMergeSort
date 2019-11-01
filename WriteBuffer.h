#pragma once
#include "Buffer.h"
class WriteBuffer : public Buffer
{
public:
	char print;

	explicit WriteBuffer(File* file);
	WriteBuffer(File* file, char print);
	~WriteBuffer();

	bool writeRecord(Probability* record);
	bool writeValues(Probability* record);
	void writeRemainingValues();
};

