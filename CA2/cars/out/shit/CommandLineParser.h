#ifndef __COMMAND_LINE_PARSER__
#define __COMMAND_LINE_PARSER__

#include <iostream>
#include <iomanip>
#include <map>
#include <list>

#include "Types.h"
#include <string.h>

using namespace std;

struct Option {
	char shortName;
	string longName;
	string description;
	bool hasArgument;
	string argumentName;
};

class CommandLineParser {
private:
	map<char, Option> dictionary;
	map<char, string> parsedValues;
	list<string> environmentVariables;
public:
	void addOption(char shortName, string longName, string description, string argumentName);
	void addOption(char shortName, string longName, string description);

	bool hasOption(char shortName);
	string getOptionValue(char shortName);
	list<string>& getEnvironmentVariables();

	void parse(int size, char* arguments[]);

	void printHelp();
};

#endif