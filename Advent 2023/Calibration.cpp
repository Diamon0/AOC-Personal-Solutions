#include "Calibration.h"
#include <iostream>
#include <sstream>
#include <string>
#include <regex>

uint32_t toInteger(std::string stringValue, bool isFirst, bool isInverted) {
	std::string lowercaseString = stringValue.data();

	if (isInverted) {
		std::string reversedString = lowercaseString.data();
		//Essentially reverse the order and lowercase it
		std::transform(lowercaseString.rbegin(), lowercaseString.rend(), reversedString.begin(), [](unsigned char c) { return std::tolower(c); });
		lowercaseString = reversedString;
	} else {
		std::transform(lowercaseString.begin(), lowercaseString.end(), lowercaseString.begin(), [](unsigned char c) { return std::tolower(c); });
	}

	//EXTREMELY EVIL EDGE CASE IF NESTING
	//Catching the (isFirst) case, since we have to take the first number (1-9) instead of the last (0)
	if (isFirst) {
		if (lowercaseString == "ten" || lowercaseString == "eleven" || lowercaseString == "twelve" || lowercaseString == "thirteen" || lowercaseString == "fourteen" || lowercaseString == "fifteen" || lowercaseString == "sixteen" || lowercaseString == "seventeen" || lowercaseString == "eighteen" || lowercaseString == "nineteen") {
			return 1;
		}if (lowercaseString == "twenty") {
			return 2;
		}if (lowercaseString == "thirty") {
			return 3;
		}if (lowercaseString == "forty") {
			return 4;
		}if (lowercaseString == "fifty") {
			return 5;
		}if (lowercaseString == "sixty") {
			return 6;
		}if (lowercaseString == "seventy") {
			return 7;
		}if (lowercaseString == "eighty") {
			return 8;
		}if (lowercaseString == "ninety") {
			return 9;
		}
	}

	//EXTREMELY EVIL IF NESTING
	//The first if is the reason I have an "isFirst" parameter
	if (lowercaseString == "0" || lowercaseString == "zero" || lowercaseString == "ten" || lowercaseString == "twenty" || lowercaseString == "thirty" || lowercaseString == "forty" || lowercaseString == "fifty" || lowercaseString == "sixty" || lowercaseString == "seventy" || lowercaseString == "eighty" || lowercaseString == "ninety") {
		return 0;
	}if (lowercaseString == "1" || lowercaseString == "one" || lowercaseString == "eleven") {
		return 1;
	}if (lowercaseString == "2" || lowercaseString == "two" || lowercaseString == "twelve") {
		return 2;
	}if (lowercaseString == "3" || lowercaseString == "three" || lowercaseString == "thirteen") {
		return 3;
	}if (lowercaseString == "4" || lowercaseString == "four" || lowercaseString == "fourteen") {
		return 4;
	}if (lowercaseString == "5" || lowercaseString == "five" || lowercaseString == "fifteen") {
		return 5;
	}if (lowercaseString == "6" || lowercaseString == "six" || lowercaseString == "sixteen") {
		return 6;
	}if (lowercaseString == "7" || lowercaseString == "seven" || lowercaseString == "seventeen") {
		return 7;
	}if (lowercaseString == "8" || lowercaseString == "eight" || lowercaseString == "eighteen") {
		return 8;
	}if (lowercaseString == "9" || lowercaseString == "nine" || lowercaseString == "nineteen") {
		return 9;
	}
}

// Finds the callibration values in a document
// Maybe using Regex and a bunch of ifs is not the best way to do it, but it sure works
// Thank god my cpu is fast tho (Although I think I saw something using CUDA under the hood, but I'll have to look into it later, may be an option for future projects)
std::vector<uint32_t> Calibration::findCalibrationValues(std::ifstream& file) {
	if (!file.is_open() || file.fail()) {
		throw std::runtime_error("Failed to open Calibration Document");
	}

	std::vector<uint32_t> calibrationValues;

	//Now unused because of part 2
	/*uint32_t value = 0;
	bool isSecondValue = false;
	uint32_t secondValue = 0;*/

	for (std::string line; std::getline(file, line);) {

		std::regex regex("(\\d|(twen|thir|for|fif|six|seven|eigh|nine)ty|ten|eleven|twelve|thirteen|fourteen|fifteen|sixteen|seventeen|eighteen|nineteen|zero|one|two|three|four|five|six|seven|eight|nine)", std::regex_constants::icase);

		// I looked at sregex_iterator with the intent to expand on it... No yeah, I'd rather just match the leftmost and the rightmost
		// That was nightmare fuel, and I don't feel like simply making my own iterator right now, so I'll take the easy and time-efficient way out
		// ...
		// Anyways, have an inverted version of the other regex, I'll just do a forward and a backwards
		std::regex regexInverted("(\\d|yt(newt|riht|rof|fif|xis|neves|hgie|enin)|net|nevele|evlewt|neetriht|neetruof|neetfif|neetxis|neetneves|neethgie|neetenin|orez|eno|owt|eerht|ruof|evif|xis|neves|thgie|enin)", std::regex_constants::icase);
		
		//I don't really need a vector anymore, do I
		/*std::vector<std::string> matches{};*/

		//Naw, this was not it chief, I couldn't handle the "eightwo" edge case without making my own iterator behavior
		/*std::sregex_iterator iter(line.begin(), line.end(), regex, std::regex_constants::match_default);
		std::sregex_iterator iterEnd = std::sregex_iterator();*/

		/*for (std::sregex_iterator i = iter; i != iterEnd; ++i) {
			matches.push_back((*i).str());
		}*/

		std::match_results<std::string::const_iterator> firstMatch;
		std::match_results<std::reverse_iterator<std::string::const_iterator>> lastMatch;

		std::regex_search(line.cbegin(), line.cend(), firstMatch, regex);
		std::regex_search(line.crbegin(), line.crend(), lastMatch, regexInverted);

		uint32_t fNumber = toInteger(firstMatch.str(), true, false);
		uint32_t lNumber = toInteger(lastMatch.str(), false, true);

		calibrationValues.push_back((fNumber << 3) + (fNumber << 1) + lNumber);

		//Now unused because of part 2
		//std::string::iterator ptr;
		//for (ptr = line.begin(); ptr != line.end(); ptr++) {
		//	if (isdigit(*ptr)) {
		//		if (isSecondValue) {
		//			secondValue = (*ptr - '0');
		//		}
		//		else {
		//			//Shift digit to the 10th position (digit*8 + digit*2)
		//			value += ((*ptr - '0') << 3) + ((*ptr - '0') << 1);
		//			secondValue = (*ptr - '0');
		//			isSecondValue = true;
		//		}
		//	}
		//	else {

		//	}
		//}

		//isSecondValue = false;
		//value += secondValue;
		//calibrationValues.push_back(value);
		//value = 0;
		//secondValue = 0;
	}

	return calibrationValues;
}