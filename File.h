#pragma once
#include <iostream>
#include <string>
class File
{
private:
	std::string filename;
	int placeInFile;
public:
	File(std::string name, int place);
	~File();

	std::string getFilename() { return filename; }
	int getPlaceInFile() { return placeInFile; }
	void setPlaceInFile(int place) { placeInFile = place; }
	void removeFile();
};

