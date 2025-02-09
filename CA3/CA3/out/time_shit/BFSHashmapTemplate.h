

#ifndef __BFS_HASHMAP_TEMPLATE_H__
#define __BFS_HASHMAP_TEMPLATE_H__

#include "Types.h"
#include "Config.h"
#include <ostream>
#include <fstream>
#include <list>

using namespace std;

template <class T, class P> class BFSState {
public:
    T rebsIndex[REBEC_COUNT];
    long stateID;

    list<P> parents;

    boolean equals(BFSState<T,P>& current) {
	    return !memcmp(current.rebsIndex, this->rebsIndex, sizeof(T) * REBEC_COUNT);
	}
};

template <class T, class P> class BFSHashmapTemplate {
protected:
    list<T*>** table;
    long hashValue;
    T* current;
    ostream &out;
    int hashmapSize;
    int numberOfElements;
public:
    BFSHashmapTemplate(int hashmapSize, ostream &outStream)  : out(outStream){
		this->hashmapSize = hashmapSize;
	    table = new list<T*>* [1 << hashmapSize];
	
	    if(table == NULL) {
	        throw Exception("Not enough memory for allocating state space hash table.");
	    }
	
		numberOfElements = 0;
	    memset (table, 0, (1 << hashmapSize)*sizeof(list<T*>*));
	}
    virtual ~BFSHashmapTemplate() {
    	delete [] table;
	}

    static unsigned int hashCode(void *v, unsigned long length, unsigned int mask) {
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
    
	unsigned int getHashCode(T& state)  {
		return hashCode(state.rebsIndex, (sizeof(state.rebsIndex[0]) * REBEC_COUNT), (1 << hashmapSize)-1);
	}
	/**
	 * Before putting a new state in the hashtable, the "exists" method should be called
	 *  to calculate and set a hash value for the new state.
	 */
    T* put(T& state, T* parent, int executedRebecIndex, int level) {
		// only called after exists(), so hashValue is already calculated properly
	
	    T* current = new T(state);
	    if(current == NULL)
	        throw Exception("Not enough memory to insert new item in the hash table of state space.");
	
	    P pr;
	    pr.parent = parent;
	    pr.executedRebecIndex = executedRebecIndex;
	    current->parents.push_back(pr);
	
		table[hashValue]->push_back(current);
		numberOfElements++;
		return current;
	}
    
    T* exists (T& state) {
	    hashValue = getHashCode(state);
	    if(!table[hashValue])
	    	table[hashValue] = new list<T*>();
	    for (typename list<T*>::iterator it = table[hashValue]->begin(); it != table[hashValue]->end(); it++) {
	        if ((*it)->equals(state)) {
	  	        #ifdef DEBUG_LEVEL_2
		    		out << "State exists in one bucket of hash table. Going for checking the value of claim." << endl;
		   		#endif        
	        	return *it;
	        }
	    }
	    #ifdef DEBUG_LEVEL_2
			out << "State does not exist in hash table." << endl;
		#endif
		return NULL;
	}
};

#endif