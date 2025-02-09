                                    


#include "Types.h"
#include "WirelessMediumActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

#include "CPUActor.h"
#include "SensorActor.h"
#include "MiscActor.h"
#include "CommunicationDeviceActor.h"

// location counters (values stored in messageQueue[0])
/* There can be more than one location counter for each msgsrv when we
   want to have non-atomic message servers.
   Notice that values 0 and 1 are reserved for EMPTY and INITIAL, respectively.*/
#define GETSTATUS 1
#define BROADCAST 2
#define BROADCASTINGISCOMPLETED 3


// knownrebecs (those to whom message can be sent)
// the following format allows to use them as variables --> the ID of the rebec
#define _ref_sender (rebecs[senderQueue[0]])
#define _ref_self (dynamic_cast<WirelessMediumActor*>(rebecs[(int)((unsigned char)myID)]))

    
WirelessMediumActor::WirelessMediumActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
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
    messageNames = new char *[3 + 1];
    messagePriorities = new int[3 + 1];
	messageNames[0] = (char*)"Empty";

	
		
	
		
       		 messageNames[1] = (char*)"GETSTATUS";
			messagePriorities[1] = std::numeric_limits<int>::max();
       		 messageNames[2] = (char*)"BROADCAST";
			messagePriorities[2] = std::numeric_limits<int>::max();
       		 messageNames[3] = (char*)"BROADCASTINGISCOMPLETED";
			messagePriorities[3] = std::numeric_limits<int>::max();








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

WirelessMediumActor::~WirelessMediumActor (){
	delete [] messageNames;
	delete [] messageQueue;
	delete [] senderQueue;
}


void WirelessMediumActor::exportAMessageInXML(ostream &out, string tab, int cnt) {
        int i = 0;
        switch (messageQueue[cnt]) {
            case GETSTATUS: {
                out << "getStatus("
                << ")";
                break;
               }
            case BROADCAST: {

            	CommunicationDeviceActor* _ref_receiver;
            
            
                	memcpy(&_ref_receiver, &paramQueue[cnt] [i], (1 * REACTIVE_CLASS_SIZE));
	                i += (1 * REACTIVE_CLASS_SIZE);

            	int _ref_data;
            
            
                	memcpy(&_ref_data, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_packetsNumber;
            
            
                	memcpy(&_ref_packetsNumber, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                out << "broadcast("
                	<< (_ref_receiver == NULL ? "NULL" : _ref_receiver->getName())
					<< ", "
                	<< ((int)_ref_data)
					<< ", "
                	<< ((int)_ref_packetsNumber)
                << ")";
                break;
               }
            case BROADCASTINGISCOMPLETED: {
                out << "broadcastingIsCompleted("
                << ")";
                break;
               }
    	}
}


void WirelessMediumActor::exportStateVariablesInXML(ostream &out, string tab) {
	out << tab << "<statevariables>" << endl;
	exportInsideStateVariablesTagInXML(out, tab);
	out << tab << "</statevariables>" << endl;
}

void WirelessMediumActor::exportInsideStateVariablesTagInXML(ostream &out, string tab){


			    	out << tab << '\t' << "<variable name=\"WirelessMedium.senderDevice\" type=\"CommunicationDevice\">" <<  
			    		(_ref_senderDevice == NULL ? "NULL" : _ref_senderDevice->getName()) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"WirelessMedium.receiverDevice\" type=\"CommunicationDevice\">" <<  
			    		(_ref_receiverDevice == NULL ? "NULL" : _ref_receiverDevice->getName()) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"WirelessMedium.maxTraffic\" type=\"int\">" <<  
			    		((int)_ref_maxTraffic) << "</variable>" << endl;
}


char* WirelessMediumActor::getClassName() {
    return (char*)"WirelessMedium";
}
void WirelessMediumActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    	
            
                	memcpy(&array [marshalIndex], &_ref_senderDevice, (1 * REACTIVE_CLASS_SIZE));
	                marshalIndex += (1 * REACTIVE_CLASS_SIZE);
            
                	memcpy(&array [marshalIndex], &_ref_receiverDevice, (1 * REACTIVE_CLASS_SIZE));
	                marshalIndex += (1 * REACTIVE_CLASS_SIZE);
            
                	memcpy(&array [marshalIndex], &_ref_maxTraffic, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);

    
	
}
void WirelessMediumActor::marshalActorToArray(byte* array) {
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

void WirelessMediumActor::unmarshalActorFromArray(byte* array) {

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

void WirelessMediumActor::unmarshalVariablesFromArray(byte * array){

	int index = 0;

            
            
                	memcpy(&_ref_senderDevice, &array [index], (1 * REACTIVE_CLASS_SIZE));
	                index += (1 * REACTIVE_CLASS_SIZE);

            
            
                	memcpy(&_ref_receiverDevice, &array [index], (1 * REACTIVE_CLASS_SIZE));
	                index += (1 * REACTIVE_CLASS_SIZE);

            
            
                	memcpy(&_ref_maxTraffic, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
    
	
}

/*#ifdef DEBUG
void WirelessMediumActor::printStateContent(long stateNo) {
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
long WirelessMediumActor::methodSelectorAndExecutor() {

	
    switch (messageQueue[0]) {

            case GETSTATUS:
                {
                    int i = 0;
                    return msgsrvgetStatus(
                    );
                }
            case BROADCAST:
                {
                    int i = 0;

            	CommunicationDeviceActor* _ref_receiver;
            
            
                	memcpy(&_ref_receiver, &paramQueue[0] [i], (1 * REACTIVE_CLASS_SIZE));
	                i += (1 * REACTIVE_CLASS_SIZE);

            	int _ref_data;
            
            
                	memcpy(&_ref_data, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_packetsNumber;
            
            
                	memcpy(&_ref_packetsNumber, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                    return msgsrvbroadcast(
            _ref_receiver
	        , 
            _ref_data
	        , 
            _ref_packetsNumber
                    );
                }
            case BROADCASTINGISCOMPLETED:
                {
                    int i = 0;
                    return msgsrvbroadcastingIsCompleted(
                    );
                }
    }
	return -1;
}


    long WirelessMediumActor::constructorWirelessMedium (byte creatorId 
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "WirelessMedium";
		#endif

	#ifdef TTS
	#endif

		{
			(_ref_senderDevice=null);
			(_ref_receiverDevice=null);
			(_ref_maxTraffic=((125*1024)/8));
		}
		shift = 0;
		queueTail = 0;
		while(messageQueue[queueTail]) {applyPolicy(false);queueTail++;}
		return 0;

	}



		void WirelessMediumActor::_msg_getStatus (byte senderId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, GETSTATUS);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void WirelessMediumActor::_msg_broadcast (byte senderId
	        ,
    		CommunicationDeviceActor*
	         _ref_receiver
	        , 
    		int
	         _ref_data
	        , 
    		int
	         _ref_packetsNumber
  	  ){
        queueTail = 0;
        enqueue(messageQueue, BROADCAST);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
                	memcpy(&paramsValues [index], &_ref_receiver, (1 * REACTIVE_CLASS_SIZE));
	                index += (1 * REACTIVE_CLASS_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_data, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_packetsNumber, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void WirelessMediumActor::_msg_broadcastingIsCompleted (byte senderId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, BROADCASTINGISCOMPLETED);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}









	byte** WirelessMediumActor::table = (byte**)0xFFFFFFFF;

	byte** WirelessMediumActor::getTable() {
		return table;
	}








	vector<TimeFrame>** WirelessMediumActor::hashtableTimeExtension = (vector<TimeFrame>**)0xFFFFFFFF;
	
	vector<TimeFrame>** WirelessMediumActor::getHashtableTimeExtension() {
		return hashtableTimeExtension;
	}








	    void WirelessMediumActor::_timed_msg_getStatus (byte senderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_getStatus (senderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void WirelessMediumActor::_timed_msg_broadcast (byte senderId
	        ,
    		CommunicationDeviceActor*
	         _ref_receiver
	        , 
    		int
	         _ref_data
	        , 
    		int
	         _ref_packetsNumber
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_broadcast (senderId
	        ,
            _ref_receiver
	        , 
            _ref_data
	        , 
            _ref_packetsNumber
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void WirelessMediumActor::_timed_msg_broadcastingIsCompleted (byte senderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_broadcastingIsCompleted (senderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	void WirelessMediumActor::setNow(TIME_TYPE now) {
	
		AbstractTimedActor::setNow(now);
	}	
	

	    long WirelessMediumActor::msgsrvgetStatus (
    ) {
    			long arrayIndexChecker = 0;CommunicationDeviceActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "getStatus";
		#endif

	#ifdef TTS
	#endif

		{
			(temp0=(dynamic_cast<CommunicationDeviceActor*>(_ref_sender)), assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 128"), temp0)->_timed_msg_receiveStatus(myID, (_ref_senderDevice!=null), _ref_now, MAX_TIME);
		}
		return 0;

}
	    long WirelessMediumActor::msgsrvbroadcast (
    		CommunicationDeviceActor*
	         _ref_receiver
	        , 
    		int
	         _ref_data
	        , 
    		int
	         _ref_packetsNumber
    ) {
    			long arrayIndexChecker = 0;WirelessMediumActor *temp0;CommunicationDeviceActor *temp1;CommunicationDeviceActor *temp2;CommunicationDeviceActor *temp3;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "broadcast";
		#endif

	#ifdef TTS
	#endif

		{
			if ((_ref_senderDevice==null)) {
				{
					(_ref_senderDevice=(dynamic_cast<CommunicationDeviceActor*>(_ref_sender)));
					(_ref_receiverDevice=_ref_receiver);
					(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 135"), temp0)->_timed_msg_broadcastingIsCompleted(myID, (_ref_packetsNumber*_ref_OnePacketTransmissionTime) + _ref_now, MAX_TIME);
					(temp1=(dynamic_cast<CommunicationDeviceActor*>(_ref_sender)), assertion(temp1!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 136"), temp1)->_timed_msg_receiveResult(myID, true, (_ref_packetsNumber*_ref_OnePacketTransmissionTime) + _ref_now, MAX_TIME);
					(temp2=_ref_receiverDevice, assertion(temp2!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 137"), temp2)->_timed_msg_receiveData(myID, _ref_receiver, _ref_data, _ref_packetsNumber, _ref_now, MAX_TIME);
				}
}
			else {
				{
					(temp3=(dynamic_cast<CommunicationDeviceActor*>(_ref_sender)), assertion(temp3!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 140"), temp3)->_timed_msg_receiveResult(myID, false, _ref_now, MAX_TIME);
				}
}
		}
		return 0;

}
	    long WirelessMediumActor::msgsrvbroadcastingIsCompleted (
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "broadcastingIsCompleted";
		#endif

	#ifdef TTS
	#endif

		{
			(_ref_senderDevice=null);
			(_ref_receiverDevice=null);
		}
		return 0;

}
