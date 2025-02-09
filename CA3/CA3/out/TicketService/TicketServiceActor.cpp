                                    


#include "Types.h"
#include "TicketServiceActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

#include "CustomerActor.h"
#include "AgentActor.h"

// location counters (values stored in messageQueue[0])
/* There can be more than one location counter for each msgsrv when we
   want to have non-atomic message servers.
   Notice that values 0 and 1 are reserved for EMPTY and INITIAL, respectively.*/
#define REQUESTTICKET 1


// knownrebecs (those to whom message can be sent)
// the following format allows to use them as variables --> the ID of the rebec
#define _ref_sender (rebecs[senderQueue[0]])
#define _ref_self (dynamic_cast<TicketServiceActor*>(rebecs[(int)((unsigned char)myID)]))

    	#define _ref_a (dynamic_cast<AgentActor*>(rebecs[(int)((unsigned char)known[1])]))
    
TicketServiceActor::TicketServiceActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
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
    messageNames = new char *[1 + 1];
    messagePriorities = new int[1 + 1];
	messageNames[0] = (char*)"Empty";

	
		
	
		
       		 messageNames[1] = (char*)"REQUESTTICKET";
			messagePriorities[1] = std::numeric_limits<int>::max();








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

TicketServiceActor::~TicketServiceActor (){
	delete [] messageNames;
	delete [] messageQueue;
	delete [] senderQueue;
}


void TicketServiceActor::exportAMessageInXML(ostream &out, string tab, int cnt) {
        int i = 0;
        switch (messageQueue[cnt]) {
            case REQUESTTICKET: {

            	CustomerActor* _ref_customer;
            
            
                	memcpy(&_ref_customer, &paramQueue[cnt] [i], (1 * REACTIVE_CLASS_SIZE));
	                i += (1 * REACTIVE_CLASS_SIZE);
                out << "requestTicket("
                	<< (_ref_customer == NULL ? "NULL" : _ref_customer->getName())
                << ")";
                break;
               }
    	}
}


void TicketServiceActor::exportStateVariablesInXML(ostream &out, string tab) {
	out << tab << "<statevariables>" << endl;
	exportInsideStateVariablesTagInXML(out, tab);
	out << tab << "</statevariables>" << endl;
}

void TicketServiceActor::exportInsideStateVariablesTagInXML(ostream &out, string tab){


			    	out << tab << '\t' << "<variable name=\"TicketService.issueDelay\" type=\"int\">" <<  
			    		((int)_ref_issueDelay) << "</variable>" << endl;
}


char* TicketServiceActor::getClassName() {
    return (char*)"TicketService";
}
void TicketServiceActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    	
            
                	memcpy(&array [marshalIndex], &_ref_issueDelay, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);

    
	
}
void TicketServiceActor::marshalActorToArray(byte* array) {
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

void TicketServiceActor::unmarshalActorFromArray(byte* array) {

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

void TicketServiceActor::unmarshalVariablesFromArray(byte * array){

	int index = 0;

            
            
                	memcpy(&_ref_issueDelay, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
    
	
}

/*#ifdef DEBUG
void TicketServiceActor::printStateContent(long stateNo) {
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
long TicketServiceActor::methodSelectorAndExecutor() {

	
    switch (messageQueue[0]) {

            case REQUESTTICKET:
                {
                    int i = 0;

            	CustomerActor* _ref_customer;
            
            
                	memcpy(&_ref_customer, &paramQueue[0] [i], (1 * REACTIVE_CLASS_SIZE));
	                i += (1 * REACTIVE_CLASS_SIZE);
                    return msgsrvrequestTicket(
            _ref_customer
                    );
                }
    }
	return -1;
}


    long TicketServiceActor::constructorTicketService (byte creatorId 
	        ,
    		int
	         _ref_myIssueDelay
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "TicketService";
		#endif

	#ifdef TTS
	#endif

		{
			(_ref_issueDelay=_ref_myIssueDelay);
		}
		shift = 0;
		queueTail = 0;
		while(messageQueue[queueTail]) {applyPolicy(false);queueTail++;}
		return 0;

	}



		void TicketServiceActor::_msg_requestTicket (byte senderId
	        ,
    		CustomerActor*
	         _ref_customer
  	  ){
        queueTail = 0;
        enqueue(messageQueue, REQUESTTICKET);
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









	byte** TicketServiceActor::table = (byte**)0xFFFFFFFF;

	byte** TicketServiceActor::getTable() {
		return table;
	}








	vector<TimeFrame>** TicketServiceActor::hashtableTimeExtension = (vector<TimeFrame>**)0xFFFFFFFF;
	
	vector<TimeFrame>** TicketServiceActor::getHashtableTimeExtension() {
		return hashtableTimeExtension;
	}








	    void TicketServiceActor::_timed_msg_requestTicket (byte senderId
	        ,
    		CustomerActor*
	         _ref_customer
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_requestTicket (senderId
	        ,
            _ref_customer
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	void TicketServiceActor::setNow(TIME_TYPE now) {
	
		AbstractTimedActor::setNow(now);
	}	
	

	    long TicketServiceActor::msgsrvrequestTicket (
    		CustomerActor*
	         _ref_customer
    ) {
    			long arrayIndexChecker = 0;AgentActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "requestTicket";
		#endif

	#ifdef TTS
		if (__pc == 0) {__pc = -1; goto __jumpPoint0;}
	#endif

		{
				__res = _ref_now + _ref_issueDelay;
	__pc = 0;
	shift = 0;
	return 0;
	__jumpPoint0:
;
			(temp0=_ref_a, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 50"), temp0)->_timed_msg_ticketIssued(myID, _ref_customer, _ref_now, MAX_TIME);
		}
		return 0;

}
