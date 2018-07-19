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


#include "dfa.h"

/*
* description: Allocates memory for an dfa and Creates an empty dfa.
* return: Empty dfa.
*/
dfa *dfaEmpty(){

    dfa *dfa = malloc(sizeof(struct dfa));
    dfa -> allStates = NULL;
    dfa -> startState = NULL;
    dfa -> currState = NULL;
	dfa -> capacity = 0;
	dfa -> size = 0;
    return dfa;
}

/*
* description: Checks if the dfa is empty.
* param[in]: dfa - A pointer to the dfa.
* return: bool depending on if the dfa is empty or not.
*/
bool dfaIsEmpty (dfa *dfa) {

    return dfa -> allStates == NULL;
}

/*
* description: Allocates memory for all possible states.
* param[in]: dfa - A pointer to the dfa.
* param[in]: capacity - the number of possible states.
*/
void dfaSetStates (dfa *dfa, int capacity) {

    dfa -> capacity = capacity;
    dfa -> allStates = realloc(dfa -> allStates, sizeof(state*) * capacity);
}

/*
* description: Sets which state is the startState in the dfa.
* param[in]: dfa - A pointer to the dfa.
* param[in]: startState - the state to be set to startState.
*/
void dfaSetStart (dfa *dfa, char *stateName) {

	state* startState = dfaFindState(dfa, stateName);
    dfa -> startState = startState;
    dfa -> currState = startState;
}

/*
* description: Inserts a state to the dfa and sets stateNr and if the state is
* acceptable or not. Also Allocates space for the two paths pointers.
* param[in]: dfa - A pointer to the dfa.
* param[in]: acceptable - tells if the state is to be acceptlable state or not.
* param[in]: stateNr - The stateNr for the state thats going into the dfa.
*/
void dfaInsertState (dfa *dfa, bool acceptable, char *stateName) {

    dfa -> allStates[dfa -> size] = malloc(sizeof(struct state));
	dfa -> allStates[dfa -> size] -> stateName = stateName;
    dfa -> allStates[dfa -> size] -> stateNr = dfa -> size;
    dfa -> allStates[dfa -> size] -> acceptable = acceptable;
	dfa -> allStates[dfa -> size] -> paths = NULL;
	dfa -> size++;
}

/*
* description: Modifys the path pointers of a state.
* param[in]: dfa - A pointer to the dfa.
* param[in]: state - the state which path are to be modified.
* param[in]: path - the path which are to be modified.
* param[in] toState - the state to which the path going to point to.
*/
void dfaModifyState (dfa *dfa, char *fromState, char *path, char *toState) {

	state *destination = dfaFindState(dfa, toState);
	state *state = dfaFindState(dfa, fromState);

	if (state != NULL) {

		pathInsert(state, path, destination);
	}
}

/*
* description: Changes the current state of the dfa depending of which path is
* input.
* param[in]: dfa - A pointer to the dfa.
* param[in]: path - A number representing the path that current state is going
* to use to move to the next state.
*/
int dfaChangeState (dfa *dfa, char *key) {

	int letterValidity = -1;
    if (dfa -> currState -> paths != NULL) {

		state *toState = pathFindState(dfa -> currState -> paths, key);

		if (toState != NULL) {

			dfa -> currState = toState;
			letterValidity = 1;
	    } else {

			letterValidity = 0;
		}
	}
	return letterValidity;
}

void dfaReset (dfa *dfa) {

	dfa -> currState = dfa -> startState;
}

/*
* description: Gets the current state of the dfa.
* param[in]: dfa - A pointer to the dfa.
* return: A number representing the current state of the dfa.
*/
int dfaGetCurrentState (dfa *dfa) {

    return dfa -> currState -> stateNr;
}

state *dfaFindState(dfa *dfa, char *stateName) {

	state* foundState = NULL;
	int i = 0;

	while (foundState == NULL && i < dfa -> size) {

		if (strcmp(stateName, dfa -> allStates[i] -> stateName) == 0) {

			foundState = dfa -> allStates[i];
		}
		i++;
	}
	return foundState;
}

/*
* description: free's all memeory allocates bt the dfa.
* param[in]: dfa - A pointer to the dfa.
*/
void dfaKill (dfa *dfa) {

    for (int i = 0; i < dfa -> capacity; i++) {

        if (dfa -> allStates[i] != NULL) {

			stateKill(dfa -> allStates[i]);
			dfa -> allStates[i] = NULL;
        }
    }
	/*if (dfa -> startState != NULL) {

		printf("size: %d", dfa -> size);
		stateKill(dfa -> startState);
	}*/
    free(dfa -> allStates);
    free(dfa);
}

void stateKill (state *state) {

	if (state -> stateName != NULL) {

		fprintf(stderr, "Goodbye: '%s'\n", state -> stateName);
		free(state -> stateName);
	}
	pathKill(state -> paths);
	free(state);
}

path *pathEmpty(char *key, state* destination) {

	path *path = malloc(sizeof(*path));
	path -> key = key;
	path -> destination = destination;
	path -> nextPath = NULL;
	return path;
}

void pathInsert(state *fromState, char* key, state* destination) {

	if (fromState -> paths == NULL ) {

		fromState -> paths = pathEmpty(key, destination);
	} else {

		struct path *tempPath = fromState -> paths;
		fromState -> paths = pathEmpty(key, destination);
		fromState -> paths -> nextPath = tempPath;
	}
}

state *pathFindState(path* path, char* key) {

	state *foundState = NULL;
	struct path *currPath = path;

	while (foundState == NULL && currPath != NULL) {

		if (strncmp(key, currPath -> key, 1) == 0) {

			foundState = currPath -> destination;
		} else {

			currPath = currPath -> nextPath;
		}
	}
	return foundState;
}

void pathKill (path *path) {

	while (path != NULL) {

		struct path *tempPath = path;
		path = path -> nextPath;

		fprintf(stderr, "YOU'RE FREE: ");
		if (tempPath -> key != NULL) {


			fprintf(stderr, "'%s'", tempPath -> key);
			free(tempPath -> key);
		}
		fprintf(stderr, "\n");
		free(tempPath);
	}
}
