#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include "Config.h"

class Renderer {
public:
	static void render(Config*);
private:
	static void renameImages(Config*, int*);
	static std::string addSlash(std::string);
	static void prepareFiles(Config*, int*);
	static int selectCodec(std::string);
	cv::VideoWriter selectedCodec;
};