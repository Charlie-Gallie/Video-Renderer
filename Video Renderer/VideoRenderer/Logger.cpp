#include "Logger.h"
#include "Config.h"
#include <iostream>
#include <vector>

int longestCharCount(std::vector<std::string> words) {
	int longest = 0;
	for (std::string word : words)
		if (word.length() > longest)
			longest = word.length();
	return longest;
}

void Logger::list(Config* config, bool allowChanges) {
	std::vector<std::string> listOfKeys;
	for (auto& section : *config->getConfig()) {
		listOfKeys.push_back(section.first);
	}
	int
		longestKeyLength = longestCharCount(listOfKeys),
		spacerPadding = 4;
	std::string spacerCharacter = "."; // Could be char but putting multiple characters is valid so there's no reason to limit that.
	for (auto& section : *config->getConfig()) {
		std::ostringstream os;
		std::fill_n(std::ostream_iterator<std::string>(os), longestKeyLength - section.first.length() + 4, spacerCharacter);
		std::cout << section.first << os.str() << section.second.get_value<std::string>() << std::endl;
	}
	while (allowChanges) {
		std::system("cls");
		bool saveValue = false;
		list(config);
		std::cout << "\nChange a value or type \"confirm/c\" to continue: ";
		std::string userInput;
		std::cin >> userInput;
		if (userInput == "confirm" || userInput == "c") allowChanges = false;
		if (userInput[0] == '*') {
			saveValue = true;
			userInput.erase(0, 1);
		}
		for (auto& section : *config->getConfig()) {
			if (userInput == section.first) {
				std::cout << "New value for \"" << userInput << "\": ";
				std::string newValue;
				std::cin >> newValue;
				config->set(userInput, newValue, saveValue);
			}
		}
	}
}