                                    


#include "Types.h"
#include "AutoActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

#include "ControllerActor.h"
#include "InfrastructureActor.h"
#include "MannedActor.h"

// location counters (values stored in messageQueue[0])
/* There can be more than one location counter for each msgsrv when we
   want to have non-atomic message servers.
   Notice that values 0 and 1 are reserved for EMPTY and INITIAL, respectively.*/
#define CANMOVE 1
#define LIVE 2


// knownrebecs (those to whom message can be sent)
// the following format allows to use them as variables --> the ID of the rebec
#define _ref_sender (rebecs[senderQueue[0]])
#define _ref_self (dynamic_cast<AutoActor*>(rebecs[(int)((unsigned char)myID)]))

    	#define _ref_infra (dynamic_cast<InfrastructureActor*>(rebecs[(int)((unsigned char)known[1])]))
    
AutoActor::AutoActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
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

	
		
	
		
       		 messageNames[1] = (char*)"CANMOVE";
			messagePriorities[1] = std::numeric_limits<int>::max();
       		 messageNames[2] = (char*)"LIVE";
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

AutoActor::~AutoActor (){
	delete [] messageNames;
	delete [] messageQueue;
	delete [] senderQueue;
}


void AutoActor::exportAMessageInXML(ostream &out, string tab, int cnt) {
        int i = 0;
        switch (messageQueue[cnt]) {
            case CANMOVE: {

            	boolean _ref_canMove;
            
            

            	boolean _ref_isAuto;
            
            
_ref_canMove = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[0]);
_ref_isAuto = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[1]);
++i;                out << "canMove("
                	<< (_ref_canMove? "true" : "false")
					<< ", "
                	<< (_ref_isAuto? "true" : "false")
                << ")";
                break;
               }
            case LIVE: {
                out << "live("
                << ")";
                break;
               }
    	}
}


void AutoActor::exportStateVariablesInXML(ostream &out, string tab) {
	out << tab << "<statevariables>" << endl;
	exportInsideStateVariablesTagInXML(out, tab);
	out << tab << "</statevariables>" << endl;
}

void AutoActor::exportInsideStateVariablesTagInXML(ostream &out, string tab){


			    	out << tab << '\t' << "<variable name=\"Auto.shouldStop\" type=\"boolean\">" <<  
			    		(_ref_shouldStop? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Auto.line\" type=\"boolean\">" <<  
			    		(_ref_line? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Auto.id\" type=\"int\">" <<  
			    		((int)_ref_id) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Auto.block\" type=\"int\">" <<  
			    		((int)_ref_block) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Auto.up\" type=\"boolean\">" <<  
			    		(_ref_up? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Auto.asked\" type=\"boolean\">" <<  
			    		(_ref_asked? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Auto.askedCtrl\" type=\"boolean\">" <<  
			    		(_ref_askedCtrl? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Auto.firstStep\" type=\"boolean\">" <<  
			    		(_ref_firstStep? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Auto.lastStep\" type=\"boolean\">" <<  
			    		(_ref_lastStep? "true" : "false") << "</variable>" << endl;
}


char* AutoActor::getClassName() {
    return (char*)"Auto";
}
void AutoActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    	
            
            
            
                	memcpy(&array [marshalIndex], &_ref_id, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            
                	memcpy(&array [marshalIndex], &_ref_block, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            
            
            
            
            

array [marshalIndex] = 0;
array [marshalIndex] |= (_ref_shouldStop ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
array [marshalIndex] |= (_ref_line ? 0xFF : 0x00) & AbstractActor::coeff[1]; 
array [marshalIndex] |= (_ref_up ? 0xFF : 0x00) & AbstractActor::coeff[2]; 
array [marshalIndex] |= (_ref_asked ? 0xFF : 0x00) & AbstractActor::coeff[3]; 
array [marshalIndex] |= (_ref_askedCtrl ? 0xFF : 0x00) & AbstractActor::coeff[4]; 
array [marshalIndex] |= (_ref_firstStep ? 0xFF : 0x00) & AbstractActor::coeff[5]; 
array [marshalIndex] |= (_ref_lastStep ? 0xFF : 0x00) & AbstractActor::coeff[6]; 
++marshalIndex;    
	
}
void AutoActor::marshalActorToArray(byte* array) {
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

void AutoActor::unmarshalActorFromArray(byte* array) {

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

void AutoActor::unmarshalVariablesFromArray(byte * array){

	int index = 0;

            
            

            
            

            
            
                	memcpy(&_ref_id, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            
            
                	memcpy(&_ref_block, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            
            

            
            

            
            

            
            

            
            
_ref_shouldStop = (bool) (array [index] & AbstractActor::coeff[0]);
_ref_line = (bool) (array [index] & AbstractActor::coeff[1]);
_ref_up = (bool) (array [index] & AbstractActor::coeff[2]);
_ref_asked = (bool) (array [index] & AbstractActor::coeff[3]);
_ref_askedCtrl = (bool) (array [index] & AbstractActor::coeff[4]);
_ref_firstStep = (bool) (array [index] & AbstractActor::coeff[5]);
_ref_lastStep = (bool) (array [index] & AbstractActor::coeff[6]);
++index;    
	
}

/*#ifdef DEBUG
void AutoActor::printStateContent(long stateNo) {
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
long AutoActor::methodSelectorAndExecutor() {

	
    switch (messageQueue[0]) {

            case CANMOVE:
                {
                    int i = 0;

            	boolean _ref_canMove;
            
            

            	boolean _ref_isAuto;
            
            
_ref_canMove = (bool) (paramQueue[0] [i] & AbstractActor::coeff[0]);
_ref_isAuto = (bool) (paramQueue[0] [i] & AbstractActor::coeff[1]);
++i;                    return msgsrvcanMove(
            _ref_canMove
	        , 
            _ref_isAuto
                    );
                }
            case LIVE:
                {
                    int i = 0;
                    return msgsrvlive(
                    );
                }
    }
	return -1;
}


    long AutoActor::constructorAuto (byte creatorId 
	        ,
    		int
	         _ref_inputId
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "Auto";
		#endif

	#ifdef TTS
	#endif

		{
			(_ref_id=_ref_inputId);
			_synchmethod_initVars();
			_synchmethod_move();
		}
		shift = 0;
		queueTail = 0;
		while(messageQueue[queueTail]) {applyPolicy(false);queueTail++;}
		return 0;

	}

 
		void AutoActor::_synchmethod_initVars (
   		 ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "initVars";
		#endif

	#ifdef TTS
	#endif

		{
			(_ref_up=false);
			(_ref_line=true);
			(_ref_block=(_ref_MAX_BLOCK_NUM-1));
			(_ref_asked=false);
			(_ref_firstStep=true);
			(_ref_lastStep=false);
		}

		}
 
		void AutoActor::_synchmethod_move (
   		 ) {
    			long arrayIndexChecker = 0;InfrastructureActor *temp0;AutoActor *temp1;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "move";
		#endif

	#ifdef TTS
	#endif

		{
			if ((_ref_block==1)) {
				{
					(_ref_lastStep=true);
				}
}
			if ((_ref_asked||_ref_firstStep)) {
				{
					(_ref_firstStep=false);
					(temp0=_ref_infra, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 186"), temp0)->_timed_msg_isAllowed(myID, _ref_line, _ref_block, _ref_id, true, _ref_now, 4 + _ref_now);
					(_ref_asked=false);
					if ((!_ref_shouldStop)) {
						{
							if ((_ref_up&&(_ref_block==_ref_MAX_BLOCK_NUM))) {
								{
									(_ref_block--);
									(_ref_up=(!_ref_up));
								}
}
							if (((!_ref_up)&&(_ref_block==1))) {
								{
									(_ref_up=(!_ref_up));
									(_ref_block++);
								}
}
							if (((_ref_up&&(_ref_block<_ref_MAX_BLOCK_NUM))&&(_ref_block>1))) {
								{
									(_ref_block++);
								}
}
							if ((((!_ref_up)&&(_ref_block<_ref_MAX_BLOCK_NUM))&&(_ref_block>1))) {
								{
									(_ref_block--);
								}
}
						}
}
				}
}
			(temp1=_ref_self, assertion(temp1!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 211"), temp1)->_timed_msg_live(myID, _ref_now, 1 + _ref_now);
		}

		}


		void AutoActor::_msg_canMove (byte senderId
	        ,
    		boolean
	         _ref_canMove
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
    	
            
            

paramsValues [index] = 0;
paramsValues [index] |= (_ref_canMove ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
paramsValues [index] |= (_ref_isAuto ? 0xFF : 0x00) & AbstractActor::coeff[1]; 
++index;            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void AutoActor::_msg_live (byte senderId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, LIVE);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}









	byte** AutoActor::table = (byte**)0xFFFFFFFF;

	byte** AutoActor::getTable() {
		return table;
	}








	vector<TimeFrame>** AutoActor::hashtableTimeExtension = (vector<TimeFrame>**)0xFFFFFFFF;
	
	vector<TimeFrame>** AutoActor::getHashtableTimeExtension() {
		return hashtableTimeExtension;
	}








	    void AutoActor::_timed_msg_canMove (byte senderId
	        ,
    		boolean
	         _ref_canMove
	        , 
    		boolean
	         _ref_isAuto
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_canMove (senderId
	        ,
            _ref_canMove
	        , 
            _ref_isAuto
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void AutoActor::_timed_msg_live (byte senderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_live (senderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	void AutoActor::setNow(TIME_TYPE now) {
	
		AbstractTimedActor::setNow(now);
	}	
	

	    long AutoActor::msgsrvcanMove (
    		boolean
	         _ref_canMove
	        , 
    		boolean
	         _ref_isAuto
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "canMove";
		#endif

	#ifdef TTS
	#endif

		{
			if (_ref_isAuto) {
				{
					(_ref_shouldStop=_ref_canMove);
					(_ref_asked=true);
				}
}
		}
		return 0;

}
	    long AutoActor::msgsrvlive (
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "live";
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
			_synchmethod_move();
		}
		return 0;

}
