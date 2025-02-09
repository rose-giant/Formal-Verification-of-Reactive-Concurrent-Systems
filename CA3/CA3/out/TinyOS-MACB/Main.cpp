                                          







#include "Types.h"
#include "Config.h"

#include "CommandLineParser.h"

		#include "TimedModelChecker.h"

#include <signal.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdexcept>

using namespace std;

	 int _ref_samplingRate = 50;
 
	 int _ref_numberOfNodes = 6;
 
	 int _ref_bufferSize = 4;
 
	 int _ref_sensorTaskDelay = 2;
 
	 int _ref_tmdaSlotSize = 10;
 
	 int _ref_miscPeriod = 120;
 
	 int _ref_OnePacketTransmissionTime = 7;
 
	 int _ref_miscTaskDelay = 10;
 
	 int _ref_packetMaximumSize = 112;
 


		#ifdef EXPORT_STATE_SPACE
		ofstream statespace;
		#endif
		TimedModelChecker *analyzer;

string operator+(const string left, const int right) {
	return left + std::to_string((long long)right);
}
string operator+(const string left, const short right) {
	return left + std::to_string((long long)right);
}
string operator+(const string left, const float right) {
	return left + std::to_string((long double)right);
}
string operator+(const string left, const double right) {
	return left + std::to_string((long double)right);
}
string operator+(const string left, const byte right) {
	return left + std::to_string((long long)right);
}
string operator+(const string left, const boolean right) {
	return left + (right ? "true" : "false");
}

string operator+(const string left, const AbstractActor *right) {
	return left + right->getName();
}

void segmentationFaultHandler(int signum) {
	analyzer->segmentationFault();
	exit(0);
}

#ifdef PROGRESS_REPORT
#include <thread> 
#include <chrono> 
ostream *progressReportStream;
boolean continueReportProgress;
void progressReport() {
	while(continueReportProgress) {
		analyzer->progressReport(*progressReportStream);
	    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
}
#endif

int BOOLEAN_SIZE, BYTE_SIZE, SHORT_SIZE, INT_SIZE, FLOAT_SIZE, DOUBLE_SIZE, REACTIVE_CLASS_SIZE;
int main(int argc, char* argv[]) {

	BOOLEAN_SIZE = sizeof(boolean);
	BYTE_SIZE = sizeof(byte);
	SHORT_SIZE = sizeof(short);
	INT_SIZE = sizeof(int);
	FLOAT_SIZE = sizeof(float);
	DOUBLE_SIZE = sizeof(double);
	REACTIVE_CLASS_SIZE = sizeof(void*);
	
	signal(SIGSEGV, segmentationFaultHandler);

	CommandLineParser parser;
	parser.addOption('h', "help", "Print this message.");
	parser.addOption('s', "hashmapSize", "Set the size of the hashmap to \"2^value\" (default size is 20 and the value should be bigger that 20).", "value");
	parser.addOption('o', "exportResult", "Exporting the model checking result in file \"fileName\".", "fileName");
	#ifdef PROGRESS_REPORT
		parser.addOption('g', "progress", "Report progress in file \"fileName\".", "fileName");
	#endif
	
	#ifdef MONTE_CARLO
	parser.addOption('e', "error", "Error value of the calculated results", "value");
	parser.addOption('d', "delta", "Confidence value of the calculated results", "fileName");
	#endif
	
	    #ifdef EXPORT_STATE_SPACE	
		parser.addOption('x', "exportStatespace", "Exporting the state space in file \"fileName\".", "fileName");
		#endif
	    
	try {
		parser.parse(argc, argv);
	} catch (Exception e) {
		cout << e.getMessage() << endl;
		parser.printHelp();
		exit(0);
	}
	if (parser.hasOption('h')) {
		parser.printHelp();
		exit(0);
	}
	
    int hashmapSize = 24;
	ostream *result = &cout;

	if (parser.hasOption('s')) {
		int newSize = atoi(parser.getOptionValue('s').c_str());
		hashmapSize = newSize > hashmapSize ? newSize : hashmapSize;
	}
	if (parser.hasOption('o')) {
		result = new ofstream(parser.getOptionValue('o').c_str(), ofstream::out|ofstream::trunc);
	}
	#ifdef PROGRESS_REPORT
		if (parser.hasOption('g')) {
			progressReportStream = new ofstream(parser.getOptionValue('g').c_str(), ofstream::out|ofstream::trunc);
		}
	#endif

	for (list<string>::iterator it = parser.getEnvironmentVariables().begin(); it != parser.getEnvironmentVariables().end(); ++it) {
		string envVar = *it;
		int index = envVar.find("=");
		if (index == string::npos || index == 0 || index == envVar.length() - 1)
			continue;
		string name = envVar.substr(0, index);
		string value = envVar.substr(index + 1, envVar.length());
				
				if (name == "samplingRate")				
					try {
							_ref_samplingRate = stoi(value);
					} catch(std::invalid_argument e) {
						cout << "<!--Invalid value for variable \"samplingRate\".-->" << endl;
					} catch(std::out_of_range e) {
						cout << "<!--Value out of range for variable \"samplingRate\".-->" << endl;
					}

					
				
				if (name == "numberOfNodes")				
					try {
							_ref_numberOfNodes = stoi(value);
					} catch(std::invalid_argument e) {
						cout << "<!--Invalid value for variable \"numberOfNodes\".-->" << endl;
					} catch(std::out_of_range e) {
						cout << "<!--Value out of range for variable \"numberOfNodes\".-->" << endl;
					}

					
				
				if (name == "bufferSize")				
					try {
							_ref_bufferSize = stoi(value);
					} catch(std::invalid_argument e) {
						cout << "<!--Invalid value for variable \"bufferSize\".-->" << endl;
					} catch(std::out_of_range e) {
						cout << "<!--Value out of range for variable \"bufferSize\".-->" << endl;
					}

					
				
				if (name == "sensorTaskDelay")				
					try {
							_ref_sensorTaskDelay = stoi(value);
					} catch(std::invalid_argument e) {
						cout << "<!--Invalid value for variable \"sensorTaskDelay\".-->" << endl;
					} catch(std::out_of_range e) {
						cout << "<!--Value out of range for variable \"sensorTaskDelay\".-->" << endl;
					}

					
				
				if (name == "tmdaSlotSize")				
					try {
							_ref_tmdaSlotSize = stoi(value);
					} catch(std::invalid_argument e) {
						cout << "<!--Invalid value for variable \"tmdaSlotSize\".-->" << endl;
					} catch(std::out_of_range e) {
						cout << "<!--Value out of range for variable \"tmdaSlotSize\".-->" << endl;
					}

					
				
				if (name == "miscPeriod")				
					try {
							_ref_miscPeriod = stoi(value);
					} catch(std::invalid_argument e) {
						cout << "<!--Invalid value for variable \"miscPeriod\".-->" << endl;
					} catch(std::out_of_range e) {
						cout << "<!--Value out of range for variable \"miscPeriod\".-->" << endl;
					}

					
				
				if (name == "OnePacketTransmissionTime")				
					try {
							_ref_OnePacketTransmissionTime = stoi(value);
					} catch(std::invalid_argument e) {
						cout << "<!--Invalid value for variable \"OnePacketTransmissionTime\".-->" << endl;
					} catch(std::out_of_range e) {
						cout << "<!--Value out of range for variable \"OnePacketTransmissionTime\".-->" << endl;
					}

					
				
				if (name == "miscTaskDelay")				
					try {
							_ref_miscTaskDelay = stoi(value);
					} catch(std::invalid_argument e) {
						cout << "<!--Invalid value for variable \"miscTaskDelay\".-->" << endl;
					} catch(std::out_of_range e) {
						cout << "<!--Value out of range for variable \"miscTaskDelay\".-->" << endl;
					}

					
				
				if (name == "packetMaximumSize")				
					try {
							_ref_packetMaximumSize = stoi(value);
					} catch(std::invalid_argument e) {
						cout << "<!--Invalid value for variable \"packetMaximumSize\".-->" << endl;
					} catch(std::out_of_range e) {
						cout << "<!--Value out of range for variable \"packetMaximumSize\".-->" << endl;
					}

					
    }
		#ifdef EXPORT_STATE_SPACE
		if (parser.hasOption('x')) {
			statespace.open(parser.getOptionValue('x').c_str(), ofstream::out|ofstream::trunc);
		} else {
			statespace.open("statespace.xml", ofstream::out|ofstream::trunc);
			#ifdef DEBUG_LEVEL_2 
			cout.rdbuf(statespace.rdbuf());
			#endif
			
		}
		#endif
		analyzer = new TimedModelChecker(*result
	    	#ifdef EXPORT_STATE_SPACE
	    		, statespace
			#endif
			, hashmapSize
			);
		#ifdef PROGRESS_REPORT
			continueReportProgress = true;
			std::thread t1(progressReport);
		#endif
		analyzer->doModelChecking("Deadlock-Freedom and No Deadline Missed", "Reachability");
		#ifdef PROGRESS_REPORT
			continueReportProgress = false;
			t1.join();
		#endif
	
    if (parser.hasOption('o')) {
        ((ofstream*)result)->close();
    }
    return 0;
}
