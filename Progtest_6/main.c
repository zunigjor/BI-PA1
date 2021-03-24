/**@file main.c
 * @author zunigjor@fit.cvut.cz
 * @date 2019
 * Soltuion to progtest n. 6.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define INIT_MEM_LEN 64
#define MAX_NUM_OF_RESULTS 50
/**
 * @struct Phrase struct handling the count and text.
 */
typedef struct S_PHRASE{
    double count;
    char * text;
} s_phrase;
/**
 * Function which turns the string to lowercase.
 * @param s Pointer to string to be turned to lowercase.
 */
void toLowerString(char * s){
    int strLen = strlen(s);
    for (int i = 0; i < strLen; ++i) {
        s[i] = tolower(s[i]);
    }
}
/**
 * Function which removes n characters from the begging of a string.
 * @param[in,out] StringInput Pointer to string (char arr[]).
 * @param[in] NumberOfCharsToDrop Number of chars to drop.
 */
void removeChars(char * stringInput, int numberOfCharsToDrop){
    char * stringPointer = stringInput + numberOfCharsToDrop;
    while (*stringPointer){
        *stringInput = *stringPointer;
        ++stringInput;
        ++stringPointer;
    }
    *stringInput = '\0';
}
/**
 * Function which frees the phrases database.
 * @param[in] database Pointer to phrase database.
 * @param[in] numOfPhrases Number of phrases./home/jorge/Documents/School/CVUT/2019_ZS/PA1/Progtest/Progtest_5
 */
void freePhrases(s_phrase * database, int numOfPhrases){
    for (int i = 0; i < numOfPhrases; ++i) {
        free(database[i].text);
    }
    free(database);
}
/**
 * Function which inserts phrase into database.
 * @param[in] phrasesDatabase Pointer to database of phrases.
 * @param[in] numOfPhrases Number of phrases in database.
 * @param[in] count Count of phrase to be inserted.
 * @param[in] text Phrase text.
 */
void insertPhrase(s_phrase * phrasesDatabase, int * numOfPhrases, double count, char * text){
    int i;
    for (i = (*numOfPhrases) - 1; i >= 0 && phrasesDatabase[i].count < count ; i--) {
        phrasesDatabase[i+1] = phrasesDatabase[i];
    }
    phrasesDatabase[i+1].count = count;
    phrasesDatabase[i+1].text = text;
    (*numOfPhrases)++;
}
/**
 * Function which reads input and handles saving and inserting into the phrases database.
 * @param[in,out] phrasesDatabasePtr Pointer to phrases database.
 * @param[in,out] numberOfPhrasesPtr Number of phrases.
 * @return 0 if everything went well, 1 otherwise.
 */
int readPhrases(s_phrase ** phrasesDatabasePtr, int * numberOfPhrasesPtr){
    s_phrase * phrasesDatabase = (s_phrase *)malloc(INIT_MEM_LEN * sizeof(*phrasesDatabase));
    int numberOfPhrases = 0;
    int maxNumOfPhrases = INIT_MEM_LEN;
    while(1){
        char * phraseText = NULL;
        size_t phraseTextLen = 0;
        int charsRead = getline(&phraseText, &phraseTextLen, stdin);
        if (charsRead == -1){
            free(phraseText);
            freePhrases(phrasesDatabase,numberOfPhrases);
            return 1;
        }
        if (phraseText[0] == '\n'){
            free(phraseText);
            break;
        }
        phraseText[charsRead - 1] = '\0';
        double count;
        int numOfCharsRead;
        char colon;
        int ret = sscanf(phraseText,"%lf%c%n", &count,&colon,&numOfCharsRead);
        if(ret != 2 || colon != ':'){
            free(phraseText);
            freePhrases(phrasesDatabase, numberOfPhrases);
            return 1;
        }
        if(numberOfPhrases >= maxNumOfPhrases){
            s_phrase * morePhrases = (s_phrase*)realloc(phrasesDatabase, maxNumOfPhrases * 2 * sizeof(*morePhrases));
            if(morePhrases == NULL){
                free(phraseText);
                freePhrases(phrasesDatabase, numberOfPhrases);
                return 1;
            }
            phrasesDatabase = morePhrases;
            maxNumOfPhrases *= 2;
        }
        removeChars(phraseText,numOfCharsRead);
        insertPhrase(phrasesDatabase, &numberOfPhrases, count, phraseText);
    }
    if(numberOfPhrases < 1){
        freePhrases(phrasesDatabase, numberOfPhrases);
        return 1;
    }
    *phrasesDatabasePtr = phrasesDatabase;
    *numberOfPhrasesPtr = numberOfPhrases;
    return 0;
}
/**
 * Function which prints out the results.
 * @param phrasesDatabase
 * @param numOfResults
 */
void printResults(s_phrase * phrasesDatabase, int numOfResults){
    printf("Nalezeno: %d\n",numOfResults);
    for (int i = 0; i < numOfResults; ++i) {
        printf("> %s\n",phrasesDatabase[i].text);
    }
}
/**
 * Function which looks for string in the phrases database.
 * @param[in] phrasesDatabase Pointer to phrases database.
 * @param[in] numOfPhrases Number of phrases in database.
 * @param[in] searchStr String to be searched.
 */
void findStr(s_phrase * phrasesDatabase, int numOfPhrases, char * searchStr){
    char * strCpyInLowercase = NULL;
    int strLen;
    int numOfResults = 0;
    s_phrase resultDatabase[50];
    for (int i = 0; i < numOfPhrases; ++i) {
        strLen = strlen(phrasesDatabase[i].text);
        strCpyInLowercase = (char*) malloc(strLen * sizeof(strCpyInLowercase));
        strcpy(strCpyInLowercase,phrasesDatabase[i].text);
        toLowerString(strCpyInLowercase);
        char * pos = strstr(strCpyInLowercase,searchStr);
        if (pos != NULL && numOfResults < MAX_NUM_OF_RESULTS){
            resultDatabase[numOfResults] = phrasesDatabase[i];
            numOfResults++;
        }
        free(strCpyInLowercase);
    }
    printResults(resultDatabase,numOfResults);
}
/**
 * Function which reads and searches for the phrase.
 * @return 0 if everything went well, 1 otherwise.
 */
void readSearch(s_phrase * phrasesDatabase,int numOfPhrases){
    char * searchStr = NULL;
    size_t searchStrLen = 0;
    int charsRead;
    while(1){
        if ((charsRead = getline(&searchStr, &searchStrLen, stdin)) == -1){
            free(searchStr);
            break;
        }
        searchStr[charsRead - 1] = '\0';
        int numOfCharsRead;
        sscanf(searchStr, " %n", &numOfCharsRead);
        toLowerString(searchStr);
        removeChars(searchStr, numOfCharsRead);
        findStr(phrasesDatabase,numOfPhrases,searchStr);
    }
}
int main(void) {
    printf("Casto hledane fraze:\n");
    s_phrase * phrasesDatabase = NULL;
    int numberOfPhrases = 0;
    if (readPhrases(&phrasesDatabase, &numberOfPhrases) == 1){
        printf("Nespravny vstup.\n");
        return 1;
    }
    printf("Hledani:\n");
    readSearch(phrasesDatabase,numberOfPhrases);
    freePhrases(phrasesDatabase,numberOfPhrases);
    return 0;
}
