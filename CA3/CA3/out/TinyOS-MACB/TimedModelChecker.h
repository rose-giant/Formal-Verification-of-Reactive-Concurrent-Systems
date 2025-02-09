#ifndef __ABSTRACT_TIMED_MODEL_CHECKER_H_
#define __ABSTRACT_TIMED_MODEL_CHECKER_H_

#include "AbstractModelChecker.h"
#include "AbstractTimedRebecaAnalyzer.h"

#include <iostream>
#include <locale>

#define DEADLINE_MISSED 10

class TimedModelChecker : public AbstractModelChecker, public AbstractTimedRebecaAnalyzer {

protected:
	using AbstractTimedRebecaAnalyzer::exportState;
	using AbstractTimedRebecaAnalyzer::exportTransition;
	
	virtual string getModelCheckingResultName(int id);
    virtual long getConsumedMemory();
    virtual void printCounterExample(int result, AbstractActor* causedBy);
    virtual void printCounterExampleTransition(TimedBFSState* parent, TimedBFSState* child);

    virtual void printModelCheckingOptions();

	virtual void exportModelCheckingReportExtraDetails();
	
public:

	TimedModelChecker(std::ostream &out 
		#ifdef EXPORT_STATE_SPACE
		    , ostream &statespace
		#endif
		, int hashmapSize);
	virtual void doModelChecking(string userSelectedPropertyName, string userSelectedPropertyType);
	virtual void doTimedModelChecking(string userSelectedPropertyName, string userSelectedPropertyType);
	virtual void exportModelCheckingResult(string userSelectedPropertyName, 
		string userSelectedPropertyType, int result, string message, AbstractActor* causedBy);






	virtual void storeInitialState();
	virtual long executeRebec(int rebecId);
	virtual void exportProgressOfTimeTransition(OpenBorderNode &source,
		OpenBorderNode &destination, TIME_TYPE time, TIME_TYPE executionTime, TIME_TYPE shift, ostream &outStream);
		
	int numberOfDelayStates;
	int numberOfDelayTransitions;
		
	void segmentationFault();
	
	#ifdef PROGRESS_REPORT
	virtual void progressReport(ostream& reportStream);
	#endif
	
};

#define QUEUE_TYPE std::priority_queue<OpenBorderNode, std::vector<OpenBorderNode>, std::greater<OpenBorderNode> >

#endif