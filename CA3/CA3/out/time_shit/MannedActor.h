












#ifndef __MANNED_ACTOR_H__
#define __MANNED_ACTOR_H__

class ControllerActor;
class InfrastructureActor;
class AutoActor;

#include "AbstractTimedActor.h"

class MannedActor :
	public virtual AbstractTimedActor
{

public:
    MannedActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize














    );
	~MannedActor();
	
	long methodSelectorAndExecutor();
	
    char* getClassName();
    
	virtual void exportStateVariablesInXML(ostream &out, string tab);
	virtual void exportAMessageInXML(ostream &out, string tab, int cnt);

	virtual void exportInsideStateVariablesTagInXML(ostream &out, string tab);

    virtual void marshalActorToArray(byte* array);
    void marshalVariablesToArray(byte * array);


    virtual void unmarshalActorFromArray(byte* array);
    void unmarshalVariablesFromArray(byte * array);


    	int _ref_id;

    	boolean _ref_shouldChange;

    	boolean _ref_line;

    	int _ref_block;

    	boolean _ref_up;

    	boolean _ref_asked;

    	boolean _ref_askedCtrl;

    	boolean _ref_firstStep;

    	boolean _ref_randomMoved;


		long constructorManned(byte creatorID
	        ,
    		int
	         _ref_inputId
		);

       virtual void _msg_canMove(byte senderId
	        ,
    		boolean
	         _ref_canMove
	        , 
    		boolean
	         _ref_isAuto
        )
        ;
       virtual long msgsrvcanMove(
    		boolean
	         _ref_canMove
	        , 
    		boolean
	         _ref_isAuto
        )
        ;
       virtual void _msg_live(byte senderId
        )
        ;
       virtual long msgsrvlive(
        )
        ;
       virtual void _msg_randomMove(byte senderId
        )
        ;
       virtual long msgsrvrandomMove(
        )
        ;








	protected:
	    static byte** table;
		virtual byte** getTable();







	protected:
	    static vector<TimeFrame>** hashtableTimeExtension;
		virtual vector<TimeFrame>** getHashtableTimeExtension();







public:
      	  void _timed_msg_canMove(byte senderId
	        ,
    		boolean
	         _ref_canMove
	        , 
    		boolean
	         _ref_isAuto
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_live(byte senderId
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_randomMove(byte senderId
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
    virtual void setNow(TIME_TYPE now);
    
protected:
    	void _synchmethod_initVars(
        )
        ;
    	void _synchmethod_move(
        )
        ;
};

#endif