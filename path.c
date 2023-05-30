#include "path.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void addSpace(char** outputText, int* currentRow, int spazi){
    for(int i = 0; i < spazi; i++){
        strcat(outputText[*currentRow], " ");      
    }
}

char* getPath(const char* txt_path){
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    char *result = malloc(strlen(cwd) + strlen(txt_path) + 6);
    strcpy(result, cwd);
    strcat(result, "/");
    strcat(result, txt_path);
    return result;
}

// per calcolare la lunghezza della singola parola
int len(char* word) {
    double lenW = 0;
    int value = 0;
        for (int i = 0; word[i] != '\0'; i ++)
        {
            value = (int) word[i];
            
            if(value == -17 | value == -69 | value == -65){
            }
            else if(value == 92){
                lenW += 0.5;
            }
            else if(value < 0){
                lenW += 0.5;
            }
            else{
                lenW += 1;
            }
        }
    return (int) lenW;
}

char* concat(const char *s1, const char *s2) {
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    strcpy(result, s1);
    strcat(result, s2);
    return result; 
}
