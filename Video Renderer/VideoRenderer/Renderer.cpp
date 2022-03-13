#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>
#include <errno.h>
#include <utility>
#include <vector>
#include <string>
#include "Renderer.h"
#include "Config.h"

void Renderer::render(Config* config) {	
	int totalFrames = 0, framesRendered = 0;
	prepareFiles(config, &totalFrames);

	if (totalFrames < 1) {
		std::cout << "You don't have any images.\nPlease put your frames into \"" << config->get("inputFolderName") << "\" then render." << std::endl;
		return;
	}

	std::string inputPath =
		addSlash(config->get("inputFolderName")) +
		"%0" + config->get("inputNumberingDigits") + "d" +
		"." + config->get("inputFileExtension");
	std::string outputPath =
		addSlash(config->get("outputFolderName")) +
		config->get("outputFileName") +
		config->get("outputCount") +
		"." + config->get("outputFileExtension");

	cv::VideoCapture inputStream(inputPath);
	cv::VideoWriter outputStream(
		outputPath,
		selectCodec(config->get("outputFileExtension")),
		std::stoi(config->get("fps")),
		cv::Size(1920, 1080)
	);

	for (;;) {
		cv::Mat frame;
		inputStream >> frame;
		if (frame.empty()) {
			config->set("outputCount", std::to_string(std::stoi(config->get("outputCount")) + 1), true);
			std::cout << "Rendered! Find your video at:\n" << outputPath << std::endl;
			return;
		}
		outputStream.write(frame);
		framesRendered++;
		if (framesRendered % 10 == 0) std::cout << "Rendered Frames: " << framesRendered << "/" << totalFrames << std::endl;
	}
}

void Renderer::renameImages(Config* config, int* totalFrames) {
	boost::filesystem::path imagesFolder(config->get("inputFolderName"));
	boost::filesystem::directory_iterator dirItr;
	for (boost::filesystem::directory_iterator itr(imagesFolder); itr != dirItr; ++itr) {
		if (is_regular_file(itr->path())) {
			std::string originalName = itr->path().string();
			std::string newName = originalName;
			int charactersFromEnd =
				config->get("inputFileExtension").length() +
				std::stoi(config->get("inputNumberingDigits")) + 
				1; // To account for the '.' in the file name
			int charactersFromBeginning =
				addSlash(config->get("inputFolderName")).length();
			newName.replace(
				newName.begin() +
				charactersFromBeginning,
				newName.end() - charactersFromEnd,
				""
			);
			if (std::rename(originalName.c_str(), newName.c_str()) == -1) {
				std::cout << "Could not rename \"" << originalName << "\":\n" << std::endl;
			}
			*totalFrames += 1;
		}
	}
}

void Renderer::prepareFiles(Config* config, int* totalFrames) {
	if (!boost::filesystem::exists(config->get("inputFolderName"))) boost::filesystem::create_directory(config->get("inputFolderName"));
	if (!boost::filesystem::exists(config->get("outputFolderName"))) boost::filesystem::create_directory(config->get("outputFolderName"));
	renameImages(config, totalFrames);
}

int Renderer::selectCodec(std::string fileExtension) {
	std::vector<std::pair<std::string, int>> codecPairs = {
		{"mp4", cv::VideoWriter::fourcc('a', 'v', 'c', '1')}
	};
	for (auto& codec : codecPairs) {
		if (codec.first == fileExtension) return codec.second;
	}
	std::cout << "Could not find matching codec for \"." << fileExtension << "\", using .mp4 by default." << std::endl;
	return cv::VideoWriter::fourcc('a', 'v', 'c', '1');
}

// Just to make the code a little clearer
std::string Renderer::addSlash(std::string txt) {
	return txt + "/";
}