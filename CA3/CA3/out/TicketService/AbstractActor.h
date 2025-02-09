

#ifndef __ABSTRACT_ACTOR_H__
#define __ABSTRACT_ACTOR_H__

#include "Types.h"
#include "Config.h"
#include <iostream>
#include <vector>
#include <array>
#include <cmath>
using namespace std;

string to_string(int number);

#define EMPTY		0
#define INITIAL	1

// includes self & one extra for -1
#define MAX_KNOWN (2 + 1)


#define MAX_ACTIONS 2










class AbstractActor {

public:

    AbstractActor(byte myID, char * myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		, int stateSize, int hashmapSize 
    );
    virtual ~AbstractActor();

    virtual long execute ();
	virtual void preMessageExecutionInitialization();
	virtual long methodSelectorAndExecutor() = 0;
	virtual void exportStateInXML(ostream &out, string tab);
	virtual void exportQueueContentInXML(ostream &out, string tab);
	virtual void exportStateVariablesInXML(ostream &out, string tab) = 0;
	virtual void exportAMessageInXML(ostream &out, string tab, int cnt) = 0;
    virtual char* getClassName() = 0;
    
    virtual void marshalActorToArray(byte* array) = 0;
    virtual void unmarshalActorFromArray(byte* array) = 0;

    char* getName() const;
    char* activeAction() const;					// the action at queue head

    byte myID;
    char* myName;
    byte *known;	// list of indexes of known rebecs

    // message queue is made public to be accessible by areActiveActionsSafe()
    byte* messageQueue;
    byte* senderQueue;
    
    char ** messageNames;
    int *messagePriorities;

    byte** paramQueue;
    int maxParamLength;
    int maxQueueLength;
    int shift;
    
    int priority;
    virtual int getPriority();
    void setPriority(int priority);








	public:
	    long marshalActor();
	    void unmarshalActor(long stateId);
	    long storeLocalState(byte* currentValues);
	    
	    virtual long consumedMemory();
	
	    unsigned int hashCode (byte *v);
	
	protected:
	    int hashmapSize;
	    int stateSize;
	    virtual byte** getTable() = 0;
    
protected:
	const static byte coeff[];
    int queueTail;
    vector<AbstractActor*>& rebecs;

    void enqueue(byte *queue, byte message);
    void enqueue(byte **queue, byte *message, int size);

    //void _synchmethod_assertion (/*string message*/ byte id, boolean condition);
    void assertion (boolean condition, string message);
    void assertion (boolean condition);
    
    ArrayList<AbstractActor*>& getAllActors();
    
	#ifdef DEBUG_LEVEL_2 
		ostream &analysisOutputStream;
	#endif

    #ifdef NO_ATOMIC_EXECUTION
    	int __pc;
    #endif
};

#endif