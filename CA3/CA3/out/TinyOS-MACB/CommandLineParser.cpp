#include "CommandLineParser.h"

void CommandLineParser::addOption(char shortName, string longName, string description, string argumentName) {
	Option option;
	option.shortName = shortName;
	option.longName = longName;
	option.description = description;
	option.hasArgument = true;
	option.argumentName = argumentName;
	
	dictionary[shortName] = option;
}

void CommandLineParser::addOption(char shortName, string longName, string description) {
	Option option;
	option.shortName = shortName;
	option.longName = longName;
	option.description = description;
	option.hasArgument = false;
	
	dictionary[shortName] = option;
}

bool CommandLineParser::hasOption(char shortName) {
	return parsedValues.find(shortName) != parsedValues.end();
}

string CommandLineParser::getOptionValue(char shortName) {
	return parsedValues.find(shortName)->second;
}

list<string>& CommandLineParser::getEnvironmentVariables() {
	return environmentVariables;
}


void CommandLineParser::parse(int size, char* arguments[]) {
	int cnt = 1;
	while (cnt < size) {
		if (arguments[cnt][0] != '-' || strlen(arguments[cnt]) < 2) {
			string errorMessage = "Incorrect input format.";
			throw Exception(errorMessage);
		}

		Option option;
		if (strlen(arguments[cnt]) == 2) {
			if (dictionary.find(arguments[cnt][1]) == dictionary.end()) {
				string errorMessage = "Unknown option \"";
				errorMessage += arguments[cnt];
				errorMessage += "\".";
				throw Exception(errorMessage);
			} else {
				option = dictionary[arguments[cnt][1]];
			}
		} else {
			if (arguments[cnt][1] == 'D') {
				option.hasArgument = false;
				option.shortName = 'D';
				string paramValue = arguments[cnt];
				paramValue = paramValue.substr(2, paramValue.length());
				environmentVariables.push_back(paramValue);
			} else {
				if (arguments[cnt][1] != '-') {
					string errorMessage = "Incorrect input format \"";
					errorMessage += arguments[cnt];
					errorMessage += "\".";
					throw Exception(errorMessage);
				}
				string key = arguments[cnt];
				key = key.substr(2);
				
				bool isFound = false;
				for (std::map<char,Option>::iterator it = dictionary.begin();
					it != dictionary.end(); ++it) {
					if (it->second.longName == key) {
						option = it->second;
						isFound = true;
						break;
					}
				}
				if (!isFound) {
					string errorMessage = "Unknown option \"";
					errorMessage += arguments[cnt];
					errorMessage += "\".";
					throw Exception(errorMessage);					
				}
			}
		}
		if (option.hasArgument) {
			if (cnt == size - 1) {
				string errorMessage = "Option \"";
				errorMessage += arguments[cnt];
				errorMessage += "\" requires value.";
				throw Exception(errorMessage);
			}
			parsedValues[option.shortName] = arguments[++cnt];
		} else {
			parsedValues[option.shortName] = "";
		}
		cnt++;
	}
}

void CommandLineParser::printHelp() {
	cout << "Usage: rmc [options]" << endl;
	for (std::map<char,Option>::iterator it = dictionary.begin();
		it != dictionary.end(); ++it) {
	
		string firstTerm = " -";
		firstTerm += it->first;
		firstTerm += ", --";
		firstTerm += it->second.longName;
		if (it->second.hasArgument)
			firstTerm += " <" + it->second.argumentName + ">";
		cout << left << setw(39) << setfill(' ') << firstTerm;
		cout << left << setw(40) << setfill(' ') << it->second.description.substr(0, 40) << endl;
		for (int cnt = 0; cnt < (it->second.description.length() - 1) / 40; cnt++) {
			cout << left << setw(39) << setfill(' ') << " ";
			cout << left << setw(40) << setfill(' ') << it->second.description.substr(
				(cnt + 1) * 40, 40) << endl;
		}
	}
}


int maintest(int argc, char* argv[]) {

	CommandLineParser parser;
	parser.addOption('o', "out", "output");
	parser.addOption('v', "version", "version of the code.version of the code.version of the code.version of the code.version of the code.version of the code.", "number");


	parser.parse(argc, argv);

	if (parser.hasOption('v')) {
		cout << parser.getOptionValue('v') << endl;
	}
}
