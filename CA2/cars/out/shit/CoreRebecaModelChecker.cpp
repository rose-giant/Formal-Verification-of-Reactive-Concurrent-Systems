#include "CoreRebecaModelChecker.h"

CoreRebecaModelChecker::CoreRebecaModelChecker(ostream &outStream
	#ifdef EXPORT_STATE_SPACE
	    , ostream &statespaceStream
	#endif
    , int hashmapSize) : AbstractModelChecker(outStream
	#ifdef EXPORT_STATE_SPACE
    	, statespaceStream
	#endif
	), 
	AbstractCoreRebecaAnalyzer(outStream, hashmapSize)
	{

	this->hashmapSize = hashmapSize;
	depth = -1;
}

long CoreRebecaModelChecker::getConsumedMemory() {
    long totalMem = 0;
    for (int i=0; i<REBEC_COUNT; i++)
    	totalMem += rebecs[i]->consumedMemory();
    totalMem += sizeof(State) * AbstractModelChecker::numberOfStates;
    return totalMem;
}

void CoreRebecaModelChecker::exportModelCheckingResult(string userSelectedPropertyName, string userSelectedPropertyType,
	int result, string message, const AbstractActor* causedBy) {

	AbstractModelChecker::numberOfTransitions = AbstractCoreRebecaAnalyzer::numberOfTransitions;
	AbstractModelChecker::numberOfStates = AbstractCoreRebecaAnalyzer::numberOfStates;
    out  <<  "<model-checking-report>"  <<  std::endl;

	exportModelCheckingDetails(userSelectedPropertyName, userSelectedPropertyType, result, message);
	exportModelCheckingReportExtraDetails();
    out  <<  "<counter-example-trace>"  <<  std::endl;
	if (result != SATISFIED && result != HEAP_OVERFLOW && result != STACK_OVERFLOW && result != UNKNOWN_PROPERTY)
		printCounterExample(result, causedBy);
    out  <<  "</counter-example-trace>"  <<  std::endl;
	
	
    out  <<  "</model-checking-report>"  <<  std::endl;
}

void CoreRebecaModelChecker::doModelChecking(string userSelectedPropertyName, string userSelectedPropertyType) {
	vector<AbstractActor*> referenceToRebecs;
	try {
		AbstractModelChecker::doModelChecking(userSelectedPropertyName, userSelectedPropertyType);

	    storage = new CoreRebecaDFSHashmap(hashmapSize, out);
	    #ifdef DEBUG
	    	out << "State space storage is created successfully." << endl;
	    #endif

		instantiationPart();	    

		doDFSModelChecking(userSelectedPropertyName, userSelectedPropertyType);
		
	    exportModelCheckingResult(userSelectedPropertyName, userSelectedPropertyType,
	    	SATISFIED, "", null);
	    	
    } catch (AssertionFailedException &e) {
    	exportModelCheckingResult(userSelectedPropertyName, userSelectedPropertyType,
	    	ASSERTION_FAILED, e.getMessage(), e.getCausedBy());
    } catch (DeadlockException &e) {
    	exportModelCheckingResult(userSelectedPropertyName, userSelectedPropertyType,
	    	DEADLOCK, "", null);
    } catch (QueueOverflowException &e) {
    	exportModelCheckingResult(userSelectedPropertyName, userSelectedPropertyType,
	    	QUEUE_OVERFLOW, e.getMessage(), e.getCausedBy());
    } catch (StackOverflowException &e) {
    	exportModelCheckingResult(userSelectedPropertyName, userSelectedPropertyType,
	    	STACK_OVERFLOW, "", null);
    } catch (CounterExampleException &e) {
    	exportModelCheckingResult(userSelectedPropertyName, userSelectedPropertyType,
	    	COUNTER_EXAMPLE, "", null);
    } catch (UnknownPropertyException &e) {
    	exportModelCheckingResult(userSelectedPropertyName, userSelectedPropertyType,
	    	UNKNOWN_PROPERTY, e.getMessage(), null);
    } catch (...) {
    	exportModelCheckingResult(userSelectedPropertyName, userSelectedPropertyType,
	    	HEAP_OVERFLOW, "", null);
    }

    #ifdef EXPORT_STATE_SPACE
	    statespace << "</transitionsystem>" << endl;
	    statespace.flush();
	#endif

}

void CoreRebecaModelChecker::segmentationFault() {
	exportModelCheckingResult("", "", SEGMENTATION_FAULT, NULL, null);
	#ifdef EXPORT_STATE_SPACE
	    statespace << "</transitionsystem>" << endl;
	    statespace.flush();
	#endif
}





#define UNDEFINED 0
	extern int maxDepth;

// the PUSH macro is used in BFS, therefore assigning -1 to rebec & flag
#ifdef DEBUG

	#define PUSH(d, newState) \
		localStack[d].state = newState;	\
		localStack[d].rebec = (unsigned char)-1; \
		localStack[d].flag = false; \
		localStack[d].undoMark = undoMark; \
		localStack[d].fc =  localStack[d-1].fc;
#else
	#define PUSH(d, newState) \
		localStack[d].state = newState;	\
		localStack[d].rebec = (unsigned char)-1; \
		localStack[d].flag = false; \
		localStack[d].undoMark = fairnessCounter;
#endif

#define PRINT_STACK(d) \
	cout<< "..........." << endl;for(int sti = d; sti != 0; sti--) {cout << localStack[sti].state->stateID << "," << localStack[sti].state->claim << "," << localStack[sti].state->isOnStack() << endl;}cout<< "..........." << endl;

struct StackEl {
	State *state;	// a pointer to the real state stored in HashMap
	byte rebec;		// ID of the last rebec executed at this state at current depth
	bool flag;		// used in fairness algorithm
	byte undoMark;	// used in fairness algorithm
	#ifdef DEBUG
		byte fc;	// fairness counter
	#endif
}*localStack;


bool CoreRebecaModelChecker::incDepth() {
	static boolean warned = false;
	if (depth >= maxDepth-1) {
		if (!warned) {
			// warn only once about stack overflow
			// printf ("Max search depth too small : %d. (Stack overflow)\n", maxDepth);
			warned = true;
		}
		return false;
	}
	else {
		depth++;	// move one level down in the DFS stack
		if (maxReached <= depth) maxReached = depth+1;	// "depth" is zero-based
	}
	return true;
}

	#define _ref_finished1 _propDef_finished1()
	#define _ref_isSafetyViolated _propDef_isSafetyViolated()

std::list<short int> CoreRebecaModelChecker::getNextClaimState(short int claim, int propertySelector) {
	std::list<short int> result;
	switch (propertySelector) {
		case 1:
			switch(claim) {
				case 0:
							if (!_ref_finished1) result.push_back(0);
					break;
			}
		break;
		case 2:
			switch(claim) {
				case 1:
							result.push_back(1);
					break;
				case 0:
							result.push_back(0);
							if (_ref_isSafetyViolated) result.push_back(1);
					break;
			}
		break;
	}
	if (result.empty()) result.push_back(-1);	
	return result;
}

boolean CoreRebecaModelChecker::isAccepting(short int claim, int propertySelector) {
	switch (propertySelector) {
		case 1:
			switch(claim) {
					case 0:
						return true;
			}
		break;
		case 2:
			switch(claim) {
					case 1:
						return true;
					case 0:
						return false;
			}
		break;
	default:
		return false;
	}
	return false;
}

	#undef _ref_finished1
	#undef _ref_isSafetyViolated

void CoreRebecaModelChecker::doDFSModelChecking(string userSelectedPropertyName, string userSelectedPropertyType) {
	int propertySelector;
	short int claim;
	localStack = new StackEl[maxDepth];
	std::list<short int>claims;

	byte fairnessCounter;	// for fairness algorithm
	byte undoMark;			// undo mark for fairnessCounter
	int	seedDepth;			// depth at which 2nd DFS is started - used to detect when 2nd DFS finishes
	int effectiveDepth;		// in BFS: we need to keep depth of the real state being explored
	char r;					// rebec counter for systemDfs -> must be signed
	bool flag;				// fairness undo number and mark
	bool newStateFlag;		// for claim BFS
	byte result;			// result of trying to store a new state
	boolean qOverflow = false;

	long nonDetTrans = 0;		// in nondeterminism - BFS mark for system
    long claimNonDetTrans;		// BFS mark for claim
 	boolean deadlock=false;		// Are we facing a deadlock?
	boolean deadlockEnable=true;
	
 	seedDepth = -1;			// initially we are not in 2nd DFS
    fairnessCounter = REBEC_COUNT + 1;	// start with the 1st DFS
 	maxReached = 0;
	depth = -1;	// in symmetry, depth+1 is used in store()

	if (userSelectedPropertyName.empty())
		propertySelector = 0;
		else if (userSelectedPropertyName == "Starvation1") {
			propertySelector = 1;
				claim = 0;
		}
		else if (userSelectedPropertyName == "Safety") {
			propertySelector = 2;
				claim = 0;
		}
	else {
		string message = "Unknown property name \"";
		message += userSelectedPropertyName;
		message += "\"";
		throw UnknownPropertyException(message);
	}
 	// store initial state of system
 	localStack[0].state = storeRecentlyCreatedState (result, claim, fairnessCounter);
    #ifdef EXPORT_STATE_SPACE
	    statespace << "<transitionsystem>" << endl;
	    exportState(localStack[0].state, statespace);
    #endif
 	
 	#ifdef DEBUG
 		localStack[0].fc = fairnessCounter;
 	#endif
	depth = 0;
    #ifdef EXPORT_STATE_SPACE
	    string label, sender;
    #endif


 systemTurn:	// SystemDfs(): called from within ClaimDfs(), except for the first time which is called from main()
 	if (depth == maxDepth - 1){
 		//printf ("Bounded search reached its depth limit: %d.\n", maxDepth);
        throw StackOverflowException("search stack bound is reached.");
 	}

 	deadlock = deadlockEnable;	// choose whether to detect deadlocks
    flag = false;
 	undoMark = fairnessCounter;
 	// Make sure rebecs are unmarshall properly if "for" and "while" below iterate!
 	unmarshalRebecs(localStack[depth].state->rebecs);	// unmarshal is needed for checking disabled rebecs - also needed because of symmetry detection!
 	//check if disabled rebecs can contribute to fairnessCounter
    while(fairnessCounter < REBEC_COUNT && !rebecs[fairnessCounter]->messageQueue[0]) fairnessCounter++;    
     for (r = 0; r < REBEC_COUNT; r++) {	// try to execute all rebecs in the system (interleaving)
 		if (!rebecs[r]->messageQueue[0] && deadlockEnable) continue;	// if disabled continue with next rebec
 		// fairness algorithm: check whether to move to the next copy of state-space (only in 2nd DFS)
            if (fairnessCounter == r)
		{
			undoMark = fairnessCounter;
			fairnessCounter++;
 			flag = true;
 			//check if disabled rebecs can contribute to fairnessCounter
                while(fairnessCounter < REBEC_COUNT && !rebecs[fairnessCounter]->messageQueue[0]) fairnessCounter++;
         }

 		effectiveDepth = depth;
 		do {	// explore all non-det transitions in system in BFS manner
 			// read the system state which is to be explored (useful when back-tracking and in non_det BFS)
 			qOverflow = false;
		    #ifdef EXPORT_STATE_SPACE
			    label = rebecs[r]->messageNames[rebecs[r]->messageQueue[0]];
			    sender = rebecs[rebecs[r]->senderQueue[0]]->getName();
		    #endif
			try {
            	nonDetTrans = rebecs[r]->execute();	// The parameter should be removed actually I believe!
            } catch (QueueOverflowException &e) {
    			e.setCausedBy(rebecs[r]);
    			throw e;
    		}
 			/*
 			The next system state is returned in "sys"
 			Return value (nonDetTrans):
 				-2	 : some local hashTable is full.
 				-1   : current rebec was disabled
 				0    : there was no more non-det transitions
 				other: there are other (non_det) transistions
 			*/
			if (qOverflow) {	// set as an external variable when there is queue-overflow
 				if (nonDetTrans == -2) {
					out << "Reactive class local hashtable is full" << endl;
					exit (0);
 				}
				throw QueueOverflowException("A rebec queue is overflowed.");
			}
 			deadlock = false;	// if some rebec has some enabled transition, then there is no deadlock
 			// we are going to call another method, so we need to store our own local variables onto stack
 			localStack[depth].flag = flag;
 			localStack[depth].undoMark = undoMark;
 			localStack[depth].rebec = (effectiveDepth == depth)? r : -1;	// we need to know where to resume when we're back!
 			// if we are here, deadlock is false, so no need to store it

 			// claimDfs(sNext);
 			goto claimTurn;
 Back2System:
 			if (nonDetTrans <= 0) {	// if no BFS, i.e., if back tracking
 				// POP from stack
 				deadlock = false;	// useful when "deadlockEnable" is false
 				flag = localStack[depth].flag;
 				undoMark = localStack[depth].undoMark;
 				r = localStack[depth].rebec;
 				while((r == -1) && localStack[depth].state->isVisited(fairnessCounter)) {
 					// r == -1 means it was a BFS state
 					// If it is visited later, just skip it here (it is already explored)
 					/* If POR: this state was visited at a bigger depth, so if stack proviso
 					   was not violated, it won't be here, either. Anyway, POR itself must
 					   handle it somehow, if the same state is reached later with a different
 					   stack!
 					*/
 					depth--;
 					flag = localStack[depth].flag;
 					undoMark = localStack[depth].undoMark;
 					r = localStack[depth].rebec;
 				}
 			}
 			unmarshalRebecs(localStack[effectiveDepth].state->rebecs);	// make ready for next non_det execution
 		} while (nonDetTrans>0);

 		if (r == -1) {	// unexplored state - caused by non_det BFS
 			// explore it now - first visit, then if in 1st-DFS put it on stack
 			localStack[depth].state->visit(fairnessCounter);
 			if (fairnessCounter > REBEC_COUNT)
 				localStack[depth].state->setOnStack();
	 		goto systemTurn;	// continue with current DFS
 		} else {
 			if (flag && nonDetTrans != -1 &&
                undoMark == r )
				/*We must undo, only if fairnessCounter is incremented and current rebec was not disabled*/
 				fairnessCounter = undoMark;
 		}
 		unmarshalRebecs(localStack[depth].state->rebecs);	// make ready for next rebec
    }	// end of loop over processes
 	nonDetTrans = 0;	// when backtracking, it may be -1
 	if (deadlock /*&& fairnessCounter > REBEC_COUNT*/){
    	throw DeadlockException("Deadlock occured.", rebecs[r]);
 	}
 	if (depth <= 0) {
 		// Search complete
 		#ifdef DEBUG
 		if (depth < 0) cout << "<faral-error>negative depth = " << depth << "</faral-error>" << endl;
 		#endif
 		return;
 	}
 	goto Back2Claim;	// SystemDfs() is called from within ClaimDfs, so we return to there
 claimTurn:
 	newStateFlag = false;
	// Do Rebecs still have their unmarshaled state??? YES, because of BFS
	claims = getNextClaimState(localStack[effectiveDepth].state->claim, propertySelector);	// we may use recently executed rebec

	/* The next state in the product automaton is reached when both system and claim move
	one state forward. So we can store() a new state here.
	*/

	for (std::list<short int>::iterator it = claims.begin(); it != claims.end(); ++it) {
	
 		State *newState;
 		newState = storeRecentlyCreatedState (result, *it, fairnessCounter);
	    try {
	    } catch (AssertionFailedException &e) {
	    	e.setCausedBy(rebecs[r]);
	    	throw;
		}


 		if (result & NEW_STATE) {	// bitwise AND
		    #ifdef EXPORT_STATE_SPACE
		    	exportState(newState, statespace);
			    exportTransition(localStack[effectiveDepth].state, newState, sender, rebecs[r]->getName(), label, statespace);
		    #endif
 			// Move one step deeper in the stack
 			if (!incDepth()) break;
 			newStateFlag = true;
 			PUSH (depth, newState);
 			/*
 			  The PUSH macro sets "rebec" and "flag" elements of stack to -1.
 			  It causes a full expansion of that state if we have to goto Back2System. (useful for BFS in claim)
 			  The last state is however simply expanded by goto SystemTurn.
 			*/
	    #ifdef EXPORT_STATE_SPACE
 		} else {
		    exportTransition(localStack[effectiveDepth].state, newState, sender, rebecs[r]->getName(), label, statespace);
		#endif
 		}
 		if (/*equals(localStack[seedDepth].state, sys, claim)*/ (result & ON_STACK) && fairnessCounter == REBEC_COUNT) {
 			// If the stack of 1st DFS is cut, a cycle is definitly closed.
 			/*-->implies && seedDepth != -1*/
 			// implied: in 2nd DFS
 			// If we are at the copy "REBEC_COUNT" of state-space, then the cycle is fair.
	        throw CounterExampleException("Counter example if detected.");	// finished searching
 		}
 	}

     if (newStateFlag && !nonDetTrans) {

         localStack[depth].state->visit(fairnessCounter);
         if (fairnessCounter > REBEC_COUNT)
             localStack[depth].state->setOnStack();
 		#ifdef DEBUG
 			localStack[depth].fc = fairnessCounter;
 		#endif

 		goto systemTurn;	// continue with current DFS
             //systemDfs(sNext);
 Back2Claim:
 		if (depth == seedDepth){	// returning from the recursive call below
             fairnessCounter = REBEC_COUNT+1;	//back from 2nd DFS
 			seedDepth = -1;
 		}
 		else {	// returning from the above recursive call
 			// After the 1st DFS exploration of current rebec is finished, if it is accepting, we
 			// start the 2nd DFS from this state. This post-order behavior allows us to keep the
 			// state-space of 2nd DFS between consecutive calls.
 			if (/*in 1st DFS?*/fairnessCounter > REBEC_COUNT && isAccepting(localStack[depth].state->claim, propertySelector)) {
 				seedDepth = depth;
 				fairnessCounter = 0;	//enter 2nd DFS
 				goto systemTurn;	// start 2nd DFS
 			}
 		}
 		// if we just want to start 2nd DFS, we have to keep the ON_STACK flag of the last state (seed)
 		if (fairnessCounter > REBEC_COUNT)	// we only keep track of 1st DFS stack
 			localStack[depth].state->clearOnStack();
 		depth--;
     }

 	goto Back2System;
}

void CoreRebecaModelChecker::printCounterExample(int result, const AbstractActor* causedBy) {
    int loopID = -1;
    int counter = 0;

    if(depth == -1)
    	return;

	unmarshalRebecs(localStack[0].state->rebecs);
	exportState(localStack[0].state, out);
	short int rebecId = localStack[0].rebec;
	short int parent = 0;
	
    for (int cnt = 1; cnt <= depth; cnt++) {
        // non_det BFS state(not visited in DFS)
        if (localStack[cnt].rebec == (byte)-1 && cnt != depth)
            continue;
        //if (localStack[cnt].state->equals(sys, claim))
        //    loopID = counter;
		
        string rebecName = rebecs[rebecId]->getName();
        string action = rebecs[rebecId]->activeAction();
        string sender = rebecs[rebecs[rebecId]->senderQueue[0]]->getName();
		unmarshalRebecs(localStack[cnt].state->rebecs);
		exportTransition(localStack[parent].state, localStack[cnt].state, sender, rebecName, action, out);
		exportState(localStack[cnt].state, out);
		rebecId = localStack[cnt].rebec;
        parent = cnt;
	}
	
	if (causedBy != null) {
	    string rebecName = causedBy->getName();
	    string action = causedBy->activeAction();
	    string sender = rebecs[causedBy->senderQueue[0]]->getName();
	    string terminationStateName = "end";
		switch(result) {
			case ASSERTION_FAILED: terminationStateName = "assertion failed"; break; 
			case DEADLOCK: terminationStateName = "deadlock"; break; 
			case QUEUE_OVERFLOW: terminationStateName = "queue overflow"; break; 
		}
		exportTransition(to_string(localStack[parent].state->stateID), terminationStateName, sender, rebecName, action, out);
		out << "<state id=\"" << terminationStateName << "\" atomicpropositions=\"" << terminationStateName << "\">" << endl << "</state>" << endl;
	}
}


void CoreRebecaModelChecker::printModelCheckingOptions() {
    out << "\t<option>Nested DFS</option>" << std::endl;
}

void CoreRebecaModelChecker::exportModelCheckingReportExtraDetails() {
}

string CoreRebecaModelChecker::getModelCheckingResultName(int id) {
	return (
		id == STACK_OVERFLOW ? "search stack overflow" :
			AbstractModelChecker::getModelCheckingResultName(id)
		);
}



