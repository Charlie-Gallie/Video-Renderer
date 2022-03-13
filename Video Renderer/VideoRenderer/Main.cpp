#include <iostream>
#include <map>
#include "Config.h"
#include "Logger.h";
#include "Renderer.h";

/*
	Values are saved regardless-
	Get the difference between session settings and file settings
*/

std::string configFilePath = "Config.json";

std::map<std::string, std::string> settings = {
	{"fps", "60"},
	{"inputFolderName", "Images"},
	{"inputFileExtension", "jpg"},
	{"inputNumberingDigits", "4"},
	{"outputFolderName", "Renders"},
	{"outputFileName", "render"},
	{"outputFileExtension", "mp4"},
	{"outputCount", "0"}
};

int main() {
	Config config(configFilePath, settings);
	Logger::list(&config, true);
	Renderer::render(&config);

	std::cout << "\nThank you, come again.\nPress any key to exit." << std::endl;
	std::system("pause > nul");
	return 0;
}