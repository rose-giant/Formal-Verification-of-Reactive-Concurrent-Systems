                                    


#include "Types.h"
#include "CustomerActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

#include "AgentActor.h"
#include "TicketServiceActor.h"

// location counters (values stored in messageQueue[0])
/* There can be more than one location counter for each msgsrv when we
   want to have non-atomic message servers.
   Notice that values 0 and 1 are reserved for EMPTY and INITIAL, respectively.*/
#define TRY 1
#define TICKETISSUED 2


// knownrebecs (those to whom message can be sent)
// the following format allows to use them as variables --> the ID of the rebec
#define _ref_sender (rebecs[senderQueue[0]])
#define _ref_self (dynamic_cast<CustomerActor*>(rebecs[(int)((unsigned char)myID)]))

    	#define _ref_a (dynamic_cast<AgentActor*>(rebecs[(int)((unsigned char)known[1])]))
    
CustomerActor::CustomerActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
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

	
		
	
		
       		 messageNames[1] = (char*)"TRY";
			messagePriorities[1] = std::numeric_limits<int>::max();
       		 messageNames[2] = (char*)"TICKETISSUED";
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

CustomerActor::~CustomerActor (){
	delete [] messageNames;
	delete [] messageQueue;
	delete [] senderQueue;
}


void CustomerActor::exportAMessageInXML(ostream &out, string tab, int cnt) {
        int i = 0;
        switch (messageQueue[cnt]) {
            case TRY: {
                out << "try("
                << ")";
                break;
               }
            case TICKETISSUED: {
                out << "ticketIssued("
                << ")";
                break;
               }
    	}
}


void CustomerActor::exportStateVariablesInXML(ostream &out, string tab) {
	out << tab << "<statevariables>" << endl;
	exportInsideStateVariablesTagInXML(out, tab);
	out << tab << "</statevariables>" << endl;
}

void CustomerActor::exportInsideStateVariablesTagInXML(ostream &out, string tab){


			    	out << tab << '\t' << "<variable name=\"Customer.id\" type=\"byte\">" <<  
			    		((int)_ref_id) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Customer.sent\" type=\"boolean\">" <<  
			    		(_ref_sent? "true" : "false") << "</variable>" << endl;
}


char* CustomerActor::getClassName() {
    return (char*)"Customer";
}
void CustomerActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    	
            
                	memcpy(&array [marshalIndex], &_ref_id, (1 * BYTE_SIZE));
	                marshalIndex += (1 * BYTE_SIZE);
            

array [marshalIndex] = 0;
array [marshalIndex] |= (_ref_sent ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
++marshalIndex;    
	
}
void CustomerActor::marshalActorToArray(byte* array) {
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

void CustomerActor::unmarshalActorFromArray(byte* array) {

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

void CustomerActor::unmarshalVariablesFromArray(byte * array){

	int index = 0;

            
            
                	memcpy(&_ref_id, &array [index], (1 * BYTE_SIZE));
	                index += (1 * BYTE_SIZE);

            
            
_ref_sent = (bool) (array [index] & AbstractActor::coeff[0]);
++index;    
	
}

/*#ifdef DEBUG
void CustomerActor::printStateContent(long stateNo) {
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
long CustomerActor::methodSelectorAndExecutor() {

	
    switch (messageQueue[0]) {

            case TRY:
                {
                    int i = 0;
                    return msgsrvtry(
                    );
                }
            case TICKETISSUED:
                {
                    int i = 0;
                    return msgsrvticketIssued(
                    );
                }
    }
	return -1;
}


    long CustomerActor::constructorCustomer (byte creatorId 
	        ,
    		byte
	         _ref_myId
    ) {
    			long arrayIndexChecker = 0;CustomerActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "Customer";
		#endif

	#ifdef TTS
	#endif

		{
			(_ref_id=_ref_myId);
			(_ref_sent=false);
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 12"), temp0)->_timed_msg_try(myID, _ref_now, MAX_TIME);
		}
		shift = 0;
		queueTail = 0;
		while(messageQueue[queueTail]) {applyPolicy(false);queueTail++;}
		return 0;

	}



		void CustomerActor::_msg_try (byte senderId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, TRY);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void CustomerActor::_msg_ticketIssued (byte senderId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, TICKETISSUED);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}









	byte** CustomerActor::table = (byte**)0xFFFFFFFF;

	byte** CustomerActor::getTable() {
		return table;
	}








	vector<TimeFrame>** CustomerActor::hashtableTimeExtension = (vector<TimeFrame>**)0xFFFFFFFF;
	
	vector<TimeFrame>** CustomerActor::getHashtableTimeExtension() {
		return hashtableTimeExtension;
	}








	    void CustomerActor::_timed_msg_try (byte senderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_try (senderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void CustomerActor::_timed_msg_ticketIssued (byte senderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_ticketIssued (senderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	void CustomerActor::setNow(TIME_TYPE now) {
	
		AbstractTimedActor::setNow(now);
	}	
	

	    long CustomerActor::msgsrvtry (
    ) {
    			long arrayIndexChecker = 0;AgentActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "try";
		#endif

	#ifdef TTS
	#endif

		{
			(temp0=_ref_a, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 15"), temp0)->_timed_msg_requestTicket(myID, _ref_now, MAX_TIME);
			(_ref_sent=true);
		}
		return 0;

}
	    long CustomerActor::msgsrvticketIssued (
    ) {
    			long arrayIndexChecker = 0;CustomerActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "ticketIssued";
		#endif

	#ifdef TTS
	#endif

		{
			(_ref_sent=false);
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 20"), temp0)->_timed_msg_try(myID, 30 + _ref_now, MAX_TIME);
		}
		return 0;

}
