#include "AbstractModelChecker.h"

#ifdef PROGRESS_REPORT
void AbstractModelChecker::progressReport(ostream& reportStream) {
    reportStream.imbue( std::locale( std::locale::classic(), new MyNumPunct ) );
    reportStream << numberOfStates
		<< " states and " << numberOfTransitions
    	<< " transitions" << endl;
}
#endif

AbstractModelChecker::AbstractModelChecker(std::ostream &outStream
		#ifdef EXPORT_STATE_SPACE
		    , ostream &statespaceStream
		#endif
	) : out(outStream)
	#ifdef EXPORT_STATE_SPACE
		, statespace(statespaceStream)
	#endif 
	{
}

void AbstractModelChecker::doModelChecking(string userSelectedPropertyName, string userSelectedPropertyType) {
    startTime = time(NULL);
    numberOfTransitions = 0;
    numberOfStates = 0;
}


string AbstractModelChecker::getModelCheckingResultName(int id) {
	return (
		id == SATISFIED ? "satisfied" :
        id == DEADLOCK ? "deadlock" :
        id == BOUND_REACHED ? "bound reached" :
        id == COUNTER_EXAMPLE ? "counter example" :
        id == QUEUE_OVERFLOW ? "queue overflow" :
        id == HEAP_OVERFLOW ? "state space explosion (heap overflow)" :
        id == ASSERTION_FAILED ? "assertion failed" :
        id == SEGMENTATION_FAULT ? "Runtime segmentation fault" :
        id == UNKNOWN_PROPERTY ? "Unknown Property Name" : "Unknown result" );
}

void AbstractModelChecker::exportModelCheckingDetails(string userSelectedPropertyName, 
	string userSelectedPropertyType, int result, string message) {
    
    out << "<system-info>" << std::endl;
    out << "\t<total-spent-time>" << (time(NULL) - startTime) << "</total-spent-time>" << std::endl;
    out << "\t<reached-states>" << numberOfStates << "</reached-states>" << std::endl;
    out << "\t<reached-transitions>" << numberOfTransitions << "</reached-transitions>" << std::endl;
    out << "\t<consumed-mem>" << getConsumedMemory() << "</consumed-mem>" << std::endl;
    out << "</system-info>" << std::endl;
    out << "<checked-property>" << std::endl;
    out << "\t<type>" << userSelectedPropertyType << "</type>" << std::endl;
    out << "\t<name>" << (userSelectedPropertyName.empty() ? "System default property" : userSelectedPropertyName) << "</name>" << std::endl;
    out << "\t<result>" << getModelCheckingResultName(result) << "</result>" << std::endl;
    if (!message.empty()) {
    	out << "\t<message>" << message << "</message>" << std::endl;
    }
    out << "<options>" << std::endl;
    #ifdef EXPORT_STATE_SPACE
        out << "\t<option>Export state space</option>" << std::endl;
    #endif
    #ifdef SAFE_MODE
        out << "\t<option>Safe mode is enabled</option>" << std::endl;
    #endif
    printModelCheckingOptions();
    out << "</options>" << std::endl;
    out  <<  "</checked-property>"  <<  std::endl;
}