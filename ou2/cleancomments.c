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


#include "cleancomments.h"


int main (int argc, char const *argv[]) {

    if (!fileValidation(argc, argv)) {

        fprintf(stderr, "quitting program!\n");
        return 0;
    }
    FILE *inFile = fopen (argv[1], "r");
    FILE *outFile = fopen (argv[2], "w");

    int currC;
    int prevC = '\0';
    int state = Q0_FALSECOMMENT;

    /*
    * Note: Characther's '/' will only be written from prevC and not currC.
    * This is so that program can write multiple '/'s in a row.
    * If prevC = '\0' then a '/' shall not be written.
    */
    while ((currC = fgetc(inFile)) != EOF) {

        state = stateSelect(state, (char)currC);

        switch (state) {

            case Q0_FALSECOMMENT:
                if (prevC != '\0') {

                    fputc(prevC, outFile);
                    prevC = '\0';
                }
                if (currC != '/') {

                    fputc(currC, outFile);
                }
                break;

            case Q1_ISCOMMENT:
                if (prevC != '\0') {

                    fputc(prevC, outFile);
                }
                prevC = currC;
                break;

            case Q2_TRUECOMMENT:
                prevC = '\0';
                break;
            }
    }

    fclose(inFile);
    fclose(outFile);
    return 1;
}


/*
* description: Selects which state to enter, depending on which state DFA is
* currently on and next input charachter.
* param[in]: state - The current state.
* param[in]: currC - The current input characther
* return: The new state
*/
int stateSelect(int state, char currC) {

    switch (state) {
        case Q0_FALSECOMMENT:
            if (currC == '/') {

                state = Q1_ISCOMMENT;
            }
            break;

        case Q1_ISCOMMENT:
            if (currC == '*') {

                state = Q2_TRUECOMMENT;
            } else {

                state = Q0_FALSECOMMENT;
                state = stateSelect(state, currC);
            }
            break;

        case Q2_TRUECOMMENT:
            if (currC == '*') {

                state = Q3_ENDOFCOMMENT;
            }
            break;

        case Q3_ENDOFCOMMENT:
            if (currC == '/') {

                state = Q0_FALSECOMMENT;
            } else {

                state = Q2_TRUECOMMENT;
                state = stateSelect(state, currC);
            }
            break;
        }
    return state;
}


/*
* description: Validates program input.
* param[in]: argc - number of arguments.
* param[in]: argv[1] - Name of file to be read from.
* param[in]: argv[2] - Name of file to be written to.
* return: 1 if valid, else 0.
*/
int fileValidation (int argc, char const *argv[]) {

    FILE *fp;

    if (argc != 3) {

        fprintf(stderr, "Invlid number of parameters - ");
        return 0;
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL) {

        fprintf(stderr, "Could not open '%s' to read - ", argv[1]);
        return 0;
    }
    fclose(fp);

    fp = fopen(argv[2], "w");
    if (fp == NULL) {

        fprintf(stderr, "Could not open '%s' to write - ", argv[2]);
        return 0;
    }
    fclose(fp);

    return 1;
}
