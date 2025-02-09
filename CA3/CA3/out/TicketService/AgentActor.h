












#ifndef __AGENT_ACTOR_H__
#define __AGENT_ACTOR_H__

class CustomerActor;
class TicketServiceActor;

#include "AbstractTimedActor.h"

class AgentActor :
	public virtual AbstractTimedActor
{

public:
    AgentActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize














    );
	~AgentActor();
	
	long methodSelectorAndExecutor();
	
    char* getClassName();
    
	virtual void exportStateVariablesInXML(ostream &out, string tab);
	virtual void exportAMessageInXML(ostream &out, string tab, int cnt);

	virtual void exportInsideStateVariablesTagInXML(ostream &out, string tab);

    virtual void marshalActorToArray(byte* array);
    void marshalVariablesToArray(byte * array);


    virtual void unmarshalActorFromArray(byte* array);
    void unmarshalVariablesFromArray(byte * array);



		long constructorAgent(byte creatorID
		);

       virtual void _msg_requestTicket(byte senderId
        )
        ;
       virtual long msgsrvrequestTicket(
        )
        ;
       virtual void _msg_ticketIssued(byte senderId
	        ,
    		CustomerActor*
	         _ref_customer
        )
        ;
       virtual long msgsrvticketIssued(
    		CustomerActor*
	         _ref_customer
        )
        ;








	protected:
	    static byte** table;
		virtual byte** getTable();







	protected:
	    static vector<TimeFrame>** hashtableTimeExtension;
		virtual vector<TimeFrame>** getHashtableTimeExtension();







public:
      	  void _timed_msg_requestTicket(byte senderId
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_ticketIssued(byte senderId
	        ,
    		CustomerActor*
	         _ref_customer
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
    virtual void setNow(TIME_TYPE now);
    
protected:
};

#endif