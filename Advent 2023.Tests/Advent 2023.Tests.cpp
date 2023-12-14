#include "pch.h"
#include "CppUnitTest.h"
#include <Calibration.h>
#include <CubeGame.h>
#include <vector>
#include <cstdint>
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Advent2023Tests
{
	TEST_CLASS(CalibrationClass) {
	public:
		TEST_METHOD(FindCalibrationValues)
		{
			std::array<uint32_t, 8> expectedValues = { 29, 83, 13, 24, 42, 14, 76, 62 };

			std::ifstream file = std::ifstream("CDE.txt");
			std::vector<uint32_t> actualValues = Calibration::findCalibrationValues(file);
			file.close();

			for (size_t i = 0; i < expectedValues.size(); i++) {
				Assert::AreEqual(expectedValues[i], actualValues[i]);
			}
		}
	};

	TEST_CLASS(CubeGameClass) {
	public:
		TEST_METHOD(GetCubeCounts)
		{
			CubeGame::CubeCounts expectedResult{};
			expectedResult.redCubes = 4;
			expectedResult.greenCubes = 2;
			expectedResult.blueCubes = 6;

			std::ifstream file = std::ifstream("GLE.txt");
			std::string line;
			std::getline(file, line);
			file.close();

			std::string_view view{ line };
			std::string_view gameInfo = view.substr(view.find_first_of(':') + 2);

			CubeGame::CubeCounts actualResult = CubeGame::CubeGames::getCubeCounts(gameInfo);

			Assert::IsTrue(expectedResult == actualResult);
		}

		TEST_METHOD(LogLineParsingTest)
		{
			CubeGame::Game expectedResult{};
			expectedResult.id = 1;
			expectedResult.maxRedCount = 4;
			expectedResult.maxGreenCount = 2;
			expectedResult.maxBlueCount = 6;

			CubeGame::CubeGames emptyGame{};
			emptyGame.gameCount = 0;

			std::ifstream file = std::ifstream("GLE.txt");
			std::string line;
			std::getline(file, line);
			file.close();

			CubeGame::Game actualResult = CubeGame::CubeGames::parseGameLogLine(emptyGame, line);

			Assert::IsTrue(expectedResult == actualResult);
		}

		TEST_METHOD(CubeGameTest)
		{
			std::array<uint32_t, 3> expectedIDs = { 1, 2, 5 };

			std::ifstream file = std::ifstream("GLE.txt");
			CubeGame::CubeGames gameObject = CubeGame::CubeGames::initializeGames(file);
			file.close();

			uint8_t maxReds = 12;
			uint8_t maxGreens = 13;
			uint8_t maxBlues = 14;

			std::vector<CubeGame::Game> foundGames = gameObject.findGamesByLimitedCubes(maxReds, maxGreens, maxBlues);

			for (size_t i = 0; i < foundGames.size(); i++) {
				Assert::AreEqual(expectedIDs[i], foundGames[i].id);
			}
		}
	};
}
