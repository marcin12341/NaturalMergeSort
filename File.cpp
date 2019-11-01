#include "File.h"
File::File(std::string name, int place) {
	filename = name;
	placeInFile = place;
}

File::~File() {

}

void File::removeFile() {
	remove(getFilename().c_str());
}

