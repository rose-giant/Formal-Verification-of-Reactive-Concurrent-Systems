












#ifndef __CONTROLLER_ACTOR_H__
#define __CONTROLLER_ACTOR_H__

class InfrastructureActor;
class AutoActor;
class MannedActor;

#include "AbstractActor.h"

class ControllerActor :
	public virtual AbstractActor
{

public:
    ControllerActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize
    );
	~ControllerActor();
	
	long methodSelectorAndExecutor();
	
    char* getClassName();
    
	virtual void exportStateVariablesInXML(ostream &out, string tab);
	virtual void exportAMessageInXML(ostream &out, string tab, int cnt);

	virtual void exportInsideStateVariablesTagInXML(ostream &out, string tab);

    virtual void marshalActorToArray(byte* array);
    void marshalVariablesToArray(byte * array);


    virtual void unmarshalActorFromArray(byte* array);
    void unmarshalVariablesFromArray(byte * array);


    	std::array<boolean, 10> _ref_shouldStop;

    	std::array<boolean, 10> _ref_shouldChangeLine;

    	std::array<boolean, 10> _ref_autoLine;

    	std::array<boolean, 10> _ref_mannedLine;

    	boolean _ref_answeredAuto;

    	boolean _ref_answeredManned;

    	std::array<int, 10> _ref_autoBlock;

    	std::array<int, 10> _ref_mannedBlock;

    	boolean _ref_lastWasAuto;

    	boolean _ref_safetyCheck;


		long constructorController(byte creatorID
		);

       virtual void _msg_isAllowed(byte senderId
	        ,
    		boolean
	         _ref_line
	        , 
    		int
	         _ref_block
	        , 
    		int
	         _ref_id
	        , 
    		boolean
	         _ref_isAuto
        )
        ;
       virtual long msgsrvisAllowed(
    		boolean
	         _ref_line
	        , 
    		int
	         _ref_block
	        , 
    		int
	         _ref_id
	        , 
    		boolean
	         _ref_isAuto
        )
        ;








	protected:
	    static byte** table;
		virtual byte** getTable();
protected:
    	void _synchmethod_initVars(
        )
        ;
    	void _synchmethod_check(
        )
        ;
};

#endif