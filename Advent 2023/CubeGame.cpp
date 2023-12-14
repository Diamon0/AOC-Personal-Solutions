#include "CubeGame.h"
#include <string>
#include <iostream>
#include <array>
#include <fstream>
#include <iterator>

// Previously used for finding the ID
//static size_t ID_START_INDEX = 5;

using namespace CubeGame;

// Builder for the CubeGames, takes a file with a list of games in the format:
// Game x: x red, x green, x blue; x blue, x red; x green
CubeGames CubeGames::initializeGames(std::ifstream& gameLog) {
	if (!gameLog.is_open() || gameLog.fail()) {
		throw std::runtime_error("Failed to open Games Log");
	}

	CubeGames instance{};

	for (std::string line; std::getline(gameLog, line);) {
		instance.games.push_back(CubeGames::parseGameLogLine(instance, line));
	}

	return instance;
}

Game CubeGames::parseGameLogLine(CubeGames& instance, std::string& line) {
	std::string_view view{ line };

	// Ignored since game IDs are always 1 to x, but keeping it in case I need it
	/*size_t colonIndex = line.find_first_of(':');

	std::string_view id = view.substr(ID_START_INDEX, colonIndex - ID_START_INDEX);
	std::cout << id << std::endl;*/

	// String view with the game info
	size_t gameInfoStartIndex = view.find_first_of(':') + 2;
	std::string_view gameInfo = view.substr(gameInfoStartIndex);

	CubeCounts cubeCounts = CubeGames::getCubeCounts(gameInfo);

	Game game{};
	instance.gameCount++;
	game.id = instance.gameCount;
	game.maxRedCount = cubeCounts.redCubes;
	game.maxGreenCount = cubeCounts.greenCubes;
	game.maxBlueCount = cubeCounts.blueCubes;

	/*std::cout << "Game " << game.id << std::endl;
	std::cout << "Red cubes: " << (int)game.maxRedCount << std::endl;
	std::cout << "Green cubes: " << (int)game.maxGreenCount << std::endl;
	std::cout << "Blue cubes: " << (int)game.maxBlueCount << std::endl << std::endl;*/

	return game;
}

CubeCounts CubeGames::getCubeCounts(std::string_view& view) {
	CubeCounts counts{};
	counts.redCubes = 0;
	counts.greenCubes = 0;
	counts.blueCubes = 0;

	size_t setCount = 0;
	std::vector<size_t> setStartIndices{ 0 };
	std::vector<size_t> setEndIndices{};
	size_t currentIndex = 0;

	std::string_view::const_iterator iter = view.cbegin();
	while (iter != view.cend()) {
		if (*iter == ';') {
			setCount++;
			setEndIndices.push_back(currentIndex);
		}
		iter++;
		currentIndex++;
	}
	setEndIndices.push_back(currentIndex);

	for (size_t i = 1; i <= setEndIndices.size() - 1; i++) {
		setStartIndices.push_back(setEndIndices[i - 1] + 2);
	}

	for (size_t i = 0; i <= setCount; i++) {
		std::string_view set = view.substr(setStartIndices[i], setEndIndices[i] - setStartIndices[i]);
		
		std::string_view::const_iterator setIter = set.cbegin();

		do {
			uint8_t quantity = 0;

			if (*setIter == ',') {
				std::advance(setIter, 2);
				continue;
			}

			if (isdigit(*(std::next(setIter)))) {
				// No bit shifting for x10 this time, it would make it too unreadable with little to no payoff
				quantity = uint8_t((*setIter - '0') * 10) + uint8_t(*(std::next(setIter)) - '0');
				std::advance(setIter, 3);
			}
			else if (isdigit(*setIter)) {
				quantity = uint8_t(*setIter - '0');
				std::advance(setIter, 2);
			}
			
			const char& key = *setIter;

			if (key == 'r') {
				if (quantity > counts.redCubes)
				counts.redCubes = quantity;
				std::advance(setIter, 3);
			}
			else if (key == 'g') {
				if (quantity > counts.greenCubes)
				counts.greenCubes = quantity;
				std::advance(setIter, 5);
			}
			else if (key == 'b') {
				if (quantity > counts.blueCubes)
				counts.blueCubes = quantity;
				std::advance(setIter, 4);
			}
				
		} while (setIter != set.cend());
	}

	return counts;
}

std::vector<Game> CubeGames::findGamesByLimitedCubes(uint8_t maxReds = 0, uint8_t maxGreens = 0, uint8_t maxBlues = 0) {
	std::vector<Game> results{};
	for (Game& game : games) {
		if (game.maxRedCount <= maxReds && game.maxGreenCount <= maxGreens && game.maxBlueCount <= maxBlues) {
			results.push_back(game);
		}
	}
	return results;
}