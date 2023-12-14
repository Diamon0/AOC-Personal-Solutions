#include <iostream>
#include <cstdint>
#include <vector>
#include <fstream>
#include <numeric>
#include "Calibration.h"
#include "CubeGame.h"

int main() {
	std::ifstream file = std::ifstream("GL.txt");
	CubeGame::CubeGames cubeGame = CubeGame::CubeGames::initializeGames(file);
	file.close();

	std::vector<CubeGame::Game> foundGames = cubeGame.games;

	int sum = 0;

	for (CubeGame::Game& game : foundGames) {
		sum += game.maxRedCount * game.maxGreenCount * game.maxBlueCount;
	}
	std::cout << "The sum of every game's power is " << sum << std::endl;
}