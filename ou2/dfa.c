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
* Final build: 2018-08-21
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
* description: Allocates memory for a dfa and Creates an empty dfa.
* return: Empty dfa.
*/
bool dfaIsEmpty (dfa *dfa) {

    return dfa -> allStates == NULL;
}

/*
* description: Allocates memory for all the states to be set into the dfa.
* param[in]: dfa - A pointer to the dfa.
* param[in]: capacity - The number of states the dfa shall contain.
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
* description: If number of inserted states is smaller than the DFA's capacity,
* allocate memory for and insert a state.
* param[in]: dfa - A pointer to the dfa.
* param[in]: acceptable - tells if the state is to be acceptlable or not.
* param[in]: stateName - The name of the state.
*/
void dfaInsertState (dfa *dfa, bool acceptable, char *stateName) {

	if (dfa -> size < dfa -> capacity) {

		dfa -> allStates[dfa -> size] = malloc(sizeof(struct state));
		dfa -> allStates[dfa -> size] -> stateName = stateName;
		dfa -> allStates[dfa -> size] -> stateNr = dfa -> size;
		dfa -> allStates[dfa -> size] -> acceptable = acceptable;
		dfa -> allStates[dfa -> size] -> paths = NULL;
		dfa -> size++;
	} else {

		fprintf(stderr, "Cannot insert state '%s', DFA is already full\n",
				stateName);
	}
}

/*
* description: Modifies a state by adding a path.
* param[in]: dfa - Pointer to dfa which includes the state.
* param[in]: fromState - Pointer to the state which path are to be modified.
* param[in]: path - The alpabetical key of the path.
* param[in]: toState - The state to which the path leads to.
*/
void dfaModifyState (dfa *dfa, char *fromState, char *path, char *toState) {

	state *destination = dfaFindState(dfa, toState);
	state *state = dfaFindState(dfa, fromState);

	if (state != NULL) {

		pathInsert(state, path, destination);
	}
}

/*
* description: Changes the current state of the dfa by one of the paths
* connected to the current state.
* param[in]: dfa - Pointer to the dfa.
* param[in]: path - The alphabetical key to one of the paths connected to the
* current state.
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

/*
* description: Finds a particular state in the DFA.
* param[in]: dfa - The dfa.
* param[in]: stateName - The name of the state to be found.
* return: If found; the state, else NULL.
*/
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
* description: Resets the DFA's current state to its starting state.
* param[in]: dfa - The dfa to be reset.
*/
void dfaReset (dfa *dfa) {

	dfa -> currState = dfa -> startState;
}

/*
* description: Frees all memeory allocated by and in the dfa.
* param[in]: dfa - A pointer to the dfa.
*/
void dfaKill (dfa *dfa) {

    for (int i = 0; i < dfa -> capacity; i++) {

        if (dfa -> allStates[i] != NULL) {

			stateKill(dfa -> allStates[i]);
			dfa -> allStates[i] = NULL;
        }
    }
    free(dfa -> allStates);
    free(dfa);
}

/*
* description: Frees all memory allocated by and in the state.
* param[in]: state - A pointer to the state.
*/
void stateKill (state *state) {

	if (state -> stateName != NULL) {

		fprintf(stderr, "Goodbye: '%s'\n", state -> stateName);
		free(state -> stateName);
	}
	pathKill(state -> paths);
	free(state);
}

/*
* description: Allocates memory for a new path. Insterts an initial key for the
* path.
* param[in]: key - Pointer to the alpabetical key / name of the path.
* param[in]: destination - A pointer to the destination the key points to.
* return: The path.
*/
path *pathEmpty(char *key, state* destination) {

	path *path = malloc(sizeof(*path));
	path -> key = key;
	path -> destination = destination;
	path -> nextPath = NULL;
	return path;
}

/*
* description: Inserts a new path to a path.
* param[in]: fromState - State that path goes from.
* param[in]: key - Alphabetical key / name of the path.
* param[in]: destination - The state that path will lead too.
*/
void pathInsert(state *fromState, char* key, state* destination) {

	if (fromState -> paths == NULL ) {

		fromState -> paths = pathEmpty(key, destination);
	} else {

		struct path *tempPath = fromState -> paths;
		fromState -> paths = pathEmpty(key, destination);
		fromState -> paths -> nextPath = tempPath;
	}
}

/*
* description: Finds a state in a path.
* param[in]: path - Pointer to the path.
* param[in]: key - Alphabetical key which leads to the state.
* return: If found; the state, else NULL.
*/
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

/*
* description:Frees all memeory allocated by and in the path.
* param[in]: path - Pointer to the path.
*/
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

void dfaPrint (dfa *dfa) {

	printf("\n\n ----- Printing DFA -----\n\n");
	for (int i = 0; i < dfa -> size; i++) {

		printf("\nstate: '%s'\n", dfa -> allStates [i] -> stateName);

		struct path *tempPath = NULL;
		if (dfa -> allStates[i] != NULL) {

			tempPath = dfa -> allStates[i] -> paths;
		}

		while (tempPath != NULL) {

			printf(" - ");
			if (tempPath -> key != NULL) {

				printf("%c -> ", tempPath -> key);
				if (tempPath -> destination != NULL) {

					printf("'%s'\n", tempPath -> destination -> stateName);
				} else {

					printf("NULL\n");
				}
			} else {

				printf("NULL\n");
			}

			tempPath = tempPath -> nextPath;
		}
	}
}
