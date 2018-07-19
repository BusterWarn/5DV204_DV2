/*
* rundfa: Builds and runs the dfa by using the dfa datatype declared in dfa.h
*
* param[in]: argv[0] - ./[exacutable program name]
* param[in]: argv[1] - filename of the file with the specification for the dfa.
*
* Authors:
* Buster Hultgren Warn <dv17bhn@cs.umu.se>
* Victor Liljeholm <dv13vlm@cs.umu.se>
*
* Final build: 2018-03-13
*/

#include "rundfa.h"

int main(int argc, const char *argv[]){

    if (fileValidation(argc, argv) == 0) {

        fprintf(stderr, " - quitting program\n");
        return 0;
    }

    dfa* dfa = buildDfa(argv);

    runDfa(dfa);
    dfaKill(dfa);
    return 1;
}

/*
* description: Creates and builds the dfa by using data from a textfile and
* applying it to the dfa datatype. It calculates number of states, sets up
* the difffrent states, there path and if they are accaptable or not.
* param[in]: argv[] - Starting parameter with the name of the textfile with the
* dfa state.
* returns: The complete dfa.
*/
dfa *buildDfa (const char *argv[]) {

    dfa *dfa = dfaEmpty();
    FILE *fp = fopen(argv[1], "r");

    char *startState = readLine(fp);
    char *acceptable = readLine(fp);
    char *other = readLine(fp);

    int allStates = countStates(acceptable);
    allStates += countStates(other);

	printf("\n\n");

    dfaSetStates(dfa, allStates);

    setStates(dfa, acceptable, 1);
    setStates(dfa, other, 0);
    setStates(dfa, startState, 2);

    free(startState);
    free(acceptable);
    free(other);

    setPaths(dfa, fp);
    fclose(fp);
	return dfa;
}

/*
* description: Finds the next number(if any) in an array of chars and converts
* and saves the number found to an integer.
* param[in]: line - A pointer to an array where the next number(if any)
* is to be found.
* param[in]: i - the index where the function should start looking in the array.
* returns: the number found as an integer.
*/
int getNextInt (char *line, int *i) {

    int nextInt = -1;
    int foundInt = 0;

    while (line[*i] != '\0') {

        if (line[*i] >= '0' && line[*i] <= '9') {

            if (nextInt == -1) {

                nextInt = 0;
            }
            nextInt = nextInt * 10 + (line[*i] - '0');
            foundInt = 1;

        } else if (foundInt == 1) {

            (*i)++;
            return nextInt;
        }
        (*i)++;
    }
    return nextInt;
}

/*
* description: Finds the next number(if any) in an array of chars and converts
* and saves the number found to an integer.
* param[in]: line - A pointer to an array where the next number(if any)
* is to be found.
* param[in]: i - the index where the function should start looking in the array.
* returns: the number found as an integer.
*/
char* getNextWord (char *line, int *i) {

    int wordFound = 0;
    int startOfWord = 0;
	char *word = NULL;

	if (line[*i] == '\0') {

		wordFound = -1;
	}

    while (wordFound >= 0) {

		if (line[*i] == '\0') {

			wordFound = -1;
		}
		if (wordFound == 0) {

			if (line[*i] >= 33 && line[*i] <= 126) {

			wordFound = 1;
			startOfWord = *i;
			}
    	} else {

			if (line[*i] < 33 || line[*i] > 126) {

				wordFound = -1;
				word = malloc(sizeof(char) * (*i - startOfWord + 1));
				strncpy(word, &line[startOfWord], *i - startOfWord + 1);
				word[*i - startOfWord] = '\0';
				if (line[*i] == '\0') {

					(*i)--;
				}
			}
		}

		(*i)++;
	}

    return word;
}

/*
* description: Reads a string from a file and saves it as an array of chars that
* has its memmory dynamically allocated.
* param[in]: fp - A file pointer.
* returns: A pointer to the allocated memory for the char array.
*/
char *readLine (FILE *fp) {

    int length = 0;
    int buffer = 100;
    char *line = malloc(sizeof(char) * buffer);
    char currChar = fgetc(fp);

    while (currChar != '\n' && currChar >= 0) {

        if (length >= buffer - 2) {

            buffer = buffer * 2;
            line = realloc(line, sizeof(char) * buffer);
        }
        line[length] = currChar;

        length++;
        currChar = fgetc(fp);
    }

	if (length > 0) {

		line[length - 1] = '\0';
		line = realloc(line, sizeof(char) * length);
	} else {

		free(line);
		line = NULL;
	}

    return line;
}

/*
* description: Counts the number of stats found in an array of chars.
* param[in]: line - a pointer to the char array.
* returns: The number of stats found.
*/
int countStates (char *line) {

	int wordFound = 0;
    int states = 0;
    int i = 0;

    while (line[i] != '\0') {

        if (wordFound == 0) {

			if (line[i] >= 33 && line[i] <= 126) {

				wordFound = 1;
				states++;
			}
		} else {

			if (line[i] < 33 || line[i] > 126) {

				wordFound = 0;
			}
		}
		i++;
    }
    return states;
}

/*
* description: Inserts the diffrent states found in an array of strings to the
* right place in the dfa depending on there state number, if they are
* acceptable states and if they are a start state.
* param[in]: dfa - pointer to the dfa.
* param[in]: line - Pointer to the array of chars with the diffrent states.
* param[in] acceptable - Tells if there acceptable states(1), start state(2) or
* other states(0) thats going to be set.
*/
void setStates (dfa *dfa, char *line, int acceptable) {


    int i = 0;
    char *currState = getNextWord(line, &i);

    while (currState != NULL) {

        if (acceptable != 2) {

            dfaInsertState(dfa, acceptable, currState);
        } else {

            dfaSetStart(dfa, currState);
			free(currState);
        }
        currState = getNextWord(line, &i);
    }
}

/*
* description: Sets the path of the diffrent states, the paths is found in an
* textfile.
* param[in]: dfa - pointer to the dfa.
* param[in]: fp - A file pointer to the file with the paths.
*/
void setPaths (dfa *dfa, FILE *fp) {

    char *pathLine = readLine(fp);

    while (pathLine != NULL) {

		int i = 0;
        char* fromState = getNextWord(pathLine, &i);
        char* path = getNextWord(pathLine, &i);
        char* toState = getNextWord(pathLine, &i);

        dfaModifyState(dfa, fromState, path, toState);

		if (fromState != NULL) {

			free(fromState);
		}

		if (toState != NULL) {

			free(toState);
		}

        free(pathLine);
        pathLine = readLine(fp);
    }

    if (pathLine != NULL) {

        free(pathLine);
    }
}

/*
* description: checks so program go the right amount of parameters and that it
* can open and read or write to file.
* param[in]: argc - number of parameters.
* param[in]: argv - Parameters strings.
* returns: 0 if failed and 1 if succeeded with the tests.
*/
int fileValidation (int argc, const char *argv[]) {

    FILE *fp;

    if (argc != 2) {

        fprintf(stderr, "To many/few argument");
        return 0;
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL) {

        fprintf(stderr, "Cannot read '%s'", argv[1]);
        return 0;
    }
    fclose(fp);
    return 1;
}

/*
* description: Prompts the user to user to give the the dfa a string,
* also tells the user if the the string was accepted by the string or not.
* param[in]: argc - number of parameters.
* param[in]: argv - Parameters strings.
* returns: 0 if failed and 1 if succeeded with the tests.
*/
void runDfa (dfa *dfa) {



    char choice = '0';
    while (isAcceptedLetter(choice) >= 0)
    {
		printf("Write a sting of letters accepted by the current alphabet. A"
				" '?' will quit the program\n");

        scanf("%c", &choice);

		int letterValidity = 1;
        while (isAcceptedLetter(choice) == 1 && letterValidity == 1) {

            letterValidity = dfaChangeState(dfa, &choice);

			if (letterValidity == 1) {

				scanf("%c", &choice);
			}
        }

		if (letterValidity == 1) {

			printf("The string is %s\n\n", dfa -> currState -> acceptable == 1 ?
			"accepted by the dfa" : "not accepted by the dfa");
			dfaReset(dfa);
		} else if (letterValidity == 0) {

			printf("'%c' is not a letter in the current alphabet\n\n", choice);
		} else {

			printf("DFA was not setup correctly and path does not exist for"
			"current path in DFA\n\n");
		}
    }
}

/*
* description:
* param[in]:
* return:
*/
int isAcceptedLetter (char letter) {

	if ((letter >= '0' && letter <= '9') ||
		(letter >= 'a' && letter <= 'z') ||
		(letter >= 'A' && letter <= 'Z')) {

			return 1;
		} else  if (letter == '\n' || letter == '\r') {

			return 0;
		} else {

			return -1;
		}
}
