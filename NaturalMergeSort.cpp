#include <utility>

#include "WriteBuffer.h"
#include "ReadBuffer.h"


void generateProbabilities(std::string filename) {
	double A, B, AB;
	int howMany;
	File* output = new File(std::move(filename), 0);
	auto* tape = new WriteBuffer(output);
	Probability* record = nullptr;

	srand(time(nullptr));

	std::cout << "Insert number of records to generate: ";
	std::cin >> howMany;
	while (howMany--) {
		A = rand() / double(RAND_MAX);
		B = rand() / double(RAND_MAX);
		AB = A * B;
		record = new Probability(A, B, AB);
		tape->writeValues(record);
		delete record;
	}
	tape->writeRemainingValues();
	delete tape;
	delete output;
}
void keyboardInput(std::string filename) {
	double eventA, eventB, intersectionAB;
	int howMany;
	File* output = new File(std::move(filename), 0);
	auto* tape = new WriteBuffer(output);
	Probability* record = nullptr;

	std::cout << "How many records to insert: ";
	std::cin >> howMany;
	while (howMany--) {
		std::cout << "P(A)=";
		std::cin >> eventA;
		std::cout << "\nP(B)=";
		std::cin >> eventB;
		intersectionAB = eventA * eventB;

		record = new Probability(eventA, eventB, intersectionAB);
		tape->writeValues(record);
		delete record;
	}
	tape->writeRemainingValues();
	delete tape;
	delete output;
}
void loadFile(std::string filename)
{
	int i;
	std::string str;
	std::cout << "File path: ";
	std::getline(std::cin, str);
	std::getline(std::cin, str);
	for (i = 0; i < str.length(); i++)
		filename[i] = str[i];
}

void menu(const std::string& filename) {
	char choice;
	std::cout << "Choose a way to create record file:\n";
	std::cout << "1. Random generation\n";
	std::cout << "2. Keyboard input\n";
	std::cout << "3. Load file\n";

	std::cin >> choice;

	switch (choice) {
	case '1':
		generateProbabilities(filename);
		break;
	case '2':
		keyboardInput(filename);
		break;
	case '3':
		loadFile(filename);
		break;
	default:
		std::cout << "Unknown command, try again\n";
		menu(filename);
		break;
	}
}

void divide() {
	dividePhases++;

	File* input = new File("copy.csv", 0);
	File* output1 = new File("1.csv", 0);
	File* output2 = new File("2.csv", 0);

	auto* inputTape = new ReadBuffer(input);
	auto* outputTape1 = new WriteBuffer(output1);
	auto* outputTape2 = new WriteBuffer(output2);

	Probability* record;
	double previousValue = 0;
	WriteBuffer* tape = outputTape1;

	while (true) {
		record = inputTape->nextRecord();
		if (!record) {
			break;
		}
		if (previousValue > record->conditionalProbability) {
			if (tape == outputTape1) {
				tape = outputTape2;
			}
			else {
				tape = outputTape1;
			}
		}
		tape->writeRecord(record);
		previousValue = record->conditionalProbability;
	}

	delete outputTape2;
	delete outputTape1;
	delete inputTape;
	delete output2;
	delete output1;
	delete input;
}
bool merge(char print) {
	mergePhases++;

	File* input1 = new File("1.csv", 0);
	File* input2 = new File("2.csv", 0);
	File* output = new File("copy.csv", 0);

	auto* inputTape1 = new ReadBuffer(input1);
	auto* inputTape2 = new ReadBuffer(input2);
	auto* outputTape = new WriteBuffer(output,print);

	double previous1 = 0, previous2 = 0;
	Probability* record1 = inputTape1->nextRecord();
	Probability* record2 = inputTape2->nextRecord();
	if (!record2) {
        delete outputTape;
        delete inputTape2;
        delete inputTape1;
        delete output;
        delete input2;
        delete input1;
		return false;
	}
	while (true) {
		if (record1 && record2) {
			if (record1->conditionalProbability < previous1) {
				while (record2 && record2->conditionalProbability > previous2) {
					outputTape->writeRecord(record2);
					previous2 = record2->conditionalProbability;
					record2 = inputTape2->nextRecord();
				}
				previous1 = 0;
				previous2 = 0;
			}
			else if (record2->conditionalProbability < previous2) {
				while (record1 && record1->conditionalProbability > previous1) {
					outputTape->writeRecord(record1);
					previous1 = record1->conditionalProbability;
					record1 = inputTape1->nextRecord();
				}
				previous1 = 0;
				previous2 = 0;
			}
			else {
				if (record1->conditionalProbability < record2->conditionalProbability) {
					outputTape->writeRecord(record1);
					previous1 = record1->conditionalProbability;
					record1 = inputTape1->nextRecord();
				}
				else {
					outputTape->writeRecord(record2);
					previous2 = record2->conditionalProbability;
					record2 = inputTape2->nextRecord();
				}
			}
		}
		else if (!record1) {
			while (record2) {
				outputTape->writeRecord(record2);
				record2 = inputTape2->nextRecord();
			}
			break;
		}
		else {
            while (record1) {
                outputTape->writeRecord(record1);
                record1 = inputTape1->nextRecord();
            }
            break;
        }
	}

	delete outputTape;
	delete inputTape2;
	delete inputTape1;
	delete output;
	delete input2;
	delete input1;
	return true;
}

void rewrite(std::string org, std::string cp) {
	File* input = new File(std::move(org), 0);
	File* output = new File(std::move(cp), 0);

	auto* original = new ReadBuffer(input);
	auto* copy = new WriteBuffer(output);
	std::cout << "\nFile before sorting: \n\n";
	while (copy->writeRecord(original->getRecord()));

	delete copy;
	delete original;
	delete output;
	delete input;
}
void rewriteSorted() {
	File* input = new File("1.csv", 0);
	File* output = new File("sorted.csv", 0);

	auto* original = new ReadBuffer(input);
	auto* copy = new WriteBuffer(output);

	std::cout << "\nFile after sorting: \n\n";

	while (copy->writeRecord(original->nextRecord()));
	copy->writeRemainingValues();

	remove("1.csv");
	remove("2.csv");
	delete copy;
	delete original;
	delete output;
	delete input;
}

int main() {
	
	std::string filename = "dataset.csv";
	bool sorted = false;
	char print;
	menu(filename);
	rewrite(filename, "copy.csv");
	std::cout << "Do you want to print file after every sorting phase? y/n" << std::endl;
	std::cin >> print;
	while (!sorted) {
		divide();
		sorted = !merge(print);
	}
	rewriteSorted();
	std::cout << "Finished sorting!" << std::endl;
	std::cout << std::endl;
	std::cout << "Disk writes: " << writeCount << std::endl;
	std::cout << "Disk reads: " << readCount << std::endl;
	std::cout << "Divide phases: " << dividePhases << std::endl;
	std::cout << "Merge phases: " << mergePhases << std::endl;
	return 0;
}


