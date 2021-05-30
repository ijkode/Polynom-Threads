//polynom - threads
//author: Liran Libster
//description: this program separate a big polynom into small polynoms and let threads count each one, then summaries all the results.

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <pthread.h>

#define MAX 512

void freeArray(char ** array, int numberofWords);

int countExps(char * word);

char ** buildArray(char * word, int numofExps);

void * expressionSolution(void * polynom);

int value = 0;
int solution = 0;

int main() {

    char str[MAX];
    char ** array;

    while (1) {

        solution = 0;
        printf("Please enter polynom and value:\n");
        fgets(str, MAX, stdin);
        str[strlen(str) - 1] = '\0';
        int status, i;
        int spaceFlag = 0;
        if (strcmp(str, "done") == 0) {
            break;
        }

        for (i = 0; i < strlen(str); i++) {
            if (str[i] == ' ') {
                value = atoi( & str[i + 1]);
            }
        }

        for (i = 0; i < strlen(str); i++) { //loop to check if the polynom entered correctly without additional spaces
            if (str[i] == ',') {
                if (str[i + 2] == ' ') {
                    printf("wrong input, fix spaces!\n");
                    spaceFlag = 1;
                    break;
                }
                break;
            }
            if (str[i] == ' ') {
                printf("wrong input, fix spaces!\n");
                spaceFlag = 1;
                break;
            }
        }

        int numberofExps = countExps(str);
        array = buildArray(str, numberofExps);
        pthread_t thread[numberofExps];

        for (i = 0; i < numberofExps; i++) {
            status = pthread_create( & thread[i], NULL, expressionSolution, (void * ) array[i]);
            if (status != 0) {
                fputs("pthread create failed", stderr);
                exit(EXIT_FAILURE);
            }
        }

        for (i = 0; i < numberofExps; i++) {
            pthread_join(thread[i], NULL);
        }

        if (spaceFlag == 0) {
            printf("%d\n", solution);
        }
        freeArray(array, numberofExps);
        value = 0;
        solution = 0;

    }

}

char ** buildArray(char * word, int numofExps) {

    char ** Arr;
    Arr = (char ** ) malloc((numofExps + 1) * sizeof(char * ));
    if (Arr == NULL) {
        fprintf(stderr, "ERR");
        exit(1);
    }

    int charNum = 0; //count characters
    int arrCounter = 0;

    for (int j = 0; j < strlen(word); j++) {

        if (word[j] != '+') {

            charNum++;

            if (word[j + 1] == '+' || word[j + 1] == '\0') {

                char * newExp = malloc((charNum + 1) * sizeof(char));

                if (newExp == NULL) {
                    freeArray(Arr, numofExps);
                    fprintf(stderr, "ERR");
                    exit(1);
                }

                //insert current expression into newExp
                int counter = 0;
                for (int k = j - charNum + 1; k < (j - charNum + 1) + charNum; k++) {
                    newExp[counter] = word[k];
                    counter++;

                }

                newExp[counter] = '\0';
                Arr[arrCounter] = (char * ) malloc(sizeof(char) * (charNum + 1));
                if (Arr[arrCounter] == NULL) {
                    free(newExp);
                    freeArray(Arr, numofExps);
                    fprintf(stderr, "ERR");
                    exit(1);
                }
                strcpy(Arr[arrCounter], newExp);
                free(newExp);
                arrCounter++;
                charNum = 0;

            }
        }
    }

    return Arr;
}

int countExps(char * word) {

    int counter = 1;
    for (int i = 0; i < strlen(word); i++) {
        if (word[i] == '+') {
            counter++;
        }
    }
    return counter;
}

void freeArray(char ** array, int numberofWords) {

    for (int setFree = 0; setFree < numberofWords; setFree++) {
        free(array[setFree]);
    }
    free(array);

}

void * expressionSolution(void * polynom) {

    char * word = (char * ) polynom;
    char coefficient[MAX];;
    char exponent[MAX];
    int coeff, exp;
    int result = value;
    int flag = 0;
    int xcheck = 0;
    if (word[0] == 'x' && word[1] == '\0' || word[0] == 'x' && word[1] == ',') {
        flag = 1;
    }

    for (int i = 0; i < strlen(word); i++) { //loop that build chars for the coefficient and the exponent.
        if (word[i] == '*') {
            if (word[i + 2] == '^') {
                exponent[0] = word[i + 3];
            } else {
                exponent[0] = 1;
            }
            break;
        }
        if (word[i] == ',') {
            break;
        }
        coefficient[i] = word[i];
    }

    coefficient[strlen(coefficient)] = '\0';

    for (int j = 0; j < strlen(word); j++) {
        if (word[j] == 'x') {
            xcheck = 1;
        }
    }

    if (xcheck == 0) { //if there is no x in the expression add only the coefficient to the solution
        coeff = atoi(coefficient);
        solution += coeff;
    } else {
        for (int j = 0; j < strlen(word); j++) {
            if (word[j] == '^') {
                exponent[0] = word[j + 1];
            }
        }

        coeff = atoi(coefficient);
        exp = atoi(exponent);
        if (coeff == 0) {
            coeff = 1;
        }
        if (flag == 0) {
            for (int j = 0; j < exp - 1; j++) {
                result *= value;
            }

            if (exp == 0) {
                result = coeff * value;
            } else {
                result *= coeff;
            }
        } else {
            result += 0;
        }
        solution += result;
    }
    return 0;
}