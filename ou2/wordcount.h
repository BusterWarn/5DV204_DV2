typedef struct word{

    char word[7];
    int count;
} word;

typedef struct wordCount{

    int inUse;
    int capacity;
    word *words;
} wordCount;

/*
* Description: Creates a allocates memory for and create an empty WordCount.
* return: Empty wordCount.
*/
wordCount *wordCountEmpty ();

/*
* description: Reads strings one by one from the file and checks the strings
* against the regular expression and saves any matches to the wordCount. Also
* counts nummber of matches.
* param[in]: wc - A pointer to the wordCount.
* param[in]: regex - The compiled regular expression.
* param[in]: inFile - pointer to the file stream.
* return: the number of matches found.
*/
int calculateWordCount(wordCount *wc, regex_t regex, FILE *inFile);
/*
* description: Reallocates memeory if the wordCount is full, then checks if the
* string allready exist in wordCount to then either add to the count of a
* string int wordCount or add a new string to wordCount.
* param[in]: wc - A pointer to the wordCount.
* param[in]: newWord - Pointer to the string to be added to wordCount.
* return: updated wordCount.
*/
void addWord(wordCount *wc, char* newWord);

/*
* description: Compares a string to the strings already in the wordCount, if
* there is a match it returns the index of the matched string in wordCount,
* otherwise -1.
* param[in]: wc - A pointer to the wordCount.
* param[in]: newWord - Pointer to the string to be compared to strings already
* in wordCount.
* return: The index of the matched word in WordCount, -1 if no match is found.
*/
int wordLookup (wordCount *wc, char *newWord);

/*
* description: Prints out the strings saved to wordCount and how many of those
* strings that have been found, also the total number of matches found.
* param[in]: wc - A pointer to the wordCount.
* param[in]: count - total number of matches found.
*/
void printWordCount(wordCount *wc, int count);

/*
* description: checks so program go the right amount of parameters and that it
* can open and read or write to file.
* param[in]: argc - number of parameters.
* param[in]: argv - Parameters strings.
* returns: 0 if failed and 1 if succeeded wit the tests.
*/
int fileValidation (int argc, char const *argv[]);
