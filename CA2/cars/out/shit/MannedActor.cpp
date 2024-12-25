                                    


#include "Types.h"
#include "MannedActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

#include "ControllerActor.h"
#include "InfrastructureActor.h"
#include "AutoActor.h"

// location counters (values stored in messageQueue[0])
/* There can be more than one location counter for each msgsrv when we
   want to have non-atomic message servers.
   Notice that values 0 and 1 are reserved for EMPTY and INITIAL, respectively.*/
#define CANMOVE 1
#define LIVE 2
#define RANDOMMOVE 3


// knownrebecs (those to whom message can be sent)
// the following format allows to use them as variables --> the ID of the rebec
#define _ref_sender (rebecs[senderQueue[0]])
#define _ref_self (dynamic_cast<MannedActor*>(rebecs[(int)((unsigned char)myID)]))

    	#define _ref_infra (dynamic_cast<InfrastructureActor*>(rebecs[(int)((unsigned char)known[1])]))
    
MannedActor::MannedActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize
)
	:
		AbstractActor (myID, myName, maxQueueLength, maxParamLength, rebecsRef
			#ifdef DEBUG_LEVEL_2 
				, out
			#endif







		, stateSize, hashmapSize
		)
{
    messageNames = new char *[3 + 1];
    messagePriorities = new int[3 + 1];
	messageNames[0] = (char*)"Empty";

	
		
	
		
       		 messageNames[1] = (char*)"CANMOVE";
			messagePriorities[1] = std::numeric_limits<int>::max();
       		 messageNames[2] = (char*)"LIVE";
			messagePriorities[2] = std::numeric_limits<int>::max();
       		 messageNames[3] = (char*)"RANDOMMOVE";
			messagePriorities[3] = std::numeric_limits<int>::max();








	if (table == (byte**)0xFFFFFFFF) {	// avoid reinitialization
		table = new byte* [1 << hashmapSize];
        memset (table, 0, sizeof(byte*)*(1 << hashmapSize));
	}

	
	queueTail = 0;
}

MannedActor::~MannedActor (){
	delete [] messageNames;
	delete [] messageQueue;
	delete [] senderQueue;
}


void MannedActor::exportAMessageInXML(ostream &out, string tab, int cnt) {
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
            case RANDOMMOVE: {
                out << "randomMove("
                << ")";
                break;
               }
    	}
}


void MannedActor::exportStateVariablesInXML(ostream &out, string tab) {
	out << tab << "<statevariables>" << endl;
	exportInsideStateVariablesTagInXML(out, tab);
	out << tab << "</statevariables>" << endl;
}

void MannedActor::exportInsideStateVariablesTagInXML(ostream &out, string tab){


			    	out << tab << '\t' << "<variable name=\"Manned.id\" type=\"int\">" <<  
			    		((int)_ref_id) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Manned.shouldChange\" type=\"boolean\">" <<  
			    		(_ref_shouldChange? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Manned.line\" type=\"boolean\">" <<  
			    		(_ref_line? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Manned.block\" type=\"int\">" <<  
			    		((int)_ref_block) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Manned.up\" type=\"boolean\">" <<  
			    		(_ref_up? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Manned.asked\" type=\"boolean\">" <<  
			    		(_ref_asked? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Manned.askedCtrl\" type=\"boolean\">" <<  
			    		(_ref_askedCtrl? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Manned.firstStep\" type=\"boolean\">" <<  
			    		(_ref_firstStep? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Manned.randomMoved\" type=\"boolean\">" <<  
			    		(_ref_randomMoved? "true" : "false") << "</variable>" << endl;
}


char* MannedActor::getClassName() {
    return (char*)"Manned";
}
void MannedActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    	
            
                	memcpy(&array [marshalIndex], &_ref_id, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            
            
            
                	memcpy(&array [marshalIndex], &_ref_block, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            
            
            
            
            

array [marshalIndex] = 0;
array [marshalIndex] |= (_ref_shouldChange ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
array [marshalIndex] |= (_ref_line ? 0xFF : 0x00) & AbstractActor::coeff[1]; 
array [marshalIndex] |= (_ref_up ? 0xFF : 0x00) & AbstractActor::coeff[2]; 
array [marshalIndex] |= (_ref_asked ? 0xFF : 0x00) & AbstractActor::coeff[3]; 
array [marshalIndex] |= (_ref_askedCtrl ? 0xFF : 0x00) & AbstractActor::coeff[4]; 
array [marshalIndex] |= (_ref_firstStep ? 0xFF : 0x00) & AbstractActor::coeff[5]; 
array [marshalIndex] |= (_ref_randomMoved ? 0xFF : 0x00) & AbstractActor::coeff[6]; 
++marshalIndex;    
	
}
void MannedActor::marshalActorToArray(byte* array) {
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

void MannedActor::unmarshalActorFromArray(byte* array) {

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

void MannedActor::unmarshalVariablesFromArray(byte * array){

	int index = 0;

            
            
                	memcpy(&_ref_id, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            
            

            
            

            
            
                	memcpy(&_ref_block, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            
            

            
            

            
            

            
            

            
            
_ref_shouldChange = (bool) (array [index] & AbstractActor::coeff[0]);
_ref_line = (bool) (array [index] & AbstractActor::coeff[1]);
_ref_up = (bool) (array [index] & AbstractActor::coeff[2]);
_ref_asked = (bool) (array [index] & AbstractActor::coeff[3]);
_ref_askedCtrl = (bool) (array [index] & AbstractActor::coeff[4]);
_ref_firstStep = (bool) (array [index] & AbstractActor::coeff[5]);
_ref_randomMoved = (bool) (array [index] & AbstractActor::coeff[6]);
++index;    
	
}

/*#ifdef DEBUG
void MannedActor::printStateContent(long stateNo) {
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
long MannedActor::methodSelectorAndExecutor() {

	
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
            case RANDOMMOVE:
                {
                    int i = 0;
                    return msgsrvrandomMove(
                    );
                }
    }
	return -1;
}


    long MannedActor::constructorManned (byte creatorId 
	        ,
    		int
	         _ref_inputId
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "Manned";
		#endif

		{
			(_ref_id=_ref_inputId);
			_synchmethod_initVars();
			_synchmethod_move();
		}
		shift = 0;
		return 0;

	}

 
		void MannedActor::_synchmethod_initVars (
   		 ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "initVars";
		#endif

		{
			(_ref_line=false);
			(_ref_block=1);
			(_ref_asked=false);
			(_ref_firstStep=true);
			(_ref_up=true);
		}

		}
 
		void MannedActor::_synchmethod_move (
   		 ) {
    			long arrayIndexChecker = 0;InfrastructureActor *temp0;MannedActor *temp1;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "move";
		#endif

		{
			if ((_ref_asked||_ref_firstStep)) {
				{
					(_ref_firstStep=false);
					(temp0=_ref_infra, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 284"), temp0)->_msg_isAllowed(myID, _ref_line, _ref_block, _ref_id, false);
				}
}
			(temp1=_ref_self, assertion(temp1!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 287"), temp1)->_msg_live(myID);
		}

		}


		void MannedActor::_msg_canMove (byte senderId
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

		void MannedActor::_msg_live (byte senderId
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

		void MannedActor::_msg_randomMove (byte senderId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, RANDOMMOVE);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}









	byte** MannedActor::table = (byte**)0xFFFFFFFF;

	byte** MannedActor::getTable() {
		return table;
	}

	    long MannedActor::msgsrvcanMove (
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

		{
			if ((!_ref_isAuto)) {
				{
					(_ref_shouldChange=_ref_canMove);
					(_ref_asked=true);
					if ((_ref_up&&(_ref_block==_ref_MAX_BLOCK_NUM))) {
						{
							(_ref_block--);
							(_ref_up=(!_ref_up));
						}
}
					if (((!_ref_up)&&(_ref_block==0))) {
						{
							(_ref_up=(!_ref_up));
							(_ref_block++);
						}
}
					if (((_ref_up&&(_ref_block<_ref_MAX_BLOCK_NUM))&&(_ref_block>0))) {
						{
							(_ref_block++);
						}
}
					if ((((!_ref_up)&&(_ref_block<_ref_MAX_BLOCK_NUM))&&(_ref_block>0))) {
						{
							(_ref_block--);
						}
}
					(_ref_asked=false);
				}
}
		}
		return 0;

}
	    long MannedActor::msgsrvlive (
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "live";
		#endif

		{
			_synchmethod_move();
		}
		return 0;

}
	    long MannedActor::msgsrvrandomMove (
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "randomMove";
		#endif
		long __tNumber = 0;
		static boolean nonDetVariable0 = true;

		{
			(_ref_up=(__tNumber|=1, nonDetVariable0));
			(_ref_randomMoved=true);
		}
		if (__tNumber & 1) {
			if (false){/*Dummy 'if' statement to create nested else-if easily!*/}
			else if(nonDetVariable0 == true) {
				nonDetVariable0 = false;
			}
			else if(nonDetVariable0 == false) {
				nonDetVariable0 = true;
				__tNumber &= ~1;
			}
			if (__tNumber & 1) {
				return __tNumber;
			}
		}
		return 0;

}
