/*
* A dynamic DFA which can contain multiplale states and multiple paths from
* each state. There is no limit to the number of paths a state may contain and
* different states can contain different amount of paths.
*
* The alpahbetic keys (a,b,c or 1,2,3 etc.) that lead one state to another (Q1
* -> Q2 for example) can only be one characther long and must. The alphabet
* must be consisting of numbers or letters.
*
* Authors:
* Buster Hultgren Warn <dv17bhn@cs.umu.se>
* Victor Liljeholm <dv13vlm@cs.umu.se>
*
* Final build: 2018-08-23
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
* description: Allocates memory for a dfa and Creates an empty dfa.
* return: Empty dfa.
*/
dfa *dfaEmpty ();

/*
* description: Checks if the dfa is empty.
* param[in]: dfa - A pointer to the dfa.
* return: true if the dfa is empty, else false.
*/
bool dfaIsEmpty(dfa *dfa);

/*
* description: Allocates memory for all the states to be set into the dfa.
* param[in]: dfa - A pointer to the dfa.
* param[in]: capacity - The number of states the dfa shall contain.
*/
void dfaSetStates (dfa *dfa, int capacity);

/*
* description: Sets which state is the startState in the dfa.
* param[in]: dfa - A pointer to the dfa.
* param[in]: startState - the state to be set to startState.
*/
void dfaSetStart (dfa *dfa, char *stateName);

/*
* description: If number of inserted states is smaller than the DFA's capacity,
* allocate memory for and insert a state.
* param[in]: dfa - A pointer to the dfa.
* param[in]: acceptable - tells if the state is to be acceptlable or not.
* param[in]: stateName - The name of the state.
*/
void dfaInsertState (dfa *dfa, bool acceptable, char *stateName);

/*
* description: Modifies a state by adding a path.
* param[in]: dfa - Pointer to dfa which includes the state.
* param[in]: fromState - Pointer to the state which path are to be modified.
* param[in]: path - The alpabetical key of the path.
* param[in]: toState - The state to which the path leads to.
*/
void dfaModifyState (dfa *dfa, char *fromState, char *path, char *toState);

/*
* description: Changes the current state of the dfa by one of the paths
* connected to the current state.
* param[in]: dfa - Pointer to the dfa.
* param[in]: path - The alphabetical key to one of the paths connected to the
* current state.
*/
int dfaChangeState (dfa *dfa, char *key);

/*
* description: Finds a particular state in the DFA.
* param[in]: dfa - The dfa.
* param[in]: stateName - The name of the state to be found.
* return: If found; the state, else NULL.
*/
state *dfaFindState(dfa *dfa, char *stateName);

/*
* description: Resets the DFA's current state to its starting state.
* param[in]: dfa - The dfa to be reset.
*/
void dfaReset (dfa *dfa);

/*
* description: Help function to print DFA states and paths during development.
* param[in]: dfa - The dfa to be printed.
*/
void dfaPrint (dfa *dfa);

/*
* description: Frees all memeory allocated by and in the dfa.
* param[in]: dfa - A pointer to the dfa.
*/
void dfaKill (dfa *dfa);

/*
* description: Frees all memory allocated by and in the state.
* param[in]: state - A pointer to the state.
*/
void stateKill (state *state);

/*
* description: Allocates memory for a new path. Insterts an initial key for the
* path.
* param[in]: key - Pointer to the alpabetical key / name of the path.
* param[in]: destination - A pointer to the destination the key points to.
* return: The path.
*/
path *pathEmpty(char *key, state *destination);

/*
* description: Inserts a new path to a path.
* param[in]: fromState - State that path goes from.
* param[in]: key - Alphabetical key / name of the path.
* param[in]: destination - The state that path will lead too.
*/
void pathInsert(state *fromState, char *key, state *destination);

/*
* description: Finds a state in a path.
* param[in]: path - Pointer to the path.
* param[in]: key - Alphabetical key which leads to the state.
* return: If found; the state, else NULL.
*/
state *pathFindState(path *path, char *key);

/*
* description:Frees all memeory allocated by and in the path.
* param[in]: path - Pointer to the path.
*/
void pathKill (path *path);

#endif //DFAMGENERATOR
