                                    


#include "Types.h"
#include "MiscActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

#include "CPUActor.h"
#include "SensorActor.h"
#include "WirelessMediumActor.h"
#include "CommunicationDeviceActor.h"

// location counters (values stored in messageQueue[0])
/* There can be more than one location counter for each msgsrv when we
   want to have non-atomic message servers.
   Notice that values 0 and 1 are reserved for EMPTY and INITIAL, respectively.*/
#define MISCFIRST 1
#define MISCLOOP 2


// knownrebecs (those to whom message can be sent)
// the following format allows to use them as variables --> the ID of the rebec
#define _ref_sender (rebecs[senderQueue[0]])
#define _ref_self (dynamic_cast<MiscActor*>(rebecs[(int)((unsigned char)myID)]))

    	#define _ref_cpu (dynamic_cast<CPUActor*>(rebecs[(int)((unsigned char)known[1])]))
    
MiscActor::MiscActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
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

	
		
	
		
       		 messageNames[1] = (char*)"MISCFIRST";
			messagePriorities[1] = std::numeric_limits<int>::max();
       		 messageNames[2] = (char*)"MISCLOOP";
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

MiscActor::~MiscActor (){
	delete [] messageNames;
	delete [] messageQueue;
	delete [] senderQueue;
}


void MiscActor::exportAMessageInXML(ostream &out, string tab, int cnt) {
        int i = 0;
        switch (messageQueue[cnt]) {
            case MISCFIRST: {
                out << "miscFirst("
                << ")";
                break;
               }
            case MISCLOOP: {
                out << "miscLoop("
                << ")";
                break;
               }
    	}
}


void MiscActor::exportStateVariablesInXML(ostream &out, string tab) {
	out << tab << "<statevariables>" << endl;
	exportInsideStateVariablesTagInXML(out, tab);
	out << tab << "</statevariables>" << endl;
}

void MiscActor::exportInsideStateVariablesTagInXML(ostream &out, string tab){


}


char* MiscActor::getClassName() {
    return (char*)"Misc";
}
void MiscActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    
	
}
void MiscActor::marshalActorToArray(byte* array) {
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

void MiscActor::unmarshalActorFromArray(byte* array) {

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

void MiscActor::unmarshalVariablesFromArray(byte * array){

	int index = 0;
    
	
}

/*#ifdef DEBUG
void MiscActor::printStateContent(long stateNo) {
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
long MiscActor::methodSelectorAndExecutor() {

	
    switch (messageQueue[0]) {

            case MISCFIRST:
                {
                    int i = 0;
                    return msgsrvmiscFirst(
                    );
                }
            case MISCLOOP:
                {
                    int i = 0;
                    return msgsrvmiscLoop(
                    );
                }
    }
	return -1;
}


    long MiscActor::constructorMisc (byte creatorId 
    ) {
    			long arrayIndexChecker = 0;MiscActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "Misc";
		#endif

	#ifdef TTS
	#endif

		{
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 98"), temp0)->_timed_msg_miscFirst(myID, _ref_now, MAX_TIME);
		}
		shift = 0;
		queueTail = 0;
		while(messageQueue[queueTail]) {applyPolicy(false);queueTail++;}
		return 0;

	}



		void MiscActor::_msg_miscFirst (byte senderId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, MISCFIRST);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void MiscActor::_msg_miscLoop (byte senderId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, MISCLOOP);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}









	byte** MiscActor::table = (byte**)0xFFFFFFFF;

	byte** MiscActor::getTable() {
		return table;
	}








	vector<TimeFrame>** MiscActor::hashtableTimeExtension = (vector<TimeFrame>**)0xFFFFFFFF;
	
	vector<TimeFrame>** MiscActor::getHashtableTimeExtension() {
		return hashtableTimeExtension;
	}








	    void MiscActor::_timed_msg_miscFirst (byte senderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_miscFirst (senderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void MiscActor::_timed_msg_miscLoop (byte senderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_miscLoop (senderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	void MiscActor::setNow(TIME_TYPE now) {
	
		AbstractTimedActor::setNow(now);
	}	
	

	    long MiscActor::msgsrvmiscFirst (
    ) {
    			long arrayIndexChecker = 0;MiscActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "miscFirst";
		#endif
		long __tNumber = 0;
		static byte nonDetVariable0 = 10;

	#ifdef TTS
	#endif

		{
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 102"), temp0)->_timed_msg_miscLoop(myID, (__tNumber|=1, nonDetVariable0) + _ref_now, MAX_TIME);
		}
		if (__tNumber & 1) {
			if (false){/*Dummy 'if' statement to create nested else-if easily!*/}
			else if(nonDetVariable0 == 10) {
				nonDetVariable0 = 20;
			}
			else if(nonDetVariable0 == 20) {
				nonDetVariable0 = 30;
			}
			else if(nonDetVariable0 == 30) {
				nonDetVariable0 = 10;
				__tNumber &= ~1;
			}
			if (__tNumber & 1) {
				return __tNumber;
			}
		}
		return 0;

}
	    long MiscActor::msgsrvmiscLoop (
    ) {
    			long arrayIndexChecker = 0;CPUActor *temp0;MiscActor *temp1;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "miscLoop";
		#endif

	#ifdef TTS
	#endif

		{
			(temp0=_ref_cpu, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 106"), temp0)->_timed_msg_miscEvent(myID, _ref_now, MAX_TIME);
			(temp1=_ref_self, assertion(temp1!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 107"), temp1)->_timed_msg_miscLoop(myID, _ref_miscPeriod + _ref_now, MAX_TIME);
		}
		return 0;

}
