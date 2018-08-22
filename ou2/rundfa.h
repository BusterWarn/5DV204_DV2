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
* param[in]: argv[0] - ./[exacutable program name]
* param[in]: argv[1] - filename of the file with the specification for the dfa.
*
* Authors:
* Buster Hultgren Warn <dv17bhn@cs.umu.se>
* Victor Liljeholm <dv13vlm@cs.umu.se>
*
* Final build: 2018-08-21
*/

#include <stdio.h>
#include <stdlib.h>

#include "dfa.h"

/*
* description: Creates and builds the dfa by using data from textfile (argv[1])
* and applying it to the dfa datatype. It calculates number of states, sets up
* the difffrent states, there path and if they are accaptable or not.
* param[in]: argv[] - Starting parameter with the name of the textfile with the
* dfa state.
* returns: The complete dfa.
*/
dfa *buildDfa (const char *argv[]);

/*
* description: Finds the next number (if any) in an array of chars and returns
* it.
* param[in]: line - A pointer to an array where the next number (if any) is to
* be found.
* param[in]: i - The index where the function should start looking in the
* array.
* returns: If found; the number, else -1.
*/
int getNextInt (char *line, int *i);

/*
* description: Find the next word (if any) in an array of chars and returns it.
* param[in]: line - A pointer to an array where the next word (if any) is to
* be found.
* param[in]: i - The index where the function should start looking in the
* array.
* return: If found; the word, else NULL.
*/
char* getNextWord (char *line, int *i);

/*
* description: Reads a line from a file and saves it as an array of chars with
* dynamically allocated memory.
* param[in]: fp - A file pointer.
* returns: A pointer to the allocated memory for the char array.
*/
char *readLine (FILE *fp);

/*
* description: Counts the number of states found in an array of chars. Each
* word consisting of ascii chars including 33 to 126 counts as a state.
* param[in]: line - A pointer to the char array.
* returns: The number of states found.
*/
int countStates (char *line);

/*
* description: Inserts states found in a string. Will insert the states
* either as acceptable or not acceptable, depending on argument.
* param[in]: dfa - Pointer to the dfa.
* param[in]: line - Pointer to the string with the diffrent states.
* param[in] acceptable - Tells if states are not acceptable(0), acceptable(1)
* or if they're start states(2).
*/
void setStates (dfa *dfa, char *line, int acceptable);

/*
* description: Sets the paths of the diffrent states, the paths is found in a
* textfile.
* param[in]: dfa - Pointer to the dfa.
* param[in]: fp - A file pointer to the file with the paths.
*/
void setPaths (dfa *dfa, FILE *fp);

/*
* description: Validates number of arguments and that textfile (argv[1]) can be
* read and written to.
* param[in]: argc - number of parameters.
* param[in]: argv - Parameters strings.
* returns: 1 if arguments are valid, else 0.
*/
int fileValidation (int argc, const char *argv[]);

/*
* description: Runs the DFA with a while loop. Checks for input strings and
* compares them to the DFA states, to see if they are accepable or not. If a
* non -alhabetical or -numerical (like '?') is met, function ends.
* param[in]: dfa - Pointer to the DFA.
*/
void runDfa (dfa *dfa);

/*
* description: Validates char to see if it is a valid alphabetical / numerical
* key, a line break or a command to quit program.
* param[in]: letter - The char to be validated.
* return: 1 if key, 0 if linebreak, -1 if command to quit program.
*/
int isAcceptedLetter (char letter);
