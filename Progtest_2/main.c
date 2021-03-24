// INCLUDE
#include <stdio.h>
#include <string.h>
// definitions
#define MAX_SIZE 100
// FUNCTIONS
/*
 * The list function writes a list of palindromes in set base.
 * Base is set by first parameter.
 * The palindromes are in an interval set by ints LOW and HIGH.
 */
void list (int,long long int, long long int);
/*
 * The count function prints out how many palindrome numbers
 * in set base are in the interval LOW to HIGH.
 * Base is set by first parameter.
 */
void count(int, long long int ,long long int);
/*
 * Checks if number in decimal is a palindrome in set base.
 * Base set by first parameter.
 * return: yes := 1  /  no := 0
 */
int isPalindromeInBase(char*,int,long long int);
/*
 * Func returns decimal number converted to any base number as string.
 */
char* numToStringInBase(char*,int,long long int);
/*
 * Func takes in string of chars and checks if it is a palindrome.
 * return: yes := 1  /  no := 0
 */
int isPalindrome(char*);
/*
 *  Uses ASCII table to convert number to appropriate char.
 */
char valueToChar(int);
// MAIN
int main(void) {
    printf("Vstupni intervaly:\n");
    char command; //  c || l
    int base;     //  <2;36>
    long long int low;      //  interval low
    long long int high;     // interval high
    int repeat,scan_ret;
    do{
        repeat = 0;
        scan_ret = scanf(" %c %d %lld %lld",&command,&base,&low,&high);
        if (scan_ret == EOF){
            return 0;
        } else if (scan_ret != 4
                || base < 2 || base > 36
                || (command != 'c' && command != 'l')
                || low > high || low < 0 || high < 0){
            printf("Nespravny vstup.\n");
            return 1;
        }
        if (command == 'l'){
            list(base,low,high);
            repeat = 1;
        } else if (command == 'c'){
            count(base, low, high);
            repeat = 1;
        }
    }while(repeat);
    return 0;

}

void list (int base, long long int low, long long int high){
    if (low == 0){
        printf("%lld = ",low);
        printf("0");
        printf(" (%d)\n",base);
        low++;
    }
    for (; low <= high; ++low){
        char numAsString[MAX_SIZE];
        if (isPalindromeInBase(numAsString, base, low)){
            printf("%lld = ",low);
            printf("%s",numAsString);
            printf(" (%d)\n",base);
        }
    }
}

void count(int base, long long int low,long long int high){
    int count = 0;
    char numAsString[MAX_SIZE];
    for (; low <= high; ++low) {
        if (isPalindromeInBase(numAsString, base, low)){
            count++;
        }
    }
    printf("Celkem: %d\n",count);
}

int isPalindromeInBase(char * numInBase,int base, long long int num){
    numInBase = numToStringInBase(numInBase, base,num);
    return isPalindrome(numInBase);
}

char* numToStringInBase(char * result, int base,long long int num){
    int i = 0;
    while (num > 0){
        result[i++] = valueToChar(num % base);
        num /= base;
    }
    result[i] = '\0';
    return result;
}

int isPalindrome(char* str){
    int l = 0;
    int r = strlen(str) - 1;
    while(r>l){
        if(str[l++] != str[r--]){
            return 0;
        }
    }
    return 1;
}

char valueToChar(int val){
    if (val >= 0 && val <= 9){
        return (char)(val + '0');
    } else {
        return (char)(val - 10 + 'a');
    }
}
