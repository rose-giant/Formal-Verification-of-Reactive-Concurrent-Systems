












#ifndef __AUTO_ACTOR_H__
#define __AUTO_ACTOR_H__

class ControllerActor;
class InfrastructureActor;
class MannedActor;

#include "AbstractActor.h"

class AutoActor :
	public virtual AbstractActor
{

public:
    AutoActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize
    );
	~AutoActor();
	
	long methodSelectorAndExecutor();
	
    char* getClassName();
    
	virtual void exportStateVariablesInXML(ostream &out, string tab);
	virtual void exportAMessageInXML(ostream &out, string tab, int cnt);

	virtual void exportInsideStateVariablesTagInXML(ostream &out, string tab);

    virtual void marshalActorToArray(byte* array);
    void marshalVariablesToArray(byte * array);


    virtual void unmarshalActorFromArray(byte* array);
    void unmarshalVariablesFromArray(byte * array);


    	boolean _ref_shouldStop;

    	boolean _ref_line;

    	int _ref_id;

    	int _ref_block;

    	boolean _ref_up;

    	boolean _ref_asked;

    	boolean _ref_askedCtrl;

    	boolean _ref_firstStep;

    	boolean _ref_lastStep;


		long constructorAuto(byte creatorID
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








	protected:
	    static byte** table;
		virtual byte** getTable();
protected:
    	void _synchmethod_initVars(
        )
        ;
    	void _synchmethod_move(
        )
        ;
};

#endif