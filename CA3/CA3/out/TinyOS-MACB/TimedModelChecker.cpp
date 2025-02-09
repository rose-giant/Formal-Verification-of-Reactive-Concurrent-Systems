#include "TimedModelChecker.h"
#include<limits>

#ifdef PROGRESS_REPORT

void TimedModelChecker::progressReport(ostream& reportStream) {
    AbstractModelChecker::numberOfStates = AbstractTimedRebecaAnalyzer::numberOfStates;
    AbstractModelChecker::numberOfTransitions = AbstractTimedRebecaAnalyzer::numberOfTransitions;
	AbstractModelChecker::progressReport(reportStream);
}
#endif

TimedModelChecker::TimedModelChecker(ostream &outStream
	#ifdef EXPORT_STATE_SPACE
	    , ostream &statespaceStream
	#endif
    , int hashmapSize) : AbstractModelChecker(outStream
	#ifdef EXPORT_STATE_SPACE
    	, statespaceStream
	#endif
	), 
	AbstractTimedRebecaAnalyzer(outStream, hashmapSize)
	{

	this->hashmapSize = hashmapSize;
	current.state = NULL;
}

string TimedModelChecker::getModelCheckingResultName(int id) {
	return (
		id == DEADLINE_MISSED ? "deadline missed" :
			AbstractModelChecker::getModelCheckingResultName(id)
		);
}

long TimedModelChecker::getConsumedMemory() {
    long totalMem = 0;
    for (int i=0; i<REBEC_COUNT; i++)
    	totalMem += rebecs[i]->consumedMemory();
    totalMem += sizeof(TimedBFSState) * (AbstractTimedRebecaAnalyzer::numberOfStates - AbstractTimedRebecaAnalyzer::numberOfTimedBundles);
    totalMem += sizeof(RebecsShiftTime) * AbstractTimedRebecaAnalyzer::numberOfTimedBundles;
    return totalMem;
}

void TimedModelChecker::printCounterExample(int result, AbstractActor* causedBy) {

	if(!current.state)
		return;

	string lastStateRebecName, lastStateAction, lastStateSender;
	TIME_TYPE lastStateTime;
	if (causedBy != null) {
	    lastStateRebecName = causedBy->getName();
	    lastStateAction = causedBy->activeAction();
	    lastStateSender = rebecs[causedBy->senderQueue[0]]->getName();
	    lastStateTime = rebecs[causedBy->senderQueue[0]]->getNow();
	}
	
	list<TimedBFSState*> counterExampleStates;
	TimedBFSState* cursor = current.state;
	while(cursor) {
		counterExampleStates.push_front(cursor);
		cursor = cursor->parents.front().parent;
	}
	int cnt = 1;
	TimedBFSState* parent = NULL;
	for (list<TimedBFSState*>::iterator it = counterExampleStates.begin(); it != counterExampleStates.end(); it++, cnt++) {

		(*it)->stateID = cnt;

		if (parent != NULL) {
			parent->stateID = cnt - 1;
			printCounterExampleTransition(parent, *it);
		}

		unmarshalRebecs((*it)->rebsIndex, 
    		(*it)->shiftTimes[(*it)->parents.front().timeBundleIndex].shift);
    	
    	OpenBorderNode child;
    	child.state = (*it);
		child.stateActiveBundleNumber = 0;

		exportState(child, out);    	
		
		parent = *it;
	}
	if (causedBy != null) {
	    string terminationStateName = "end";
		switch(result) {
			case ASSERTION_FAILED: terminationStateName = "assertion failed"; break; 
			case DEADLOCK: terminationStateName = "deadlock"; break; 
			case QUEUE_OVERFLOW: terminationStateName = "queue overflow"; break; 
			case DEADLINE_MISSED: terminationStateName = "deadline missed"; break; 
		}
		list<TimedBFSState*>::iterator last = counterExampleStates.end();
		last--;
		string sourceName = to_string((*last)->stateID) + "_0" ;
		exportTransition(sourceName, terminationStateName, lastStateSender, lastStateRebecName, lastStateAction, lastStateTime, 0, out);
		out << "<state id=\"" << terminationStateName << "\" atomicpropositions=\"" << terminationStateName << "\">" << endl << "</state>" << endl;
	}
	
}

void TimedModelChecker::exportModelCheckingResult(string userSelectedPropertyName, string userSelectedPropertyType,
	int result, string message, AbstractActor* causedBy) {

    AbstractModelChecker::numberOfStates = AbstractTimedRebecaAnalyzer::numberOfStates;
    AbstractModelChecker::numberOfTransitions = AbstractTimedRebecaAnalyzer::numberOfTransitions;

    out  <<  "<model-checking-report>"  <<  std::endl;

	exportModelCheckingDetails(userSelectedPropertyName, userSelectedPropertyType, result, message);
	exportModelCheckingReportExtraDetails();
    out  <<  "<counter-example-trace>"  <<  std::endl;
	if (result != HEAP_OVERFLOW && result != SATISFIED)
		printCounterExample(result, causedBy);
    out  <<  "</counter-example-trace>"  <<  std::endl;
    out  <<  "</model-checking-report>"  <<  std::endl;
}

void TimedModelChecker::doModelChecking(string userSelectedPropertyName, string userSelectedPropertyType) {
	vector<AbstractActor*> referenceToRebecs;
	try {
		AbstractModelChecker::doModelChecking(userSelectedPropertyName, userSelectedPropertyType);

	    bfsStorage = new TimedRebecaBFSHashmap(hashmapSize, out);
	    #ifdef DEBUG
	    	out << "State space storage is created successfully." << endl;
	    #endif

		instantiationPart();	    

		doTimedModelChecking(userSelectedPropertyName, userSelectedPropertyType);
	    exportModelCheckingResult(userSelectedPropertyName, userSelectedPropertyType,
	    	SATISFIED, "", null);
	    	
    } catch (AssertionFailedException &e) {
    	exportModelCheckingResult(userSelectedPropertyName, userSelectedPropertyType,
	    	ASSERTION_FAILED, e.getMessage(), e.getCausedBy());
    } catch (DeadlockException &e) {
    	exportModelCheckingResult(userSelectedPropertyName, userSelectedPropertyType,
	    	DEADLOCK, "", e.getCausedBy());
    } catch (QueueOverflowException &e) {
    	exportModelCheckingResult(userSelectedPropertyName, userSelectedPropertyType,
	    	QUEUE_OVERFLOW, "", e.getCausedBy());
    } catch (CounterExampleException &e) {
    	exportModelCheckingResult(userSelectedPropertyName, userSelectedPropertyType,
	    	COUNTER_EXAMPLE, "", null);
    } catch (UnknownPropertyException &e) {
    	exportModelCheckingResult(userSelectedPropertyName, userSelectedPropertyType,
	    	UNKNOWN_PROPERTY, e.getMessage(), null);
    } catch (DeadlineMissedException &e) {
    	exportModelCheckingResult(userSelectedPropertyName, userSelectedPropertyType,
	    	DEADLINE_MISSED, "", e.getCausedBy());
    } catch (...) {
    	exportModelCheckingResult(userSelectedPropertyName, userSelectedPropertyType,
	    	HEAP_OVERFLOW, "", null);
    }

    #ifdef EXPORT_STATE_SPACE
	    statespace << "</transitionsystem>" << endl;
	    statespace.flush();
	#endif

}

void TimedModelChecker::segmentationFault() {
	exportModelCheckingResult("", "", SEGMENTATION_FAULT, NULL, null);
	#ifdef EXPORT_STATE_SPACE
	    statespace << "</transitionsystem>" << endl;
	    statespace.flush();
	#endif
}






void TimedModelChecker::printCounterExampleTransition(TimedBFSState* parent, TimedBFSState* child) {

	OpenBorderNode parentNode, childNode;
	
	childNode.state = child;
	childNode.stateActiveBundleNumber = 0;
	
	parentNode.state = parent;
	parentNode.stateActiveBundleNumber = 0;
	
	if (child->parents.front().executedRebecIndex == -1) {
		TIME_TYPE diff = rebecs[0]->_ref_now;
		
		unmarshalRebecs(child->rebsIndex, 
    		child->shiftTimes[child->parents.front().timeBundleIndex].shift);
    	diff = rebecs[0]->_ref_now - diff;
    		
		exportProgressOfTimeTransition(parentNode, childNode, diff, rebecs[0]->getNow(), 0, out);
	} else {
		int rebecId = child->parents.front().executedRebecIndex;
		string label = "";
		if (rebecs[rebecId]->__pc != -1) {
			label += "tau=>";
		}
		label += rebecs[rebecId]->activeAction();
		string sender = rebecs[rebecs[rebecId]->senderQueue[0]]->getName();
		exportTransition(parentNode, childNode, sender, rebecs[rebecId]->getName(), label, rebecs[rebecId]->getNow(), 0, out);
	}
}

void TimedModelChecker::printModelCheckingOptions() {
    out << "\t<option>TTS</option>" << std::endl;
}

void TimedModelChecker::exportModelCheckingReportExtraDetails() {
	out << "<extra-details>" << endl;
	out << "\t<timed-states>" << numberOfDelayStates << "</timed-states>" << endl;
	out << "\t<timed-transitions>" << numberOfDelayTransitions << "</timed-transitions>" << endl;
	out << "</extra-details>" << endl;
}

void TimedModelChecker::storeInitialState() {

    byte result;
    TIME_TYPE shift;

	current = storeRecentlyCreatedState (result, shift, NULL, 0, 0);
	current.state->stateID = 1;

    #ifdef EXPORT_STATE_SPACE
	    statespace << "<transitionsystem>" << endl;
	    exportState(current, statespace);
    #endif

    #ifdef DEBUG
    	out << "The initial state is stored in the state space storage successfully." << endl;
    #endif

	numberOfDelayStates = 0;
	numberOfDelayTransitions = 0;

    current.currentTime = 0;
	
	//In this case, the first transition is a "progress of time" transition.
	int nextRebecTime = MAX_TIME;
	for (int r2 = 0; r2 < REBEC_COUNT; r2++) {
		if (rebecs[r2]->messageQueue[0]) {
	        TIME_TYPE rebecNextMessageTime = max(rebecs[r2]->getNow(), rebecs[r2]->executionTime[0]);
	        nextRebecTime = min(rebecNextMessageTime ,nextRebecTime);
	    }
	}
    if (nextRebecTime != 0) {
        for (int r2 = 0; r2 < REBEC_COUNT; r2++) {
            rebecs[r2]->setNow(nextRebecTime);
        }
    	OpenBorderNode newState2 = storeRecentlyCreatedState(result, shift, current.state,
    		current.stateActiveBundleNumber, 0);
        newState2.currentTime = nextRebecTime;

		#ifdef ENABLE_STATE_ID
		newState2.state->stateID = 2;
		#endif
		
	    #ifdef EXPORT_STATE_SPACE
	    exportState(newState2, statespace);
	    exportProgressOfTimeTransition(current, newState2, nextRebecTime, 0, 0, statespace);
	    #endif
		openBorderQueue.push(newState2);
		numberOfDelayTransitions++;
		numberOfDelayStates++;
    } else {
    	openBorderQueue.push(current);
    }
}

void TimedModelChecker::doTimedModelChecking(string userSelectedPropertyName, string userSelectedPropertyType) {
    byte result;
    TIME_TYPE shift;

	storeInitialState();
    
    #ifdef DEBUG
        out << "Before start state space generation \"while\" loop (The initial state is stored)." << endl;
    #endif
    while(!openBorderQueue.empty()) {

		TIME_TYPE currentTime = prepareWithNewState();
        int deadlock = true;
	    #ifdef DEBUG
	        out << "State is unmarshalled and try to find an enabled rebec." << endl;
	    #endif

	    int highestPriority = std::numeric_limits<int>::max();
        for (int r = 0; r < REBEC_COUNT; r++) {
        	if (rebecs[r]->__pc == -1) {
	            if (!rebecs[r]->messageQueue[0])
	                continue;
	            if ((max(rebecs[r]->getNow(), rebecs[r]->executionTime[0])) > currentTime) {
	                continue;
                }
                highestPriority = min(rebecs[r]->getPriority(), highestPriority);
        	} else {
        		if (rebecs[r]->__res != currentTime)
        			continue;
                highestPriority = min(rebecs[r]->getPriority(), highestPriority);
			}
		}
	    
        for (int r = 0; r < REBEC_COUNT; r++) {
        	TIME_TYPE executionTime;
            int numberOfAlternatives = 1;
            if (rebecs[r]->getPriority() != highestPriority)
            	continue;
        	if (rebecs[r]->__pc == -1) {
	            if (!rebecs[r]->messageQueue[0])
	                continue;
	            if ((max(rebecs[r]->getNow(), rebecs[r]->executionTime[0])) > currentTime) {
	                continue;
                }
                executionTime = (max(rebecs[r]->getNow(), rebecs[r]->executionTime[0]));
                numberOfAlternatives = getNumberOfAlternatives(r, executionTime);
        	} else {
        		if (rebecs[r]->__res != currentTime)
        			continue;
			}
            long nonDetTrans;
            for(int alternativesCounter = 0; alternativesCounter < numberOfAlternatives; alternativesCounter++) {
	            do {
					changeOrder(r, alternativesCounter);
					if (rebecs[r]->__pc == -1) {
					    if (rebecs[r]->deadline[0] < currentTime) {
					        throw DeadlineMissedException("Deadline missed", rebecs[r]);
					    }
					}
					nonDetTrans = executeRebec(r);
					unmarshalRebecs(current.state->rebsIndex, current.state->shiftTimes[current.stateActiveBundleNumber].shift);
	                currentTime = current.currentTime;
	            } while (nonDetTrans > 0);
            }
        }
    }
}

long TimedModelChecker::executeRebec(int rebecId) {
	long nonDetTrans;
    byte result;
    TIME_TYPE shift;

	#ifdef EXPORT_STATE_SPACE
	    char* actionName = rebecs[rebecId]->activeAction();
	    string prefix="";
		string sender = rebecs[rebecs[rebecId]->senderQueue[0]]->getName();
	#endif
	#ifdef DEBUG
	    out << "Enabled rebec is found. Try to excute message \"" << rebecs[rebecId]->getName() << "::" << rebecs[rebecId]->activeAction() << "\"."<< endl;
	#endif
	if (rebecs[rebecId]->__pc != -1) {
		#ifdef EXPORT_STATE_SPACE
			prefix = "tau=>";
		#endif
	}
	TIME_TYPE currentTime = rebecs[rebecId]->getNow();//(currentTime);
	
	try {
		nonDetTrans = rebecs[rebecId]->execute();
	} catch (QueueOverflowException &e) {
		e.setCausedBy(rebecs[rebecId]);
		throw e;
	}

	#ifdef DEBUG
	    out << "Message \"" << rebecs[rebecId]->getName() << "::" << rebecs[rebecId]->activeAction() << "\" is executed " 
	    	<< "and the return value is \"" << (long)nonDetTrans << "\"." << endl;
	#endif
	
	int nextRebecTime = MAX_TIME;
	for (int r2 = 0; r2 < REBEC_COUNT; r2++) {
		int index = (rebecId == r2 ? 1 : 0 );
		if (rebecs[r2]->__pc != -1) {
			nextRebecTime = min(rebecs[r2]->__res, nextRebecTime);
		} else if (rebecs[r2]->messageQueue[index]) {
	        TIME_TYPE rebecNextMessageTime = max(rebecs[r2]->getNow(), rebecs[r2]->executionTime[index]);
	        nextRebecTime = min(rebecNextMessageTime ,nextRebecTime);
	    }
	}
	
	OpenBorderNode newState = storeRecentlyCreatedState(result, shift, current.state, current.stateActiveBundleNumber, rebecId);
    try {
    } catch (AssertionFailedException &e) {
    	e.setCausedBy(rebecs[rebecId]);
    	throw;
	}
	
	if (nextRebecTime == MAX_TIME) {
		current = newState;
	    throw DeadlockException("Deadlock");
	}
	
	if (result & NEW_STATE) {
	    newState.currentTime = nextRebecTime;
	
	    #ifdef ENABLE_STATE_ID
	        newState.state->stateID = AbstractTimedRebecaAnalyzer::numberOfStates;
		#endif
		
	    #ifdef EXPORT_STATE_SPACE
		    exportState(newState, statespace);
	    #endif
	
	    if (currentTime != nextRebecTime) {
	        for (int r2 = 0; r2 < REBEC_COUNT; r2++) {
	            rebecs[r2]->setNow(nextRebecTime);
	        }
	        TIME_TYPE shift2 = 0;
	    	OpenBorderNode newState2 = storeRecentlyCreatedState(result, shift2, newState.state, current.stateActiveBundleNumber, -1);
	    	if (result & NEW_STATE) {
	    		numberOfDelayStates++;
	            newState2.currentTime = nextRebecTime;
	
			    #ifdef ENABLE_STATE_ID
			    newState2.state->stateID = AbstractTimedRebecaAnalyzer::numberOfStates;
			    #endif
			    
			    #ifdef EXPORT_STATE_SPACE
			    exportState(newState2, statespace);
			    #endif
			    
	    		openBorderQueue.push(newState2);
	    	}
		    #ifdef EXPORT_STATE_SPACE
		    exportProgressOfTimeTransition(newState, newState2, (nextRebecTime - currentTime), currentTime, shift2, statespace);
		    #endif
		    numberOfDelayTransitions++;
	    } else {
	    	openBorderQueue.push(newState);
	    }
	}
	#ifdef EXPORT_STATE_SPACE
	string label = prefix;
	label += actionName;
	exportTransition(current, newState, sender, rebecs[rebecId]->getName(), label, currentTime, shift, statespace);
	#endif
	return nonDetTrans;
}


void TimedModelChecker::exportProgressOfTimeTransition(OpenBorderNode &source,
	OpenBorderNode &destination, TIME_TYPE time, TIME_TYPE executionTime, TIME_TYPE shift, ostream &outStream) {
    
    outStream << "<transition source=\"" << source.state->stateID << "_" << 
		(int)source.stateActiveBundleNumber << 
    	"\" destination=\"" << destination.state->stateID << "_" << 
    	(int)destination.stateActiveBundleNumber <<
    	"\" executionTime=\"" << (int)executionTime << "\" shift=\"" << (int) shift <<
		"\"> <time value=\"" << (int)time << "\"/></transition>" << endl;
}
