#pragma once
#include <iostream>
#include <map>
#include <boost/property_tree/ptree.hpp>

class Config {
public:
	Config(std::string, std::map<std::string, std::string>);
	std::string get(std::string);
	void set(std::string, std::string, bool = false);
	boost::property_tree::ptree* getConfig();
	void save();
private:
	boost::property_tree::ptree* configFile;
	std::string configFilePath;
};