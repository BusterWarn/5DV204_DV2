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

#include <stdio.h>
#include <stdlib.h>

#include "dfa.h"

/*
* description: Creates and builds the dfa by using data from a textfile and
* applying it to the dfa datatype. It calculates number of states, sets up
* the difffrent states, there path and if they are accaptable or not.
* param[in]: argv[] - Starting parameter with the name of the textfile with the
* dfa state.
* returns: The complete dfa.
*/
dfa *buildDfa (const char *argv[]);

/*
* description: Finds the next number(if any) in an array of chars and converts
* and saves the number found to an integer.
* param[in]: line - A pointer to an array where the next number(if any)
* is to be found.
* param[in]: i - the index where the function should start looking in the array.
* returns: the number found as an integer.
*/
int getNextInt (char *line, int *i);

char* getNextWord (char *line, int *i);

/*
* description: Reads a string from a file and saves it as an array of chars that
* has its memmory dynamically allocated.
* param[in]: fp - A file pointer.
* returns: A pointer to the allocated memory for the char array.
*/
char *readLine (FILE *fp);

/*
* description: Counts the number of stats found in an array of chars.
* param[in]: line - a pointer to the char array.
* returns: The number of stats found.
*/
int countStates (char *line);

/*
* description: Inserts the diffrent states found in an array of strings to the
* right place in the dfa depending on there state number, if they are acceptable
* states and if they are a start state.
* param[in]: dfa - pointer to the dfa.
* param[in]: line - Pointer to the array of chars with the diffrent states.
* param[in] acceptable - Tells if there acceptable states(1), start state(2) or
* other states(0) thats going to be set.
*/
void setStates (dfa *dfa, char *line, int acceptable);

/*
* description: Sets the path of the diffrent states, the paths is found in an
* textfile.
* param[in]: dfa - pointer to the dfa.
* param[in]: fp - A file pointer to the file with the paths.
*/
void setPaths (dfa *dfa, FILE *fp);

/*
* description: checks so program go the right amount of parameters and that it
* can open and read or write to file.
* param[in]: argc - number of parameters.
* param[in]: argv - Parameters strings.
* returns: 0 if failed and 1 if succeeded with the tests.
*/
int fileValidation (int argc, const char *argv[]);

/*
* description: Converts a char variable to an integer so if its a A or B it can
* also represent an one or zero.
* param[in]: choicechar - a char that going to be checked if it should be
* converted to a one, zero or minus one.
* returns: A number representing the input char.
*/
void runDfa (dfa *dfa);

/*
* description: Converts a char variable to an integer so if its a A or B it can
* also represent an one or zero.
* param[in]: choicechar - a char that going to be checked if it should be
* converted to a one, zero or minus one.
* returns: A number representing the input char.
*/
int convertPathChoiceToInt(char choicechar);

int isAcceptedLetter (char letter);
