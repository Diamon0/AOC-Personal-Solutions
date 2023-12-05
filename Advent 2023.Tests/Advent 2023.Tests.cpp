#include "pch.h"
#include "CppUnitTest.h"
#include <Calibration.h>
#include <vector>
#include <cstdint>
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Advent2023Tests
{
	TEST_CLASS(FunctionalityTests)
	{
	public:
		
		TEST_METHOD(CalibrationValuesTest)
		{
			std::ifstream file = std::ifstream("CDE.txt");
			std::array<uint32_t, 8> expectedValues = { 29, 83, 13, 24, 42, 14, 76, 62 };
			std::vector<uint32_t> actualValues = Calibration::findCalibrationValues(file);

			for (size_t i = 0; i < expectedValues.size(); i++) {
				Assert::AreEqual(expectedValues[i], actualValues[i]);
			}
		}
	};
}
