/*
* DFA: creates a static DFA of size n. Note that all states in dfa must be in
* a numerical order from 0 to (n - 1).
*
* Authors:
* Buster Hultgren Warn <dv17bhn@cs.umu.se>
* Victor Liljeholm <dv13vlm@cs.umu.se>
*
* Final build: 2018-03-13
*/


#ifndef DFAMGENERATOR
#define DFAMGENERATOR

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int bool;
#define true 1
#define false 0


typedef struct state {

    int stateNr;
	char *stateName;
    bool acceptable;
    struct path *paths;
} state;

typedef struct path {

	char *key;
	struct path* nextPath;
	struct state *destination;
} path;

typedef struct dfa {

    int capacity;
	int size;
    struct state **allStates;
    struct state *startState;
    struct state *currState;
} dfa;

/*
* description: Allocates memory for an dfa and Creates an empty dfa.
* return: Empty dfa.
*/
dfa *dfaEmpty ();

/*
* description: Checks if the dfa is empty.
* param[in]: dfa - A pointer to the dfa.
* return: bool depending on if the dfa is empty or not.
*/
bool dfaIsEmpty(dfa *dfa);

/*
* description: Allocates memory for all possible states.
* param[in]: dfa - A pointer to the dfa.
* param[in]: capacity - the number of possible states.
*/
void dfaSetStates (dfa *dfa, int capacity);

/*
* description: Sets which state is the startState in the dfa.
* param[in]: dfa - A pointer to the dfa.
* param[in]: startState - the state to be set to startState.
*/
void dfaSetStart (dfa *dfa, char *stateName);

/*
* description: Inserts a state to the dfa and sets stateNr and if the state is
* acceptable or not. Also Allocates space for the two paths pointers.
* param[in]: dfa - A pointer to the dfa.
* param[in]: acceptable - tells if the state is to be acceptlable state or not.
* param[in]: stateNr - The stateNr for the state thats going into the dfa.
*/
void dfaInsertState (dfa *dfa, bool acceptable, char *stateName);

/*
* description: Modifys the path pointers of a state.
* param[in]: dfa - A pointer to the dfa.
* param[in]: state - the state which path are to be modified.
* param[in]: path - the path which are to be modified.
* param[in]: toState - the state to which the path going to point to.
*/
void dfaModifyState (dfa *dfa, char *state, char *path, char *toState);

/*
* description: Changes the current state of the dfa depending of which path is
* input.
* param[in]: dfa - A pointer to the dfa.
* param[in]: path - A number representing the path that current state is going
* to use to move to the next state.
*/
int dfaChangeState (dfa *dfa, char *key);


/*
* description: Resets the DFA to its starting positon.
* param[in]: dfa - The dfa to be reset.
*/
void dfaReset (dfa *dfa);

/*
* description: Gets the current state of the dfa.
* param[in]: dfa - A pointer to the dfa.
* return: A number representing the current state of the dfa.
*/
int dfaGetCurrentState (dfa *dfa);

/*
* description: Finds a particular state in the DFA.
* param[in]: dfa - The dfa
* param[in]: stateName - The name of the state to be found.
* return: If found; the state, else NULL.
*/
state *dfaFindState(dfa *dfa, char *stateName);


/*
* description: frees all memeory allocates bt the dfa.
* param[in]: dfa - A pointer to the dfa.
*/
void dfaKill (dfa *dfa);

/*
* description: frees all memory allocated by the state.
* param[in]: state - A pointer to the state.
*/
void stateKill (state *state);

/*
* description: Allocates memory for a new path. Insterts an initial key for the
* path.
* param[in]: key - Pointer to the key / name of the path.
* param[in]: destination - A pointer to the destination the key points to.
* return: The path.
*/
path *pathEmpty(char *key, state *destination);

/*
* description: Inserts a new path to a path.
* param[in]: fromState - State that path goes from.
* param[in]: key - Key / name of the path.
* param[in]: destination - The state that path will lead too.
*/
void pathInsert(state *fromState, char *key, state *destination);

/*
* description: Finds a state in a pathlist.
* param[in]:
* return:
*/
state *pathlistFindState(path *path, char *key);

/*
* description:
* param[in]:
* return:
*/
void pathKill (path *path);

/*
* description:
* param[in]:
* return:
*/

#endif //DFAMGENERATOR
