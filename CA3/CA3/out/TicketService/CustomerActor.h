












#ifndef __CUSTOMER_ACTOR_H__
#define __CUSTOMER_ACTOR_H__

class AgentActor;
class TicketServiceActor;

#include "AbstractTimedActor.h"

class CustomerActor :
	public virtual AbstractTimedActor
{

public:
    CustomerActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize














    );
	~CustomerActor();
	
	long methodSelectorAndExecutor();
	
    char* getClassName();
    
	virtual void exportStateVariablesInXML(ostream &out, string tab);
	virtual void exportAMessageInXML(ostream &out, string tab, int cnt);

	virtual void exportInsideStateVariablesTagInXML(ostream &out, string tab);

    virtual void marshalActorToArray(byte* array);
    void marshalVariablesToArray(byte * array);


    virtual void unmarshalActorFromArray(byte* array);
    void unmarshalVariablesFromArray(byte * array);


    	byte _ref_id;

    	boolean _ref_sent;


		long constructorCustomer(byte creatorID
	        ,
    		byte
	         _ref_myId
		);

       virtual void _msg_try(byte senderId
        )
        ;
       virtual long msgsrvtry(
        )
        ;
       virtual void _msg_ticketIssued(byte senderId
        )
        ;
       virtual long msgsrvticketIssued(
        )
        ;








	protected:
	    static byte** table;
		virtual byte** getTable();







	protected:
	    static vector<TimeFrame>** hashtableTimeExtension;
		virtual vector<TimeFrame>** getHashtableTimeExtension();







public:
      	  void _timed_msg_try(byte senderId
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_ticketIssued(byte senderId
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
    virtual void setNow(TIME_TYPE now);
    
protected:
};

#endif