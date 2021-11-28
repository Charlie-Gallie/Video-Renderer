#include "Config.h"
#include <iostream>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>

Config::Config(std::string filePath, std::map<std::string, std::string> defaultValues) {
	configFile = new boost::property_tree::ptree();
	configFilePath = filePath;
	if (boost::filesystem::exists(filePath)) {
		std::ifstream existingConfigFile(filePath);
		std::stringstream configStringStream;
		configStringStream << existingConfigFile.rdbuf();
		boost::property_tree::json_parser::read_json(configStringStream, *configFile);
		existingConfigFile.close();
	}
	else {
		for (auto& entry : defaultValues) configFile->put(entry.first, entry.second);
		try {
			save();
		}
		catch (std::exception& exception) {
			std::cout << "Could not create \"" << filePath << "\":\n" << exception.what() << std::endl;
		}
	}
}

std::string Config::get(std::string key) {
	try {
		return configFile->get<std::string>(key);
	}
	catch (std::exception& exception) {
		std::cout << "Could not get the value \"" << key << "\" (Deleting your config file may fix it):\n" << exception.what() << std::endl;
	}
	return "Invalid config file";
}

void Config::set(std::string key, std::string value, bool saveFile) {
	try {
		configFile->put(key, value);
		if(saveFile) save();
	}
	catch(std::exception& exception) {
		std::cout << "Could not set the value \"" << key << " = " << value << "\":\n" << exception.what() << std::endl;
	}
}

void Config::save() {
	try {
		std::stringstream configStringStream;
		boost::property_tree::json_parser::write_json(configStringStream, *configFile);
		std::ofstream newConfigFile(configFilePath);
		newConfigFile << configStringStream.str();
		newConfigFile.close();
	}
	catch (std::exception& exception) {
		std::cout << "Could not save config:\n" << exception.what() << std::endl;
	}
}

boost::property_tree::ptree* Config::getConfig() {
	return configFile;
}