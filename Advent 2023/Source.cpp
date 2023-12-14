#include <iostream>
#include <cstdint>
#include <vector>
#include <fstream>
#include <numeric>
#include "Calibration.h"
#include "CubeGame.h"

int main() {
	std::ifstream file = std::ifstream("GLE.txt");
	CubeGame::CubeGames cubeGame = CubeGame::CubeGames::initializeGames(file);
	file.close();

	uint8_t maxReds = 12;
	uint8_t maxGreens = 13;
	uint8_t maxBlues = 14;

	std::vector<CubeGame::Game> foundGames = cubeGame.findGamesByLimitedCubes(maxReds, maxGreens, maxBlues);

	int sum = 0;

	std::cout << "Found " << foundGames.size() << " Games with a maximum of " << (int)maxReds << " Reds, " << (int)maxGreens << " Greens, and " << (int)maxBlues << " Blues" << std::endl;
	std::cout << "The IDs are:";
	for (CubeGame::Game& game : foundGames) {
		std::cout << " " << game.id;
		sum += game.id;
	}
	std::cout << std::endl;
	std::cout << "The sum of all Game IDs is " << sum << std::endl;
}