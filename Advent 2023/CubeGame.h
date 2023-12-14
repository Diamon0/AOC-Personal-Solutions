#pragma once
#include <cstdint>
#include <fstream>
#include <vector>

#ifndef CUBE_GAME
#define CUBE_GAME

namespace CubeGame {

	struct Game {
	public:
		uint32_t id;
		uint8_t maxRedCount;
		uint8_t maxGreenCount;
		uint8_t maxBlueCount;

		friend bool operator==(const Game& left, const Game& right) {
			return left.id == right.id && left.maxRedCount == right.maxRedCount && left.maxGreenCount == right.maxGreenCount && left.maxBlueCount == right.maxBlueCount;
		}

	};

	struct CubeCounts {
		uint8_t redCubes;
		uint8_t greenCubes;
		uint8_t blueCubes;

		friend bool operator==(const CubeCounts& left, const CubeCounts& right) {
			return left.redCubes == right.redCubes && left.greenCubes == right.greenCubes && left.blueCubes == right.blueCubes;
		}
	};

	class CubeGames {
	public:

		std::vector<Game> games{};

		uint32_t gameCount = 0;

		// Builder for the CubeGames, takes a file with a list of games in the format:
		// Game x: x red, x green, x blue; x blue, x red; x green
		static CubeGames initializeGames(std::ifstream& gameLog);

		static Game parseGameLogLine(CubeGames& instance, std::string& line);

		static CubeCounts getCubeCounts(std::string_view& view);

		std::vector<Game> findGamesByLimitedCubes(uint8_t maxReds, uint8_t maxGreens, uint8_t maxBlues);
	};
}

#endif CUBE_GAME