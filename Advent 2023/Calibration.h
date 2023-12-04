#pragma once
#include <cstdint>
#include <vector>
#include <array>
#include <utility>
#include <fstream>

#ifndef CALIBRATION
#define CALIBRATION

namespace Calibration {
	std::vector<uint32_t> findCalibrationValues(std::ifstream& file);
}
#endif