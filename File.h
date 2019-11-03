#pragma once

#include <iostream>
#include <string>

class File {
private:
    std::string filename;
    long placeInFile;
public:
    File(std::string name, int place);

    ~File() = default;

    std::string getFilename() { return filename; }

    long getPlaceInFile() { return placeInFile; }

    void setPlaceInFile(std::fpos<mbstate_t> place) { placeInFile = place; }

    void removeFile();
};

