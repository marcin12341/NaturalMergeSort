#include "File.h"

#include <utility>
File::File(std::string name, int place) {
	filename = std::move(name);
	placeInFile = place;
}


void File::removeFile() {
	remove(getFilename().c_str());
}

