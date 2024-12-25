#ifndef __ABSTRACT_CORE_REBECA_MODEL_CHECKER_H_
#define __ABSTRACT_CORE_REBECA_MODEL_CHECKER_H_

#include "AbstractModelChecker.h"
#include "AbstractCoreRebecaAnalyzer.h"

#include <iostream>
#include <list>

#define STACK_OVERFLOW 10

class CoreRebecaModelChecker : public AbstractModelChecker, public AbstractCoreRebecaAnalyzer {

protected:
	using AbstractCoreRebecaAnalyzer::exportState;
	using AbstractCoreRebecaAnalyzer::exportTransition;
	
    virtual long getConsumedMemory();
    virtual void printCounterExample(int result, const AbstractActor* causedBy);

    virtual void printModelCheckingOptions();

	virtual void exportModelCheckingReportExtraDetails();
	
public:

	CoreRebecaModelChecker(std::ostream &out 
		#ifdef EXPORT_STATE_SPACE
		    , ostream &statespace
		#endif
		, int hashmapSize);
	virtual void doModelChecking(string userSelectedPropertyName, string userSelectedPropertyType);
	virtual void exportModelCheckingResult(string userSelectedPropertyName, 
		string userSelectedPropertyType, int result, string message, const AbstractActor* causedBy);





	virtual void doDFSModelChecking(string userSelectedPropertyName, string userSelectedPropertyType);
	int depth, maxReached;// depth in manual stack
	string getModelCheckingResultName(int id);
	bool incDepth();
	
	std::list<short int> getNextClaimState(short int claim, int propertySelector);
	boolean isAccepting(short int claim, int propertySelector);
		
	void segmentationFault();
};

#endif