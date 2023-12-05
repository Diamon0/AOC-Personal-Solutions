#include <iostream>
#include <cstdint>
#include <vector>
#include <fstream>
#include <numeric>
#include "Calibration.h"

int main() {
	std::ifstream file = std::ifstream("CD.txt");
	std::vector<uint32_t> calibrationValues = Calibration::findCalibrationValues(file);

	//Commented out to keep the console clean, I don't plan to use the NDEBUG ifndef for this since I'm strictly using Debug mode
	/*for (uint32_t value : calibrationValues) {
		std::cout << "Calibration Value: " << value << std::endl;
	}*/

	int sum = std::accumulate(calibrationValues.begin(), calibrationValues.end(), 0);
	std::cout << "Sum of Calibration Values: " << sum << std::endl;
}