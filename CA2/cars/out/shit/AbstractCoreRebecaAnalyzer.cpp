#include "AbstractCoreRebecaAnalyzer.h"

		#define _ref_infra (dynamic_cast<InfrastructureActor*>(rebecs[0]))
		#define _ref_ctrl (dynamic_cast<ControllerActor*>(rebecs[1]))
		#define _ref_auto1 (dynamic_cast<AutoActor*>(rebecs[2]))
		#define _ref_manned1 (dynamic_cast<MannedActor*>(rebecs[3]))
	
		boolean AbstractCoreRebecaAnalyzer::_propDef_finished1 () {
			return _ref_auto1->_ref_lastStep;
		}
		boolean AbstractCoreRebecaAnalyzer::_propDef_isSafetyViolated () {
			return _ref_ctrl->_ref_safetyCheck;
		}
	
		#define _ref_finished1 _propDef_finished1()
		#define _ref_isSafetyViolated _propDef_isSafetyViolated()
		#undef _ref_finished1
		#undef _ref_isSafetyViolated

		#undef _ref_infra
		#undef _ref_ctrl
		#undef _ref_auto1
		#undef _ref_manned1

AbstractCoreRebecaAnalyzer::AbstractCoreRebecaAnalyzer(ostream &outStream, int hashmapSize) : analysisOutputStream(outStream){
	this->hashmapSize = hashmapSize;
	this->numberOfTransitions = 0;
	this->numberOfStates = 0;
}

void AbstractCoreRebecaAnalyzer::unmarshalRebecs(long* node) {
	int i;
	for (i = 0; i < REBEC_COUNT; i++) {
        rebecs[i]->unmarshalActor(node[i]);   // uncompress all states of all rebecs
    }
}

void AbstractCoreRebecaAnalyzer::marshalRebecs(long* node) {
	int i;
	for (i=0; i<REBEC_COUNT; i++){
		node[i] = rebecs[i]->marshalActor();
	}
}

State* AbstractCoreRebecaAnalyzer::storeRecentlyCreatedState (byte &result, short int claim, short int fairnessCounter) {

    numberOfTransitions++;
    State *st;
    long sys[REBEC_COUNT];
	marshalRebecs(sys);
	result = NEW_STATE;
	if (st = storage->exists (sys, claim)){
		if (st->isOnStack()){
			result |= ON_STACK;	// used in detecting cycle & stack proviso
		}
		if (st->isVisited(fairnessCounter)){
			result &= ~NEW_STATE;
		}
	}
	else
	{
		st = storage->put(sys, claim);	//create the new State
		if (st == NULL) {
	        throw Exception("Not enough memory for continuing the state space generation.");
		}
	    st->stateID = numberOfStates++;
	    #ifdef DEBUG_LEVEL_2
			cout << "State stored successfully with id: " << st->stateID << "." << endl;
		#endif
	}
	return st;
}

void AbstractCoreRebecaAnalyzer::instantiationPart() {
	vector<AbstractActor*>* referenceToRebecs = new vector<AbstractActor*>();
	byte maxRebecID = 0;
    	
    	InfrastructureActor* _ref_infra = new InfrastructureActor(maxRebecID++, "infra", 6, 9, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 226
    											, hashmapSize
    											);
        rebecs.push_back(_ref_infra);
        referenceToRebecs->push_back(_ref_infra);
    	
    	ControllerActor* _ref_ctrl = new ControllerActor(maxRebecID++, "ctrl", 6, 9, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 152
    											, hashmapSize
    											);
        rebecs.push_back(_ref_ctrl);
        referenceToRebecs->push_back(_ref_ctrl);
    	
    	AutoActor* _ref_auto1 = new AutoActor(maxRebecID++, "auto1", 3, 1, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 18
    											, hashmapSize
    											);
        rebecs.push_back(_ref_auto1);
        referenceToRebecs->push_back(_ref_auto1);
    	
    	MannedActor* _ref_manned1 = new MannedActor(maxRebecID++, "manned1", 3, 1, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 18
    											, hashmapSize
    											);
        rebecs.push_back(_ref_manned1);
        referenceToRebecs->push_back(_ref_manned1);

    	_ref_infra->known = new byte[MAX_KNOWN];
    	_ref_infra->known[0] = _ref_infra->myID;
 _ref_infra->known[1] = _ref_ctrl->myID;

		_ref_infra->constructorInfrastructure(INITIAL_SENDER
		);
    	_ref_ctrl->known = new byte[MAX_KNOWN];
    	_ref_ctrl->known[0] = _ref_ctrl->myID;

		_ref_ctrl->constructorController(INITIAL_SENDER
		);
    	_ref_auto1->known = new byte[MAX_KNOWN];
    	_ref_auto1->known[0] = _ref_auto1->myID;
 _ref_auto1->known[1] = _ref_infra->myID;

		_ref_auto1->constructorAuto(INITIAL_SENDER
	        , 0
		);
    	_ref_manned1->known = new byte[MAX_KNOWN];
    	_ref_manned1->known[0] = _ref_manned1->myID;
 _ref_manned1->known[1] = _ref_infra->myID;

		_ref_manned1->constructorManned(INITIAL_SENDER
	        , 0
		);

	numberOfStates = 0;
	numberOfTransitions = 0;

    #ifdef DEBUG
    	analysisOutputStream << "Rebecs are instanciated successfully." << endl;
    #endif

}

void AbstractCoreRebecaAnalyzer::exportState(State* current, ostream& outStream) {
	outStream << "<state id=\"" << current->stateID << "\" "; 
	outStream << "atomicpropositions=\""; 
		if (_propDef_finished1 ())
			outStream << "finished1,"; 
		if (_propDef_isSafetyViolated ())
			outStream << "isSafetyViolated,"; 
	outStream << "\" ";
	outStream << ">" << endl;
	for (int cnt = 0; cnt < REBEC_COUNT; cnt++)
		rebecs[cnt]->exportStateInXML(outStream, "\t");
	outStream << "</state>" << endl;
}

void AbstractCoreRebecaAnalyzer::exportTransition(State* source, State* destination, 
		string sender, string owner, string label, ostream& outStream) {
	exportTransition(to_string(source->stateID), to_string(destination->stateID), sender, owner, label, outStream);  
}

void AbstractCoreRebecaAnalyzer::exportTransition(string source, string destination, 
		string sender, string owner, string label, ostream& outStream) {
    outStream << "<transition source=\"" << source << "\" destination=\"" << 
		destination << "\"> <messageserver sender=\"" << sender << 
		"\" owner=\"" << owner << "\" title=\"" << label << "\"/></transition>" << endl;
}
