/*
* Wordcount: Using a regular expression the program counts how many matches to
* the regular expression is found in a textfile and prints the words and how
* matches of that word is found, also prints the totalt number of matches found.
*
* param[in]: argv[0] - ./[exacutable program name]
* param[in]: argv[1] - Name of the file where the words are to be matched and
* counted.
*
* Authors:
* Buster Hultgren Warn <dv17bhn@cs.umu.se>
* Victor Liljeholm <dv13vlm@cs.umu.se>
*
* Final build: 2018-03-13
*/

#include <stdio.h>
#include <regex.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include "wordcount.h"

#define CAPACITY 100
#define MAXMATCHES 50

int main(int argc, char const *argv[]) {

    char expr[] = "\\b([a-z][aeiouy]{2}ing|"
                    "[aeiouy][aeiouy][a-z]ing|"
                    "[a-z]{2}[aeiouy]{2}ly|"
                    "[a-z][aeiouy]{2}[a-z]ly|"
                    "[aeiouy]{2}[a-z]{2}ly)\\b";
	regex_t regex;
    int status;
	int count = 0;
    FILE *inFile;
    wordCount *wc = wordCountEmpty();

    if (!fileValidation(argc, argv)) {

        fprintf(stderr, "quitting program!\n");
        return 0;
    }

    inFile = fopen(argv[1], "r");
	status = regcomp(&regex, expr, REG_ICASE|REG_EXTENDED);

    if (status != 0){

        fprintf(stderr, "Could not compile the regular expression\n");
    }

    count = calculateWordCount(wc, regex, inFile);

    printWordCount(wc, count);

    regfree(&regex);
	free(wc -> words);
	free (wc);
    fclose(inFile);

    return 0;
    }

/*
* Description: Creates a allocates memory for and create an empty WordCount.
* return: Empty wordCount.
*/
wordCount *wordCountEmpty () {

    wordCount *wc = malloc(sizeof(wordCount));
    wc -> words = calloc(CAPACITY, sizeof(word));
    wc -> capacity = CAPACITY;
    wc -> inUse = 0;

    return wc;
}

/*
* description: Reads strings one by one from the file and checks the strings
* against the regular expression and saves any matches to the wordCount. Also
* counts nummber of matches.
* param[in]: wc - A pointer to the wordCount.
* param[in]: regex - The compiled regular expression.
* param[in]: inFile - pointer to the file stream.
* return: the number of matches found.
*/
int calculateWordCount(wordCount *wc, regex_t regex, FILE *inFile){
    int count = 0;
    char buffer[500];
    char tempWord[50];
    int status;
    regmatch_t matches[MAXMATCHES];

    while(fgets(buffer, 1000, inFile) != NULL){


        status = regexec(&regex, buffer, MAXMATCHES, matches, 0);

        if(status == 0){

            int i = 1;
            while(matches[i].rm_so != -1){

                strncpy(tempWord, buffer+matches[i].rm_so,
                        matches[i].rm_eo - matches[i].rm_so);
                tempWord[matches[i].rm_eo - matches[i].rm_so] = '\0';

                addWord(wc, tempWord);
                count++;
                i++;
            }
        }
    }
    return count;
}

/*
* description: Reallocates memeory if the wordCount is full, then checks if the
* string allready exist in wordCount to then either add to the count of a
* string int wordCount or add a new string to wordCount.
* param[in]: wc - A pointer to the wordCount.
* param[in]: newWord - Pointer to the string to be added to wordCount.
* return: updated wordCount.
*/
void addWord(wordCount *wc, char* newWord) {

    int pos = 0;
    if (wc -> inUse == wc -> capacity) {

        wc -> capacity = wc -> capacity + 100;
        wc -> words = realloc(wc -> words, sizeof(word) * wc -> capacity);
    }

    pos = wordLookup(wc, newWord);

    if(pos == -1){

        strncpy(wc -> words[wc -> inUse].word, newWord, 6);
        wc -> words[wc -> inUse].count = 1;
        wc -> inUse++;
    } else {

        wc -> words[pos].count++;
    }
}


int wordLookup (wordCount *wc, char *newWord) {

    for(int i = 0; i < wc -> inUse; i++){

        if(strcasecmp(wc -> words[i].word, newWord) == 0){

        return i;
        }
    }

    return -1;
}

/*
* description: Prints out the strings saved to wordCount and how many of those
* strings that have been found, also the total number of matches found.
* param[in]: wc - A pointer to the wordCount.
* param[in]: count - total number of matches found.
*/
void printWordCount(wordCount *wc, int count){

    for(int i = 0; i < wc -> inUse; i++){

        printf("%6s %d\n", wc -> words[i].word, wc -> words[i].count);
    }

    printf("Number of total words found: %d \n", count);
}

/*
* description: checks so program go the right amount of parameters and that it
* can open and read or write to file.
* param[in]: argc - number of parameters.
* param[in]: argv - Parameters strings.
* returns: 0 if failed and 1 if succeeded wit the tests.
*/
int fileValidation (int argc, char const *argv[]) {

    FILE *fp;

    if (argc != 2) {

        fprintf(stderr, "Invlid number of parameters - ");
        return 0;
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL) {

        fprintf(stderr, "Could not open '%s' to read - ", argv[1]);
        return 0;
    }
    fclose(fp);

    return 1;
}
