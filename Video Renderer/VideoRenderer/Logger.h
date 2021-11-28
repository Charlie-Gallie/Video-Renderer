#pragma once
#include "Config.h"
#include <iostream>
#include <map>

class Logger {
public:
	static void list(Config*, bool = false);
};