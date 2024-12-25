                                    

#include "CoreRebecaDFSHashmap.h"
#include <string.h>
#include <stdlib.h>

/*
	Suppose we have n processes, we need
	  2 flags if we use NDFS only
	  n+2 flags if we use fairness (extended state space) plus NDFS
	  one more for marking on-stack (used only for the stack of the first DFS)

	if we use partial order reduction, we need one more flag
		to indicate if current state had been fully expanded in 1st DFS
		It is also used in the 1st DFS to dinstinguish between the two phases of the loop over processes (the second phase is fully_expansion)
Summary:
  n = REBEC_COUNT
  we need n+1 bits for handling fairness ("visited" bit for each copy of the state-space)
  add 1 bit for NDFS
  we need 1 bit to flag "on_stack"
  ifdef PO, we need 1 bit to flag "fully expansion"
*/

void State::clearOnStack(){
    flag1 &= ~0x80; // n + 3
}

boolean State::isOnStack(){
    return flag1 & 0x80;   // n + 3
}

void State::setOnStack(){
		flag1 |= 0x80;   // n + 3
}

/*
Set the "counter"th bit, which may be in flag1, flag2, etc.
Since the flag bytes are adjunct, we use the following formula to find
the correct place of the requested bit.
Setting this bit means that this state is visited in state-space #counter.
*/
void State::visit(short int counter){
    (*(&flag1+(counter/8))) |= (1  <<  (counter%8));
}

/*
Is this state visited in state-space #counter?
*/
boolean State::isVisited(byte counter){
    return ((*(&flag1+(counter/8)))  >>  (counter%8)) & 1;
}

/*
The equality of two states depends only on "sys" and "claim" elements.
This function is used so that one state is never stored twice.
*/
boolean State::equals(long* sys, long claim){
    boolean cEq = (claim == this->claim);
    boolean sEq = !memcmp(sys, (long*)this, sizeof(long)*REBEC_COUNT);
    return cEq && sEq;
}

//-----------------------------------------------------------------------
// HashMap: Global state space
//-----------------------------------------------------------------------

CoreRebecaDFSHashmap::CoreRebecaDFSHashmap(int hashmapSize, ostream &outStream) : out(outStream) {
	this->hashmapSize = hashmapSize; 
    table = new State* [1 << hashmapSize];

    if(table == NULL) {
        throw Exception("Not enough memory for allocating state space hash table.");
    }
    
    memset (table, 0, (1 << hashmapSize)*sizeof(State*));
}

CoreRebecaDFSHashmap::~CoreRebecaDFSHashmap () {	//never called
    delete [] table;
}

 /*
  * The hash function is taken from SPIN, which is based on
  * the public domain hash function from Bob Jenkins, 1996.
  * see: http://www.burtleburtle.net/bob/
  * It is changed to fit the data structures.
  */

#define mix(a,b,c) \
{  a -= b; a -= c; a ^= (c >> 13); \
   b -= c; b -= a; b ^= (a << 8);  \
   c -= a; c -= b; c ^= (b >> 13); \
   a -= b; a -= c; a ^= (c >> 12); \
   b -= c; b -= a; b ^= (a << 16); \
   c -= a; c -= b; c ^= (b >> 5);  \
   a -= b; a -= c; a ^= (c >> 3);  \
   b -= c; b -= a; b ^= (a << 10); \
   c -= a; c -= b; c ^= (b >> 15); \
}

unsigned int CoreRebecaDFSHashmap::hashCode(void *v, unsigned long length, unsigned int mask) {
    unsigned long a, b, c, len = length;

    byte *k = (byte*)v;
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
    switch (len) {
        case 2: b+=k[1];
        case 1: a+=k[0];
    }
    mix(a,b,c);
    return c & mask;
}

/*
Check if a state identified by "sys" and "claim" is already stored.
*/
State* CoreRebecaDFSHashmap::exists (long* sys, long claim) {
    hashValue = hashCode(sys, (REBEC_COUNT)*sizeof(long), (1 << hashmapSize)-1);
    State* current = table[hashValue];
    if (current == null) {
        #ifdef DEBUG_LEVEL_2
    		out << "State does not exist in hash table." << endl;
   		#endif
    	return NULL;
    }
    while (current){	// use "buckets" for collision handling
        if (current->equals(sys, claim)) {
	        #ifdef DEBUG_LEVEL_2
	    		out << "State exists in one bucket of hash table. Going for checking the value of the claim." << endl;
	   		#endif        
			return current;
		}
        current = current->next;
    }
    #ifdef DEBUG_LEVEL_2
		out << "State does not exist in hash table (search in a filled bucket)." << endl;
	#endif
    return NULL;
}

/*
After we make sure the state identified by "sys" and "claim" is not
already stored (with exists()), this function is called to store the
state.
This function sets no flags for the given state, because in different
situations (like BFS) different flags must be set.
*/
State* CoreRebecaDFSHashmap::put (long* sys, long claim){
	// only called after exists(), so hashValue is already calculated properly
    State* current = table[hashValue];
    #ifdef VERBOSE
	    if (current) collision++;	// count the number of collisions
    #endif
    State *prev = null;
    while (current){	// move to the end of bucket
        prev = current;
        current = current->next;	// move in the bucket
    }
    current = new State;
    if(current == NULL)
        return NULL;
        
	memset (current, 0, sizeof(State));
    for (int i=0; i<REBEC_COUNT; i++)
        ((long*)current)[i] = sys[i];
    current->claim = claim;
    current->next = null;
    if (prev)
        prev->next = current;
    else
        table[hashValue] = current;
	return current;
}

/*
Both exists() and put() point "current" to the last state referenced.
*/
//State* CoreRebecaDFSHashmap::getState(){
//    return current;
//}
