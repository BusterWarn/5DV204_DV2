/*
* rundfa: Builds and runs the dfa by using the dfa datatype declared in dfa.h
*
* The DFA will be built dynamically. Each state must be named with letters and
* / or numbers. A space will indicate a new state or path. There is no limit
* to the amount of paths each state may contain, and different states may
* contain different amount of paths.
*
* Each alphabetical / numerical key marking each path must be exactly one char
* long - a path can contain the key 'a' but not 'aa'.
*
* The DFA specifications need to be declared in a textfile as following:
* Row 1: The start state, exactly one.
* Row 2: The acceptable states.
* Row 3: The non acceptable states.
* Row 4 - n: A state, followed by a key, followed by another state (with spaces
* in between). Each row will represent a state (the former) and a key to a path
* in it which will lead to another state (the latter).
*
* To test if strings are valid in the built DFA, simply enter the string in the
* terminal. Only alphabetical and numerical keys are valid. Anything else, like
* a '?', will quit the program.
*
* param[in]: argv[0] - ./[exacutable program name]
* param[in]: argv[1] - filename of the file with the specification for the dfa.
*
* Authors:
* Buster Hultgren Warn <dv17bhn@cs.umu.se>
* Victor Liljeholm <dv13vlm@cs.umu.se>
*
* Final build: 2018-08-23
*/

#include "rundfa.h"

/*
* Description: The main file. First validating the input
* arguments, then building, running and freeing the DFA.
*/
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
* description: Creates and builds the dfa by using data from textfile (argv[1])
* and applying it to the dfa datatype. It calculates number of states, sets up
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
* description: Finds the next number (if any) in an array of chars and returns
* it.
* param[in]: line - A pointer to an array where the next number (if any) is to
* be found.
* param[in]: i - The index where the function should start looking in the
* array.
* returns: If found; the number, else -1.
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
* description: Find the next word (if any) in an array of chars and returns it.
* param[in]: line - A pointer to an array where the next word (if any) is to
* be found.
* param[in]: i - The index where the function should start looking in the
* array.
* return: If found; the word, else NULL.
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
* description: Reads a line from a file and saves it as an array of chars with
* dynamically allocated memory.
* param[in]: fp - A file pointer.
* returns: A pointer to the allocated memory for the char array.
*/
char *readLine (FILE *fp) {

    int length = 0;
    int buffer = 100;
    char *line = malloc(sizeof(char) * buffer);
    char currChar = fgetc(fp);

    while (currChar != '\n' && currChar >= 0) {

		//If a carriage return is read, skip it.
		if (currChar == '\r') {

			currChar = fgetc(fp);
		} else {

			if (length >= buffer - 2) {

	            buffer = buffer * 2;
	            line = realloc(line, sizeof(char) * buffer);
	        }
	        line[length] = currChar;

	        length++;
	        currChar = fgetc(fp);
		}
    }

	if (length > 0) {

		line[length] = '\0';
		line = realloc(line, sizeof(char) * length + 1);
	} else {

		free(line);
		line = NULL;
	}

    return line;
}

/*
* description: Counts the number of states found in an array of chars. Each
* word consisting of ascii chars including 33 to 126 counts as a state.
* param[in]: line - A pointer to the char array.
* returns: The number of states found.
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
* description: Inserts states found in a string. Will insert the states
* either as acceptable or not acceptable, depending on argument.
* param[in]: dfa - Pointer to the dfa.
* param[in]: line - Pointer to the string with the diffrent states.
* param[in] acceptable - Tells if states are not acceptable(0), acceptable(1)
* or if they're start states(2).
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
* description: Sets the paths of the diffrent states, the paths is found in a
* textfile.
* param[in]: dfa - Pointer to the dfa.
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
* description: Validates number of arguments and that textfile (argv[1]) can be
* read and written to.
* param[in]: argc - number of parameters.
* param[in]: argv - Parameters strings.
* returns: 1 if arguments are valid, else 0.
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
* description: Runs the DFA with a while loop. Checks for input strings and
* compares them to the DFA states, to see if they are accepable or not. If a
* non -alhabetical or -numerical (like '?') is met, function ends.
* param[in]: dfa - Pointer to the DFA.
*/
void runDfa (dfa *dfa) {

	char choice = '0';
    while (isAcceptedLetter(choice) >= 0) {

		printf("-----\n\n");
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
		} else if (letterValidity == 0) {

			printf("'%c' is not a letter in the current alphabet\n\n", choice);
			clearInputStream();
		} else {

			printf("DFA was not setup correctly and path does not exist for"
			"current path in DFA\n\n");
			clearInputStream();
		}
		dfaReset(dfa);
    }
}

/*
* description: Validates char to see if it is a valid alphabetical / numerical
* key, a line break or a command to quit program.
* param[in]: letter - The char to be validated.
* return: 1 if key, 0 if linebreak, -1 if command to quit program.
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

/*
* description: Clears the current input stream. Note: if input stream is
* already clear, function will wait for input. If this happens, simply input
* something in terminal.
*/
void clearInputStream () {

	char buffer = '0';
	while (buffer != '\n') {

		buffer = getchar();
	}
}
