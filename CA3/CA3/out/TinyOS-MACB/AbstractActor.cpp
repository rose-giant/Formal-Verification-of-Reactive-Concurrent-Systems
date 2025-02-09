                                                

#include "AbstractActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

string to_string(int number){
    string number_string = "";
    char ones_char;
    int ones = 0;
    while(true){
        ones = number % 10;
        switch(ones){
            case 0: ones_char = '0'; break;
            case 1: ones_char = '1'; break;
            case 2: ones_char = '2'; break;
            case 3: ones_char = '3'; break;
            case 4: ones_char = '4'; break;
            case 5: ones_char = '5'; break;
            case 6: ones_char = '6'; break;
            case 7: ones_char = '7'; break;
            case 8: ones_char = '8'; break;
            case 9: ones_char = '9'; break;
            //default : ErrorHandling("Trouble converting number to string.");
        }
        number -= ones;
        number_string = ones_char + number_string;
        if(number == 0){
            break;
        }
        number = number/10;
    }
    return number_string;
}


const byte AbstractActor::coeff[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, static_cast<char>(0x80)};

AbstractActor::AbstractActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		, int stateSize, int hashmapSize 
	) : rebecs(rebecsRef) 
		#ifdef DEBUG_LEVEL_2 
			, analysisOutputStream(out)
		#endif
	{
    this->myID = myID;
	this->myName = myName;
	this->maxQueueLength = maxQueueLength;
	this->maxParamLength = maxParamLength;
	this->priority = std::numeric_limits<int>::max();
	shift = 0;
    messageQueue = new byte[maxQueueLength];
	memset (messageQueue, 0, sizeof(byte) * maxQueueLength);
    senderQueue  = new byte[maxQueueLength];
	memset (senderQueue, 0, sizeof(byte) * maxQueueLength);
    paramQueue = new byte*[maxQueueLength];
    memset (paramQueue, 0, sizeof(byte) * maxQueueLength);
    if(maxParamLength != 0)
        for (int i = 0; i < maxQueueLength; i++) {
            paramQueue[i] = new byte[maxParamLength];
            memset (paramQueue[i],  0, sizeof (byte)* (maxParamLength));
        }
    







	this->stateSize = stateSize;
	this->hashmapSize = hashmapSize;
}

AbstractActor::~AbstractActor(){
}

char* AbstractActor::activeAction() const {
	return messageNames[messageQueue[0]];
}

void AbstractActor::enqueue(byte* queue, byte message){
	while (queueTail < maxQueueLength - 1 && queue[queueTail]) {
        queueTail++;
    }
	if (queue[queueTail]) {
		throw QueueOverflowException(string("Queue overflow in ") + string(myName), this);
	}
	else {
		queue[queueTail] = message;
	}
}

void AbstractActor::enqueue(byte **queue, byte *message, int size){
    for (int cnt = 0; cnt < size; cnt++) {
        queue[queueTail][cnt] = message[cnt];
    }
}

char* AbstractActor::getName() const {
    return myName;
}

void AbstractActor::assertion(boolean condition) {
	if (!condition)
		throw AssertionFailedException("no message", this);
}

void AbstractActor::assertion(boolean condition, string message) {
	if (!condition)
		throw AssertionFailedException(message, this);
}

 ArrayList<AbstractActor*>& AbstractActor::getAllActors() {
 
 	return rebecs;
 }


long AbstractActor::execute() {
 	
 	preMessageExecutionInitialization();
	return methodSelectorAndExecutor();
	
	}
void AbstractActor::preMessageExecutionInitialization() {

}









	long AbstractActor::consumedMemory(){
		return 0;
	}
	
	unsigned int AbstractActor::hashCode (byte *v) {
		unsigned long c;
	    if (stateSize < 8) { // 8 is sizeof(c)=sizeof(long)
		    c = * (unsigned long*) v;
	    }
	    else {
	        unsigned long a, b, len, length;
	
	        length = len = stateSize;
	        byte *k = v;
	        a = b = 0x9e3779b9;
	        c = 0;
	        while (len >= 3)
	        {	a += k[0];
	            b += k[1];
	            c += k[2];
	            mix(a,b,c);
	            k += 3; len -= 3;
	        }
	        c += length;
	        // If len equals to 2, both cases, 2 and 1, are executed
	        // If len equals to 1, just the second case (1) will be executed
	        switch (len) {
	            case 2: b+=k[1];
	            case 1: a+=k[0];
	        }
	        mix(a,b,c);
	    }
		return c & ((1 << hashmapSize)-1);
	}
	
	long AbstractActor::marshalActor() {
		byte currentValues[stateSize];
		#ifndef __LP64__
			memset(currentValues, 0, stateSize);
		#endif
		marshalActorToArray(currentValues);
		#ifdef DEBUG_LEVEL_2
			analysisOutputStream << "Rebec " << myName << " is marshaled to array [";
			for (int cnt = 0; cnt < stateSize; cnt++)
				analysisOutputStream << (int)currentValues[cnt] << ",";
			analysisOutputStream << "]" << endl;
		#endif
	    return storeLocalState(currentValues);
	}
	
	void AbstractActor::unmarshalActor(long stateId) {
	    byte* currentValues = getTable()[stateId];
		#ifdef DEBUG_LEVEL_2
			analysisOutputStream << "Rebec " << myName << " is unmarshalled from array [";
			for (int cnt = 0; cnt < stateSize; cnt++)
				analysisOutputStream << (int)currentValues[cnt] << ",";
			analysisOutputStream << "]" << endl;
		#endif
		unmarshalActorFromArray(currentValues);
	}
	
	long AbstractActor::storeLocalState(byte* currentValues) {
		// get Number by State
	    unsigned long h, stateNum;
		h = stateNum = hashCode(currentValues);
		byte **table = getTable();
	next:
		if (table[stateNum] == null) {
			table[stateNum] = new byte [stateSize];
			if (getTable()[stateNum] == NULL) {
	            throw HeapOverflowException("Cannot store rebec's local state." );
			}
			memcpy (table[stateNum], currentValues, sizeof(byte) * stateSize);
	        return stateNum;
		}
		if (!memcmp (table[stateNum], currentValues, sizeof(byte) * stateSize)) {
	        return stateNum;
	    }
	
		// Collision has occurred
		// Check next slot (linear probing) for collision handling
		stateNum = (stateNum < (1 << hashmapSize)-1)? stateNum+1 : 0; // may wrap arond
		if (stateNum == h) {
	        throw Exception("Hash table of reactive class is full.");
		}
		goto next;	// collision
	}

void AbstractActor::exportQueueContentInXML(ostream &out, string tab) {
	out << tab << "<queue>" << endl;
    for (int cnt = (shift ? 1 : 0); cnt < maxQueueLength && messageQueue[cnt] != 0; cnt++) {
        out << tab << '\t' << "<message sender=\"" << rebecs[senderQueue[cnt]]->getName()<< "\">";
        exportAMessageInXML(out, tab, cnt);
        out << "</message>" << endl;

    }        
	out << tab << "</queue>" << endl;	
}

void AbstractActor::exportStateInXML(ostream &out, string tab) {
	out << tab << "<rebec name=\"" << myName << "\">" << endl;
	exportStateVariablesInXML(out, tab + "\t");
	exportQueueContentInXML(out, tab + "\t");	
	out << tab << "</rebec>" << endl;
}

int AbstractActor::getPriority() {
	return priority;
}

void AbstractActor::setPriority(int priority) {
	this->priority = priority;
}
