#include "Calibration.h"
#include <iostream>
#include <sstream>

std::vector<uint32_t> Calibration::findCalibrationValues(std::ifstream& file) {
	if (!file.is_open() || file.fail()) {
		throw std::runtime_error("Failed to open Calibration Document");
	}

	std::vector<uint32_t> calibrationValues;

	uint32_t value = 0;
	bool isSecondValue = false;
	uint32_t secondValue = 0;

	for (std::string line; std::getline(file, line);) {
		std::string::iterator ptr;
		for (ptr = line.begin(); ptr != line.end(); ptr++) {
			if (isdigit(*ptr)) {
				if (isSecondValue) {
					secondValue = (*ptr - '0');
				}
				else {
					value += ((*ptr - '0') << 3) + ((*ptr - '0') << 1);
					secondValue = (*ptr - '0');
					isSecondValue = true;
				}
			}
		}
		isSecondValue = false;
		value += secondValue;
		calibrationValues.push_back(value);
		value = 0;
		secondValue = 0;
	}

	return calibrationValues;
}