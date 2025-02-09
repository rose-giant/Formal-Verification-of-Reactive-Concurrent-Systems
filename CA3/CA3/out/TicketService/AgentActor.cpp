                                    


#include "Types.h"
#include "AgentActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

#include "CustomerActor.h"
#include "TicketServiceActor.h"

// location counters (values stored in messageQueue[0])
/* There can be more than one location counter for each msgsrv when we
   want to have non-atomic message servers.
   Notice that values 0 and 1 are reserved for EMPTY and INITIAL, respectively.*/
#define REQUESTTICKET 1
#define TICKETISSUED 2


// knownrebecs (those to whom message can be sent)
// the following format allows to use them as variables --> the ID of the rebec
#define _ref_sender (rebecs[senderQueue[0]])
#define _ref_self (dynamic_cast<AgentActor*>(rebecs[(int)((unsigned char)myID)]))

    	#define _ref_ts (dynamic_cast<TicketServiceActor*>(rebecs[(int)((unsigned char)known[1])]))
    
AgentActor::AgentActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
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

	
		
	
		
       		 messageNames[1] = (char*)"REQUESTTICKET";
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

AgentActor::~AgentActor (){
	delete [] messageNames;
	delete [] messageQueue;
	delete [] senderQueue;
}


void AgentActor::exportAMessageInXML(ostream &out, string tab, int cnt) {
        int i = 0;
        switch (messageQueue[cnt]) {
            case REQUESTTICKET: {
                out << "requestTicket("
                << ")";
                break;
               }
            case TICKETISSUED: {

            	CustomerActor* _ref_customer;
            
            
                	memcpy(&_ref_customer, &paramQueue[cnt] [i], (1 * REACTIVE_CLASS_SIZE));
	                i += (1 * REACTIVE_CLASS_SIZE);
                out << "ticketIssued("
                	<< (_ref_customer == NULL ? "NULL" : _ref_customer->getName())
                << ")";
                break;
               }
    	}
}


void AgentActor::exportStateVariablesInXML(ostream &out, string tab) {
	out << tab << "<statevariables>" << endl;
	exportInsideStateVariablesTagInXML(out, tab);
	out << tab << "</statevariables>" << endl;
}

void AgentActor::exportInsideStateVariablesTagInXML(ostream &out, string tab){


}


char* AgentActor::getClassName() {
    return (char*)"Agent";
}
void AgentActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    
	
}
void AgentActor::marshalActorToArray(byte* array) {
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

void AgentActor::unmarshalActorFromArray(byte* array) {

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

void AgentActor::unmarshalVariablesFromArray(byte * array){

	int index = 0;
    
	
}

/*#ifdef DEBUG
void AgentActor::printStateContent(long stateNo) {
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
long AgentActor::methodSelectorAndExecutor() {

	
    switch (messageQueue[0]) {

            case REQUESTTICKET:
                {
                    int i = 0;
                    return msgsrvrequestTicket(
                    );
                }
            case TICKETISSUED:
                {
                    int i = 0;

            	CustomerActor* _ref_customer;
            
            
                	memcpy(&_ref_customer, &paramQueue[0] [i], (1 * REACTIVE_CLASS_SIZE));
	                i += (1 * REACTIVE_CLASS_SIZE);
                    return msgsrvticketIssued(
            _ref_customer
                    );
                }
    }
	return -1;
}


    long AgentActor::constructorAgent (byte creatorId 
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "Agent";
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



		void AgentActor::_msg_requestTicket (byte senderId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, REQUESTTICKET);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void AgentActor::_msg_ticketIssued (byte senderId
	        ,
    		CustomerActor*
	         _ref_customer
  	  ){
        queueTail = 0;
        enqueue(messageQueue, TICKETISSUED);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
                	memcpy(&paramsValues [index], &_ref_customer, (1 * REACTIVE_CLASS_SIZE));
	                index += (1 * REACTIVE_CLASS_SIZE);

            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}









	byte** AgentActor::table = (byte**)0xFFFFFFFF;

	byte** AgentActor::getTable() {
		return table;
	}








	vector<TimeFrame>** AgentActor::hashtableTimeExtension = (vector<TimeFrame>**)0xFFFFFFFF;
	
	vector<TimeFrame>** AgentActor::getHashtableTimeExtension() {
		return hashtableTimeExtension;
	}








	    void AgentActor::_timed_msg_requestTicket (byte senderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_requestTicket (senderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void AgentActor::_timed_msg_ticketIssued (byte senderId
	        ,
    		CustomerActor*
	         _ref_customer
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_ticketIssued (senderId
	        ,
            _ref_customer
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	void AgentActor::setNow(TIME_TYPE now) {
	
		AbstractTimedActor::setNow(now);
	}	
	

	    long AgentActor::msgsrvrequestTicket (
    ) {
    			long arrayIndexChecker = 0;TicketServiceActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "requestTicket";
		#endif

	#ifdef TTS
	#endif

		{
			(temp0=_ref_ts, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 32"), temp0)->_timed_msg_requestTicket(myID, (dynamic_cast<CustomerActor*>(_ref_sender)), _ref_now, 24 + _ref_now);
		}
		return 0;

}
	    long AgentActor::msgsrvticketIssued (
    		CustomerActor*
	         _ref_customer
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
			(temp0=_ref_customer, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 35"), temp0)->_timed_msg_ticketIssued(myID, _ref_now, MAX_TIME);
		}
		return 0;

}
