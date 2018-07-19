/*
* Cleancomments: Deletes all comments from a text file. These are the standard
* comments in C, consisting of / and * characthers.
*
* param[in]: argv[0] - ./[exacutable program name]
* param[in]: argv[1] - Name of file to be read (consisting comments).
* param[in]: argv[2]  - Name of file to be read (will not contain comments).
*
* Authors:
* Buster Hultgren Warn <dv17bhn@cs.umu.se>
* Victor Liljeholm <dv13vlm@cs.umu.se>
*
* Final build: 2018-03-13
*/


#include <stdio.h>
#include <stdlib.h>


/*
* Predefined states for easier reading.
* State Q0 - Reading non-comment text. If '/' is read, state will change to
* Q1. Else Q0.
* State Q1 - Found beginning of comment. If '*' is read, state will change to
* Q2, else Q0.
* State Q2 - Inside comment. If '*' is read, state will change to Q3, else Q2.
* State Q3 - End of comment found. If '/' is read, state will change to Q0,
* else Q2.
*/
#define Q0_FALSECOMMENT 0
#define Q1_ISCOMMENT 1
#define Q2_TRUECOMMENT 2
#define Q3_ENDOFCOMMENT 3


/*
* description: Selects which state to enter, depending on which state DFA is
* currently on and next input charachter.
* param[in]: state - The current state.
* param[in]: currC - The current input characther
* return: The new state
*/
int stateSelect(int state, char currentC);


/*
* description: Validates program input.
* param[in]: argc - number of arguments.
* param[in]: argv[1] - Name of file to be read from.
* param[in]: argv[2] - Name of file to be written to.
* return: 1 if valid, else 0.
*/
int fileValidation (int argc, char const *argv[]);
