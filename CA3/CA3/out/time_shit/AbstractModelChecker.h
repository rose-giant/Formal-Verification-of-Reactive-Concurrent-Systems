#ifndef __ABSTRACT_MODEL_CHECKER_H_
#define __ABSTRACT_MODEL_CHECKER_H_

#include <iostream>
#include <time.h>
#include <fstream>
#include <stdlib.h>

#include "Config.h"

#define SATISFIED 1
#define DEADLOCK 2
#define BOUND_REACHED 3
#define COUNTER_EXAMPLE 4
#define QUEUE_OVERFLOW 5
#define HEAP_OVERFLOW 6
#define UNKNOWN_PROPERTY 7
#define ASSERTION_FAILED 8
#define SEGMENTATION_FAULT 9

class AbstractModelChecker {
protected:
	ostream &out;
	int numberOfTransitions;
	int numberOfStates;
	time_t startTime;
	
	#ifdef EXPORT_STATE_SPACE
	    ostream &statespace;
	#endif

    virtual void printModelCheckingOptions() = 0;
    virtual long getConsumedMemory() = 0;

    virtual string getModelCheckingResultName(int result);
    

public:
	AbstractModelChecker(ostream &outStream 
		#ifdef EXPORT_STATE_SPACE
		    , ostream &statespace
		#endif
	);

	virtual void doModelChecking(string userSelectedPropertyName, string userSelectedPropertyType);
	virtual void exportModelCheckingDetails(string userSelectedPropertyName, 
		string userSelectedPropertyType, int result, string message);
		
	#ifdef PROGRESS_REPORT
		virtual void progressReport(ostream& reportStream);
	#endif
		
};

#ifdef PROGRESS_REPORT
	class MyNumPunct : public std::numpunct<char> {
	protected:
	    virtual char do_thousands_sep() const { return ','; }
	    virtual std::string do_grouping() const { return "\03"; }
	};
#endif
#endif