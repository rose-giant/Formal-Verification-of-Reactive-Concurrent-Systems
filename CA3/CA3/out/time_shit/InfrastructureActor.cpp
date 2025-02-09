                                    


#include "Types.h"
#include "InfrastructureActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

#include "ControllerActor.h"
#include "AutoActor.h"
#include "MannedActor.h"

// location counters (values stored in messageQueue[0])
/* There can be more than one location counter for each msgsrv when we
   want to have non-atomic message servers.
   Notice that values 0 and 1 are reserved for EMPTY and INITIAL, respectively.*/
#define ISALLOWED 1
#define CANMOVE 2


// knownrebecs (those to whom message can be sent)
// the following format allows to use them as variables --> the ID of the rebec
#define _ref_sender (rebecs[senderQueue[0]])
#define _ref_self (dynamic_cast<InfrastructureActor*>(rebecs[(int)((unsigned char)myID)]))

    	#define _ref_ctrl (dynamic_cast<ControllerActor*>(rebecs[(int)((unsigned char)known[1])]))
    
InfrastructureActor::InfrastructureActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize














)
	:
		AbstractTimedActor (myID, myName, maxQueueLength, maxParamLength, rebecsRef
			#ifdef DEBUG_LEVEL_2 
				, out
			#endif







		, stateSize, hashmapSize














		)
	, AbstractActor (myID, myName, maxQueueLength, maxParamLength, rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, out
		#endif







		, stateSize, hashmapSize














	)
{
    messageNames = new char *[2 + 1];
    messagePriorities = new int[2 + 1];
	messageNames[0] = (char*)"Empty";

	
		
	
		
       		 messageNames[1] = (char*)"ISALLOWED";
			messagePriorities[1] = std::numeric_limits<int>::max();
       		 messageNames[2] = (char*)"CANMOVE";
			messagePriorities[2] = std::numeric_limits<int>::max();








	if (table == (byte**)0xFFFFFFFF) {	// avoid reinitialization
		table = new byte* [1 << hashmapSize];
        memset (table, 0, sizeof(byte*)*(1 << hashmapSize));
	}








	if (hashtableTimeExtension == (vector<TimeFrame>**)0xFFFFFFFF) {	// avoid reinitialization
        hashtableTimeExtension = new vector<TimeFrame>*[1 << hashmapSize];
        memset (hashtableTimeExtension, 0, sizeof(vector<TimeFrame>*)*(1 << hashmapSize));
	}







	
	
	queueTail = 0;
}

InfrastructureActor::~InfrastructureActor (){
	delete [] messageNames;
	delete [] messageQueue;
	delete [] senderQueue;
}


void InfrastructureActor::exportAMessageInXML(ostream &out, string tab, int cnt) {
        int i = 0;
        switch (messageQueue[cnt]) {
            case ISALLOWED: {

            	boolean _ref_line;
            
            

            	int _ref_block;
            
            
                	memcpy(&_ref_block, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_id;
            
            
                	memcpy(&_ref_id, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	boolean _ref_isAuto;
            
            
_ref_line = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[0]);
_ref_isAuto = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[1]);
++i;                out << "isAllowed("
                	<< (_ref_line? "true" : "false")
					<< ", "
                	<< ((int)_ref_block)
					<< ", "
                	<< ((int)_ref_id)
					<< ", "
                	<< (_ref_isAuto? "true" : "false")
                << ")";
                break;
               }
            case CANMOVE: {

            	boolean _ref_canMove;
            
            

            	int _ref_id;
            
            
                	memcpy(&_ref_id, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	boolean _ref_isAuto;
            
            
_ref_canMove = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[0]);
_ref_isAuto = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[1]);
++i;                out << "canMove("
                	<< (_ref_canMove? "true" : "false")
					<< ", "
                	<< ((int)_ref_id)
					<< ", "
                	<< (_ref_isAuto? "true" : "false")
                << ")";
                break;
               }
    	}
}


void InfrastructureActor::exportStateVariablesInXML(ostream &out, string tab) {
	out << tab << "<statevariables>" << endl;
	exportInsideStateVariablesTagInXML(out, tab);
	out << tab << "</statevariables>" << endl;
}

void InfrastructureActor::exportInsideStateVariablesTagInXML(ostream &out, string tab){


			    	out << tab << '\t' << "<variable name=\"Infrastructure.autoCars\" type=\"Auto[10]\">" <<  
			    		"[" << (_ref_autoCars[0] == NULL ? "NULL" : _ref_autoCars[0]->getName()) << ", " << (_ref_autoCars[1] == NULL ? "NULL" : _ref_autoCars[1]->getName()) << ", " << (_ref_autoCars[2] == NULL ? "NULL" : _ref_autoCars[2]->getName()) << ", " << (_ref_autoCars[3] == NULL ? "NULL" : _ref_autoCars[3]->getName()) << ", " << (_ref_autoCars[4] == NULL ? "NULL" : _ref_autoCars[4]->getName()) << ", " << (_ref_autoCars[5] == NULL ? "NULL" : _ref_autoCars[5]->getName()) << ", " << (_ref_autoCars[6] == NULL ? "NULL" : _ref_autoCars[6]->getName()) << ", " << (_ref_autoCars[7] == NULL ? "NULL" : _ref_autoCars[7]->getName()) << ", " << (_ref_autoCars[8] == NULL ? "NULL" : _ref_autoCars[8]->getName()) << ", " << (_ref_autoCars[9] == NULL ? "NULL" : _ref_autoCars[9]->getName()) << ", " << "]" << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Infrastructure.mannedCars\" type=\"Manned[10]\">" <<  
			    		"[" << (_ref_mannedCars[0] == NULL ? "NULL" : _ref_mannedCars[0]->getName()) << ", " << (_ref_mannedCars[1] == NULL ? "NULL" : _ref_mannedCars[1]->getName()) << ", " << (_ref_mannedCars[2] == NULL ? "NULL" : _ref_mannedCars[2]->getName()) << ", " << (_ref_mannedCars[3] == NULL ? "NULL" : _ref_mannedCars[3]->getName()) << ", " << (_ref_mannedCars[4] == NULL ? "NULL" : _ref_mannedCars[4]->getName()) << ", " << (_ref_mannedCars[5] == NULL ? "NULL" : _ref_mannedCars[5]->getName()) << ", " << (_ref_mannedCars[6] == NULL ? "NULL" : _ref_mannedCars[6]->getName()) << ", " << (_ref_mannedCars[7] == NULL ? "NULL" : _ref_mannedCars[7]->getName()) << ", " << (_ref_mannedCars[8] == NULL ? "NULL" : _ref_mannedCars[8]->getName()) << ", " << (_ref_mannedCars[9] == NULL ? "NULL" : _ref_mannedCars[9]->getName()) << ", " << "]" << "</variable>" << endl;
}


char* InfrastructureActor::getClassName() {
    return (char*)"Infrastructure";
}
void InfrastructureActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    	
            
					memcpy(&array [marshalIndex], _ref_autoCars.data(), (1 * REACTIVE_CLASS_SIZE) * 10);					
					marshalIndex += (1 * REACTIVE_CLASS_SIZE) * 10;
            
					memcpy(&array [marshalIndex], _ref_mannedCars.data(), (1 * REACTIVE_CLASS_SIZE) * 10);					
					marshalIndex += (1 * REACTIVE_CLASS_SIZE) * 10;

    
	
}
void InfrastructureActor::marshalActorToArray(byte* array) {
    int i,j, marshalIndex = 0;

	for (i = (shift ? 1 : 0); i < maxQueueLength; i++) {
        array[marshalIndex++] = messageQueue[i];
    }

    if (shift) {
        array[marshalIndex++] = 0;
    }
    
	for (i = (shift ? 1 : 0); i < maxQueueLength; i++) {
        array[marshalIndex++] = senderQueue[i];
    }

    if (shift) {
        array[marshalIndex++] = 0;
    }

	if(maxParamLength != 0) {
        for (i = (shift ? 1 : 0); i < maxQueueLength; i++) {
            for (j = 0; j < maxParamLength; j++) {
                array[marshalIndex++] = paramQueue[i][j];
            }
        }
        if (shift) {
            for (j = 0; j < maxParamLength; j++) {
                array[marshalIndex++] = 0;
            }
        }
    }

	 #ifdef NO_ATOMIC_EXECUTION
    	memcpy(&array [marshalIndex], &__pc, 4);
    	marshalIndex += 4;
    	//array[marshalIndex++] = __pc;
    #endif

		marshalVariablesToArray( & array[marshalIndex]);

   

}

void InfrastructureActor::unmarshalActorFromArray(byte* array) {

    int index = 0, i, j;
    shift = 0;

	for (i = 0; i < maxQueueLength; i++) {
        messageQueue[i] = array[index++];
    }
	
	for (i = 0; i < maxQueueLength; i++) {
        senderQueue[i] = array[index++];
    }
    
	if(maxParamLength != 0) {
        for (i = 0; i < maxQueueLength; i++) {
            for (j = 0; j < maxParamLength; j++) {
                paramQueue[i][j] = array[index++];
            }
        }
    }
    
	#ifdef NO_ATOMIC_EXECUTION
    	memcpy(&__pc, &array [index], 4);
    	index += 4;
    	//__pc = array[index++];
    #endif
    
	unmarshalVariablesFromArray( & array[index]);
}

void InfrastructureActor::unmarshalVariablesFromArray(byte * array){

	int index = 0;

            
            
					memcpy(_ref_autoCars.data(), &array [index], (1 * REACTIVE_CLASS_SIZE) * 10);					
					index += (1 * REACTIVE_CLASS_SIZE) * 10;

            
            
					memcpy(_ref_mannedCars.data(), &array [index], (1 * REACTIVE_CLASS_SIZE) * 10);					
					index += (1 * REACTIVE_CLASS_SIZE) * 10;
    
	
}

/*#ifdef DEBUG
void InfrastructureActor::printStateContent(long stateNo) {
	out << myName << "=>[";
	byte* currentValues = getTable()[stateNo];
	for (int cnt = 0; cnt < STATE_SIZE; cnt++)
		out << (int)currentValues[cnt] << ", ";
	out << "]";
}
#endif
*/

//---------------------------------------------------------
/**
  Checks which action of this rebec is enabled, and executes it. Actions are the
  same as the location-counter defined at the beginning of this file.
  In the case of non-atomic message servers, a location-counter may point to the
  middle of a message server.
  If this action leads to more than one transition (non_det), this method executes
  only one of these transitions. With the help of some static variables, consecutive
  calls to this method execute different transitions caused by this action.
  Local variables must be properly valuated before any call to this method (see marshal()).

  returns: >0 : there are more possible next states (non-det transitions).
		   =0 : no more non_det transitions
		   -1 : no enabled action (empty queue)
		   -2 : Local hash table is full - immediate termination of search
 */
long InfrastructureActor::methodSelectorAndExecutor() {

	
    switch (messageQueue[0]) {

            case ISALLOWED:
                {
                    int i = 0;

            	boolean _ref_line;
            
            

            	int _ref_block;
            
            
                	memcpy(&_ref_block, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_id;
            
            
                	memcpy(&_ref_id, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	boolean _ref_isAuto;
            
            
_ref_line = (bool) (paramQueue[0] [i] & AbstractActor::coeff[0]);
_ref_isAuto = (bool) (paramQueue[0] [i] & AbstractActor::coeff[1]);
++i;                    return msgsrvisAllowed(
            _ref_line
	        , 
            _ref_block
	        , 
            _ref_id
	        , 
            _ref_isAuto
                    );
                }
            case CANMOVE:
                {
                    int i = 0;

            	boolean _ref_canMove;
            
            

            	int _ref_id;
            
            
                	memcpy(&_ref_id, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	boolean _ref_isAuto;
            
            
_ref_canMove = (bool) (paramQueue[0] [i] & AbstractActor::coeff[0]);
_ref_isAuto = (bool) (paramQueue[0] [i] & AbstractActor::coeff[1]);
++i;                    return msgsrvcanMove(
            _ref_canMove
	        , 
            _ref_id
	        , 
            _ref_isAuto
                    );
                }
    }
	return -1;
}


    long InfrastructureActor::constructorInfrastructure (byte creatorId 
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "Infrastructure";
		#endif

	#ifdef TTS
	#endif

		{
		}
		shift = 0;
		queueTail = 0;
		while(messageQueue[queueTail]) {applyPolicy(false);queueTail++;}
		return 0;

	}



		void InfrastructureActor::_msg_isAllowed (byte senderId
	        ,
    		boolean
	         _ref_line
	        , 
    		int
	         _ref_block
	        , 
    		int
	         _ref_id
	        , 
    		boolean
	         _ref_isAuto
  	  ){
        queueTail = 0;
        enqueue(messageQueue, ISALLOWED);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
            
                	memcpy(&paramsValues [index], &_ref_block, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_id, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            

paramsValues [index] = 0;
paramsValues [index] |= (_ref_line ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
paramsValues [index] |= (_ref_isAuto ? 0xFF : 0x00) & AbstractActor::coeff[1]; 
++index;            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void InfrastructureActor::_msg_canMove (byte senderId
	        ,
    		boolean
	         _ref_canMove
	        , 
    		int
	         _ref_id
	        , 
    		boolean
	         _ref_isAuto
  	  ){
        queueTail = 0;
        enqueue(messageQueue, CANMOVE);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
            
                	memcpy(&paramsValues [index], &_ref_id, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            

paramsValues [index] = 0;
paramsValues [index] |= (_ref_canMove ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
paramsValues [index] |= (_ref_isAuto ? 0xFF : 0x00) & AbstractActor::coeff[1]; 
++index;            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}









	byte** InfrastructureActor::table = (byte**)0xFFFFFFFF;

	byte** InfrastructureActor::getTable() {
		return table;
	}








	vector<TimeFrame>** InfrastructureActor::hashtableTimeExtension = (vector<TimeFrame>**)0xFFFFFFFF;
	
	vector<TimeFrame>** InfrastructureActor::getHashtableTimeExtension() {
		return hashtableTimeExtension;
	}








	    void InfrastructureActor::_timed_msg_isAllowed (byte senderId
	        ,
    		boolean
	         _ref_line
	        , 
    		int
	         _ref_block
	        , 
    		int
	         _ref_id
	        , 
    		boolean
	         _ref_isAuto
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_isAllowed (senderId
	        ,
            _ref_line
	        , 
            _ref_block
	        , 
            _ref_id
	        , 
            _ref_isAuto
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void InfrastructureActor::_timed_msg_canMove (byte senderId
	        ,
    		boolean
	         _ref_canMove
	        , 
    		int
	         _ref_id
	        , 
    		boolean
	         _ref_isAuto
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_canMove (senderId
	        ,
            _ref_canMove
	        , 
            _ref_id
	        , 
            _ref_isAuto
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	void InfrastructureActor::setNow(TIME_TYPE now) {
	
		AbstractTimedActor::setNow(now);
	}	
	

	    long InfrastructureActor::msgsrvisAllowed (
    		boolean
	         _ref_line
	        , 
    		int
	         _ref_block
	        , 
    		int
	         _ref_id
	        , 
    		boolean
	         _ref_isAuto
    ) {
    			long arrayIndexChecker = 0;ControllerActor *temp0;ControllerActor *temp1;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "isAllowed";
		#endif

	#ifdef TTS
		if (__pc == 0) {__pc = -1; goto __jumpPoint0;}
	#endif

		{
				__res = _ref_now + 1;
	__pc = 0;
	shift = 0;
	return 0;
	__jumpPoint0:
;
			if (_ref_isAuto) {
				{
					(_ref_autoCars[(arrayIndexChecker=_ref_id, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 110") , arrayIndexChecker)]=(dynamic_cast<AutoActor*>(_ref_sender)));
					(temp0=_ref_ctrl, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 111"), temp0)->_timed_msg_isAllowed(myID, _ref_line, _ref_block, _ref_id, true, _ref_now, MAX_TIME);
				}
}
			if ((!_ref_isAuto)) {
				{
					(_ref_mannedCars[(arrayIndexChecker=_ref_id, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 115") , arrayIndexChecker)]=(dynamic_cast<MannedActor*>(_ref_sender)));
					(temp1=_ref_ctrl, assertion(temp1!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 116"), temp1)->_timed_msg_isAllowed(myID, _ref_line, _ref_block, _ref_id, false, _ref_now, MAX_TIME);
				}
}
		}
		return 0;

}
	    long InfrastructureActor::msgsrvcanMove (
    		boolean
	         _ref_canMove
	        , 
    		int
	         _ref_id
	        , 
    		boolean
	         _ref_isAuto
    ) {
    			long arrayIndexChecker = 0;AutoActor *temp0;MannedActor *temp1;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "canMove";
		#endif

	#ifdef TTS
		if (__pc == 0) {__pc = -1; goto __jumpPoint0;}
	#endif

		{
				__res = _ref_now + 1;
	__pc = 0;
	shift = 0;
	return 0;
	__jumpPoint0:
;
			if (_ref_isAuto) {
				{
					(temp0=_ref_autoCars[(arrayIndexChecker=_ref_id, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 124") , arrayIndexChecker)], assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 124"), temp0)->_timed_msg_canMove(myID, _ref_canMove, true, _ref_now, MAX_TIME);
				}
}
			if ((!_ref_isAuto)) {
				{
					(temp1=_ref_mannedCars[(arrayIndexChecker=_ref_id, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <10, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 128") , arrayIndexChecker)], assertion(temp1!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 128"), temp1)->_timed_msg_canMove(myID, _ref_canMove, false, _ref_now, MAX_TIME);
				}
}
		}
		return 0;

}
